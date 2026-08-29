import numpy as np
import pandas as pd
from scipy.stats import pearsonr, kurtosis, skew, entropy, percentileofscore
from scipy.signal import find_peaks, correlate, hilbert
from scipy.fft import fft, fftfreq
from itertools import combinations, product
from sklearn.preprocessing import PolynomialFeatures
from sklearn.decomposition import PCA
from sklearn.cluster import KMeans
import warnings
warnings.filterwarnings('ignore')

data = np.array([0,1,1,0,1,0,0,0,0,1,1,0,0,1,1,1,0,0,1,1,1,1,1,0,0,1,0,1,0,1,1,1,0,0,0,1])
n = len(data)
y_data = data[1:]

# ==================== ФУНКЦИЯ ЦЕЛЕВОГО КОДИРОВАНИЯ ====================
def target_encoding(series, target, t, past_window, smooth=1):
    """
    Целевое кодирование сглаженного значения
    series: исходный ряд
    target: целевая переменная (следующий шаг)
    t: текущая позиция
    past_window: скользящее окно для агрегации
    smooth: параметр сглаживания (чем больше, тем сильнее сглаживание)
    """
    if t < past_window:
        return 0.5  # возвращаем априорную вероятность
    
    # Берем прошлые значения
    past_values = series[max(0, t-past_window):t]
    past_targets = target[max(0, t-past_window):t]
    
    if len(past_values) == 0:
        return 0.5
    
    # Глобальное среднее (априорная вероятность)
    global_mean = np.mean(target[:t]) if t > 0 else 0.5
    
    # Для каждого уникального значения считаем средний таргет
    unique_vals = np.unique(past_values)
    encoding_dict = {}
    
    for val in unique_vals:
        mask = past_values == val
        if mask.sum() > 0:
            category_mean = past_targets[mask].mean()
            # Сглаживание: (сумма таргетов + smooth * global_mean) / (количество + smooth)
            smoothed = (past_targets[mask].sum() + smooth * global_mean) / (mask.sum() + smooth)
            encoding_dict[val] = smoothed
        else:
            encoding_dict[val] = global_mean
    
    # Текущее значение
    current_val = series[t]
    return encoding_dict.get(current_val, global_mean)

def create_ultra_features(series, target, t):
    """Создает МАКСИМАЛЬНО расширенный набор признаков с target encoding"""
    features = {}
    
    # ==================== 1. ВСЕ ЛАГИ ====================
    for lag in range(1, min(10, t+2)):
        features[f'lag_{lag}'] = series[t - lag]
    
    # ==================== 2. ЦЕЛЕВОЕ КОДИРОВАНИЕ ====================
    # Базовое target encoding с разными окнами и сглаживанием
    for window in [3, 5, 7, 10, 15, 20]:
        for smooth in [0.5, 1, 2, 5]:
            if t >= 1:
                te_value = target_encoding(series, target, t, window, smooth)
                features[f'target_encoding_smooth_{smooth}_window_{window}'] = te_value
    
    # Особо важные по вашему запросу
    features['target_encoding_smooth_1'] = target_encoding(series, target, t, 10, 1)
    features['target_encoding_smooth_1_window_5'] = target_encoding(series, target, t, 5, 1)
    features['target_encoding_smooth_1_window_3'] = target_encoding(series, target, t, 3, 1)
    features['target_encoding_smooth_1_window_20'] = target_encoding(series, target, t, 20, 1)
    
    # Экспоненциальное целевое кодирование (свежие данные важнее)
    if t >= 5:
        weights = np.exp(np.linspace(-2, 0, min(t, 20)))
        weights = weights / weights.sum()
        
        past_series = series[max(0, t-20):t]
        past_targets = target[max(0, t-20):t]
        
        if len(past_series) == len(weights[-len(past_series):]):
            weighted_sum = 0
            total_weight = 0
            for i, (val, weight) in enumerate(zip(past_series, weights[-len(past_series):])):
                weighted_sum += val * weight
                total_weight += weight
            
            # Кодируем текущее значение
            current_val = series[t]
            if total_weight > 0:
                # Находим взвешенную вероятность для текущего значения
                mask = past_series == current_val
                if mask.sum() > 0:
                    exp_encoding = np.sum(past_targets[mask] * weights[-len(past_series):][mask]) / np.sum(weights[-len(past_series):][mask])
                    features['target_encoding_exp'] = exp_encoding
                else:
                    features['target_encoding_exp'] = np.mean(past_targets)
    
    # ==================== 3. РАСШИРЕННЫЕ СКОЛЬЗЯЩИЕ СТАТИСТИКИ ====================
    for window in [2, 3, 4, 5, 6, 7, 8, 9, 10]:
        if t - window + 1 >= 0:
            window_data = series[t-window+1:t+1]
            
            # Базовые статистики
            features[f'sum_{window}'] = window_data.sum()
            features[f'mean_{window}'] = window_data.mean()
            features[f'std_{window}'] = window_data.std()
            features[f'var_{window}'] = window_data.var()
            features[f'min_{window}'] = window_data.min()
            features[f'max_{window}'] = window_data.max()
            features[f'range_{window}'] = window_data.max() - window_data.min()
            features[f'median_{window}'] = np.median(window_data)
            
            # Медианные отклонения
            features[f'mad_{window}'] = np.mean(np.abs(window_data - np.median(window_data)))
            features[f'iqr_{window}'] = np.percentile(window_data, 75) - np.percentile(window_data, 25)
            
            # Скошенность и эксцесс
            if len(window_data) >= 3 and window_data.std() > 0:
                features[f'skew_{window}'] = skew(window_data)
                features[f'kurtosis_{window}'] = kurtosis(window_data, fisher=True)
            
            # Процентили
            for p in [10, 25, 75, 90]:
                features[f'percentile_{p}_{window}'] = np.percentile(window_data, p)
    
    # ==================== 4. НЕЛИНЕЙНЫЕ ТРАНСФОРМАЦИИ ПРИЗНАКОВ ====================
    # Взаимодействия между лагами
    lag_features = [features.get(f'lag_{i}', 0) for i in range(1, 6)]
    
    # Полиномиальные признаки (до 3 степени)
    for i in range(1, 6):
        if features.get(f'lag_{i}', None) is not None:
            val = features[f'lag_{i}']
            features[f'lag_{i}_sq'] = val ** 2
            features[f'lag_{i}_cube'] = val ** 3
            features[f'lag_{i}_exp'] = np.exp(val) - 1
            features[f'lag_{i}_log1p'] = np.log1p(val)
            features[f'lag_{i}_sigmoid'] = 1 / (1 + np.exp(-5*(val - 0.5)))
    
    # Взаимодействия пар лагов
    for i, j in combinations(range(1, 6), 2):
        if features.get(f'lag_{i}', None) is not None and features.get(f'lag_{j}', None) is not None:
            features[f'interaction_{i}_{j}'] = features[f'lag_{i}'] * features[f'lag_{j}']
            features[f'interaction_{i}_{j}_diff'] = abs(features[f'lag_{i}'] - features[f'lag_{j}'])
            features[f'interaction_{i}_{j}_sum'] = features[f'lag_{i}'] + features[f'lag_{j}']
    
    # ==================== 5. ДИНАМИЧЕСКИЕ ХАРАКТЕРИСТИКИ ====================
    if t >= 2:
        # Разности разных порядков
        features['diff_1'] = series[t] - series[t-1]
        features['diff_2'] = series[t] - 2*series[t-1] + series[t-2]
        if t >= 3:
            features['diff_3'] = series[t] - 3*series[t-1] + 3*series[t-2] - series[t-3]
        
        # Абсолютные разности
        features['abs_diff_1'] = abs(features['diff_1'])
        features['abs_diff_2'] = abs(features['diff_2'])
        
        # Относительные изменения
        if series[t-1] > 0:
            features['rel_change'] = (series[t] - series[t-1]) / series[t-1]
        
        # Скорость изменения
        features['change_velocity'] = features['diff_1']
        if t >= 2:
            features['change_acceleration'] = features['diff_2']
    
    # ==================== 6. ПАТТЕРНЫ ПЕРЕКЛЮЧЕНИЙ ====================
    for window in [2, 3, 4, 5, 6, 7, 8]:
        if t - window + 2 >= 0:
            window_data = series[t-window+1:t+1]
            switches = np.sum(window_data[1:] != window_data[:-1])
            features[f'switches_{window}'] = switches
            features[f'switch_rate_{window}'] = switches / max(1, window-1)
            
            # Длина самой длинной подсерии
            max_run = 1
            current_run = 1
            for k in range(1, len(window_data)):
                if window_data[k] == window_data[k-1]:
                    current_run += 1
                    max_run = max(max_run, current_run)
                else:
                    current_run = 1
            features[f'max_run_{window}'] = max_run
            features[f'max_run_ratio_{window}'] = max_run / window
    
    # ==================== 7. РАСШИРЕННЫЕ RUN СТАТИСТИКИ ====================
    if t == 0:
        features['run_length'] = 1
        features['run_position'] = 0
    else:
        # Текущая серия
        run = 1
        for i in range(t-1, -1, -1):
            if series[i] == series[t]:
                run += 1
            else:
                break
        features['run_length'] = run
        features['run_position'] = run
        
        # Позиция в серии (относительная)
        features['run_position_normalized'] = run / max(run, 10)
        
        # Предсказание остатка серии на основе исторических данных
        if run >= 2:
            # Историческая средняя длина серии
            historical_runs = []
            current_run_hist = 1
            for i in range(1, t):
                if series[i] == series[i-1]:
                    current_run_hist += 1
                else:
                    if current_run_hist >= 2:
                        historical_runs.append(current_run_hist)
                    current_run_hist = 1
            if current_run_hist >= 2:
                historical_runs.append(current_run_hist)
            
            if historical_runs:
                avg_run = np.mean(historical_runs)
                std_run = np.std(historical_runs)
                features['run_expected_remaining'] = max(0, avg_run - run)
                features['run_zscore'] = (run - avg_run) / max(std_run, 0.1)
    
    # ==================== 8. СЛОЖНЫЕ ЭНТРОПИЙНЫЕ МЕРЫ ====================
    for window in [3, 4, 5, 6, 7, 8]:
        if t - window + 1 >= 0:
            window_data = series[t-window+1:t+1]
            
            # Энтропия Шеннона
            p1 = window_data.mean()
            p0 = 1 - p1
            shannon = 0
            if p0 > 0: shannon -= p0 * np.log2(p0)
            if p1 > 0: shannon -= p1 * np.log2(p1)
            features[f'entropy_shannon_{window}'] = shannon
            
            # Энтропия Реньи (alpha=2)
            renyi2 = -np.log2(p0**2 + p1**2) if (p0**2 + p1**2) > 0 else 0
            features[f'entropy_renyi2_{window}'] = renyi2
            
            # Энтропия Тсаллиса
            q = 2
            tsallis = (1 - (p0**q + p1**q)) / (q - 1) if (p0**q + p1**q) > 0 else 0
            features[f'entropy_tsallis_{window}'] = tsallis
            
            # Сложность по Лемпелю-Зиву
            if window >= 4:
                substrings = set()
                for length in range(1, window//2 + 2):
                    for start in range(0, window - length + 1):
                        substrings.add(tuple(window_data[start:start+length]))
                features[f'complexity_lz_{window}'] = len(substrings) / (window * np.log2(window) if window > 1 else 1)
    
    # ==================== 9. СПЕКТРАЛЬНЫЕ ПРИЗНАКИ ====================
    if t >= 8:
        # БПФ на последних 8, 16 точках
        for fft_window in [8, 16]:
            if t >= fft_window - 1:
                fft_data = series[t-fft_window+1:t+1]
                fft_vals = np.abs(fft(fft_data))
                # Основные частоты
                features[f'fft_dominant_{fft_window}'] = np.argmax(fft_vals[1:fft_window//2]) if len(fft_vals) > 2 else 0
                features[f'fft_energy_{fft_window}'] = np.sum(fft_vals**2)
                features[f'fft_max_{fft_window}'] = np.max(fft_vals)
                # Отношение сигнал/шум
                if len(fft_vals) > 2:
                    features[f'fft_snr_{fft_window}'] = np.max(fft_vals[1:]) / np.mean(fft_vals[1:]) if np.mean(fft_vals[1:]) > 0 else 0
    
    # ==================== 10. АВТОКОРРЕЛЯЦИИ ====================
    for lag in [1, 2, 3, 4, 5, 6, 7]:
        if t >= lag:
            autocorr = np.corrcoef(series[t-lag:t+1], series[t-lag:t+1])[0,1] if len(series[t-lag:t+1]) > 1 else 0
            features[f'autocorr_lag_{lag}'] = autocorr if not np.isnan(autocorr) else 0
    
    # Частичная автокорреляция (упрощенно)
    for lag in [2, 3, 4]:
        if t >= lag:
            partial = features.get(f'autocorr_lag_{lag}', 0)
            for i in range(1, lag):
                partial -= features.get(f'autocorr_lag_{i}', 0) * features.get(f'autocorr_lag_{lag-i}', 0)
            features[f'partial_autocorr_{lag}'] = partial
    
    # ==================== 11. ПАТТЕРНЫ КАК ЧИСЛА (все возможные длины) ====================
    for bits in [2, 3, 4, 5, 6]:
        if t - bits + 1 >= 0:
            pattern = series[t-bits+1:t+1]
            pattern_int = sum([pattern[i] * (2**(bits-1-i)) for i in range(bits)])
            features[f'pattern_int_{bits}bits'] = pattern_int
            features[f'pattern_int_{bits}bits_norm'] = pattern_int / (2**bits - 1)
            
            # Аномальность паттерна (как часто встречался в истории)
            if t >= bits * 2:
                history = series[:t]
                pattern_count = 0
                total_windows = max(1, len(history) - bits + 1)
                for i in range(total_windows):
                    if np.array_equal(history[i:i+bits], pattern):
                        pattern_count += 1
                features[f'pattern_freq_{bits}bits'] = pattern_count / total_windows
                features[f'pattern_anomaly_{bits}bits'] = -np.log(pattern_count / total_windows + 0.001)
    
    # ==================== 12. ПРОГНОЗНЫЕ ПРИЗНАКИ НА ОСНОВЕ МАРКОВСКИХ ЦЕПЕЙ ====================
    for order in [1, 2, 3]:
        if t >= order:
            # Состояние Марковской цепи
            state = tuple(series[t-order:t+1])
            features[f'markov_state_{order}'] = hash(state) % 1000  # хеш состояния
            
            # Вероятность перехода из текущего состояния (эмпирическая)
            if t >= order * 2:
                # Считаем переходы из этого состояния в истории
                state_transitions = {}
                for i in range(order, t):
                    prev_state = tuple(series[i-order:i+1])
                    next_val = series[i+1] if i+1 < len(series) else None
                    if next_val is not None:
                        if prev_state not in state_transitions:
                            state_transitions[prev_state] = [0, 0]
                        state_transitions[prev_state][next_val] += 1
                
                if state in state_transitions:
                    total = sum(state_transitions[state])
                    if total > 0:
                        features[f'markov_prob_0_{order}'] = state_transitions[state][0] / total
                        features[f'markov_prob_1_{order}'] = state_transitions[state][1] / total
                        features[f'markov_entropy_{order}'] = -np.sum([p * np.log2(p+1e-10) for p in [features[f'markov_prob_0_{order}'], features[f'markov_prob_1_{order}']]])
    
    # ==================== 13. ФРАКТАЛЬНЫЕ И САМОПОДОБНЫЕ ПРИЗНАКИ ====================
    if t >= 4:
        # Хёрст показатель (упрощенный)
        for window in [4, 8]:
            if t >= window:
                window_data = series[t-window+1:t+1]
                R = window_data.max() - window_data.min()
                S = window_data.std() if window_data.std() > 0 else 1
                features[f'hurst_{window}'] = np.log(R/S) / np.log(window) if R/S > 0 else 0
    
    # ==================== 14. КОМБИНИРОВАННЫЕ ЛОГИЧЕСКИЕ ПАТТЕРНЫ ====================
    if t >= 3:
        # Сложные логические условия
        features['pattern_010'] = int(series[t-2] == 0 and series[t-1] == 1 and series[t] == 0)
        features['pattern_101'] = int(series[t-2] == 1 and series[t-1] == 0 and series[t] == 1)
        features['pattern_001'] = int(series[t-2] == 0 and series[t-1] == 0 and series[t] == 1)
        features['pattern_110'] = int(series[t-2] == 1 and series[t-1] == 1 and series[t] == 0)
        features['pattern_000'] = int(series[t-2] == 0 and series[t-1] == 0 and series[t] == 0)
        features['pattern_111'] = int(series[t-2] == 1 and series[t-1] == 1 and series[t] == 1)
        
        # Альтернирующие паттерны
        features['alternating_3'] = int(series[t-2] != series[t-1] and series[t-1] != series[t])
        features['alternating_4'] = int(t >= 4 and series[t-3] != series[t-2] and series[t-2] != series[t-1] and series[t-1] != series[t])
    
    # ==================== 15. ВРЕМЕННЫЕ ИНТЕРВАЛЫ ====================
    # Время с последнего переключения
    if t == 0:
        features['time_since_switch'] = 0
    else:
        time_since = 0
        for i in range(t, 0, -1):
            if series[i] != series[i-1]:
                break
            time_since += 1
        features['time_since_switch'] = time_since
        features['time_since_switch_norm'] = time_since / max(time_since, 20)
    
    # Время до следующего переключения
    if t < len(series) - 1:
        time_until = 0
        for i in range(t, len(series)-1):
            if series[i] != series[i+1]:
                break
            time_until += 1
        features['time_until_switch'] = time_until
    else:
        features['time_until_switch'] = 0
    
    # Периодичность переключений
    if t >= 5:
        switches_positions = [i for i in range(1, t+1) if series[i] != series[i-1]]
        if len(switches_positions) >= 2:
            intervals = np.diff(switches_positions[-5:])
            features['switch_interval_mean'] = np.mean(intervals) if len(intervals) > 0 else 0
            features['switch_interval_std'] = np.std(intervals) if len(intervals) > 0 else 0
    
    # ==================== 16. ГЛОБАЛЬНЫЕ СТАТИСТИКИ ИСТОРИИ ====================
    if t >= 5:
        history = series[:t+1]
        features['global_mean'] = history.mean()
        features['global_std'] = history.std()
        features['global_sum'] = history.sum()
        features['global_skew'] = skew(history) if len(history) >= 3 and history.std() > 0 else 0
        features['global_kurtosis'] = kurtosis(history) if len(history) >= 4 and history.std() > 0 else 0
    
    # ==================== 17. ЛОКАЛЬНАЯ ПРЕДСКАЗУЕМОСТЬ ====================
    for window in [3, 4, 5, 6]:
        if t >= window:
            # Ошибка предсказания простым правилом большинства
            window_data = series[t-window:t]
            majority = 1 if window_data.sum() > window/2 else 0
            features[f'majority_error_{window}'] = abs(majority - series[t])
            
            # Скользящая точность предсказания лагом 1
            if t >= window + 1:
                predictions = [series[i-1] for i in range(t-window, t)]
                actuals = [series[i] for i in range(t-window, t)]
                accuracy = np.mean([p == a for p, a in zip(predictions, actuals)])
                features[f'lag1_accuracy_{window}'] = accuracy
    
    # ==================== 18. НЕЙРО-ВДОХНОВЛЕННЫЕ ПРИЗНАКИ ====================
    # Активационные функции от взвешенных сумм
    weights = [1.0, 0.8, 0.6, 0.4, 0.2]
    for k in range(2, 6):
        if t >= k:
            weighted_sum = sum(series[t-i] * weights[i] for i in range(min(k, len(weights))) if t-i >= 0)
            features[f'weighted_sum_{k}'] = weighted_sum
            features[f'tanh_weighted_{k}'] = np.tanh(weighted_sum - 0.5)
            features[f'sigmoid_weighted_{k}'] = 1 / (1 + np.exp(-5*(weighted_sum - 0.5)))
    
    # ==================== 19. РАЗНОСТИ ЛАГОВ ====================
    for lag1 in range(1, 5):
        for lag2 in range(lag1+1, 6):
            if features.get(f'lag_{lag1}', None) is not None and features.get(f'lag_{lag2}', None) is not None:
                features[f'lag_diff_{lag1}_{lag2}'] = features[f'lag_{lag1}'] - features[f'lag_{lag2}']
                features[f'lag_ratio_{lag1}_{lag2}'] = features[f'lag_{lag1}'] / (features[f'lag_{lag2}'] + 0.1)
                features[f'lag_product_{lag1}_{lag2}'] = features[f'lag_{lag1}'] * features[f'lag_{lag2}']
    
    # ==================== 20. ВОЛНОВЫЕ ПРИЗНАКИ ====================
    if t >= 4:
        # Локальные экстремумы
        if t >= 2 and t <= len(series)-2:
            is_peak = series[t] > series[t-1] and series[t] > series[t+1]
            is_valley = series[t] < series[t-1] and series[t] < series[t+1]
            features['is_peak'] = int(is_peak)
            features['is_valley'] = int(is_valley)
    
    # ==================== 21. TARGET ENCODING С РАЗНЫМИ АГРЕГАЦИЯМИ ====================
    # Кодирование на основе скользящего среднего таргета
    if t >= 5:
        for window in [3, 5, 7]:
            past_targets = target[max(0, t-window):t]
            if len(past_targets) > 0:
                features[f'target_ma_{window}'] = np.mean(past_targets)
                features[f'target_std_{window}'] = np.std(past_targets)
    
    # Разность между target encoding и простым средним
    if 'target_encoding_smooth_1' in features:
        features['te_vs_mean'] = features['target_encoding_smooth_1'] - features.get('mean_5', 0.5)
    
    return features

# ==================== ГЛАВНЫЙ ЦИКЛ ====================
print("Генерация признаков с TARGET ENCODING...")
all_features = []

for t in range(n-1):
    features = create_ultra_features(data, y_data, t)
    features['t'] = t
    all_features.append(features)

df_features = pd.DataFrame(all_features)
df_features['y'] = y_data

print(f"Сгенерировано {len(df_features.columns) - 2} признаков")
print(f"Временных точек: {len(df_features)}")

# Заполняем пропуски
df_features = df_features.fillna(0)

# Специально проверяем target_encoding_smooth_1
if 'target_encoding_smooth_1' in df_features.columns:
    print(f"\n✅ Признак 'target_encoding_smooth_1' успешно создан")
    print(f"   Диапазон значений: [{df_features['target_encoding_smooth_1'].min():.3f}, {df_features['target_encoding_smooth_1'].max():.3f}]")
    print(f"   Среднее: {df_features['target_encoding_smooth_1'].mean():.3f}")
    print(f"   Стандартное отклонение: {df_features['target_encoding_smooth_1'].std():.3f}")

# ==================== РАСШИРЕННАЯ КОРРЕЛЯЦИЯ ====================
print("\nВычисление корреляций...")
correlations = []
feature_names = [col for col in df_features.columns if col not in ['y', 't']]

for feature in feature_names:
    valid_mask = ~(df_features[feature].isna() | df_features['y'].isna())
    if valid_mask.sum() > 5:
        corr, p_value = pearsonr(df_features.loc[valid_mask, feature], 
                                  df_features.loc[valid_mask, 'y'])
        correlations.append({
            'feature': feature,
            'correlation': corr,
            'abs_correlation': abs(corr),
            'p_value': p_value
        })

results_df = pd.DataFrame(correlations)
results_df = results_df.sort_values('abs_correlation', ascending=False).reset_index(drop=True)

# ==================== ВЫВОД ТОП-50 ====================
print("\n" + "="*120)
print("🏆 ТОП-50 ЛУЧШИХ ПРИЗНАКОВ ПО КОРРЕЛЯЦИИ ПИРСОНА:")
print("="*120)
print(f"{'№':<5} {'Признак':<50} {'Корреляция':>12} {'p-value':>12}")
print("-"*120)

for i in range(min(50, len(results_df))):
    row = results_df.iloc[i]
    print(f"{i+1:<5} {row['feature']:<50} {row['correlation']:>12.4f} {row['p_value']:>12.6f}")

# ==================== ЛУЧШИЙ ПРИЗНАК ====================
best_feature = results_df.iloc[0]['feature']
best_corr = results_df.iloc[0]['correlation']

print("\n" + "="*120)
print(f"🎯 АБСОЛЮТНЫЙ ПОБЕДИТЕЛЬ: {best_feature}")
print(f"📈 Корреляция Пирсона: {best_corr:.4f}")
print(f"📊 p-value: {results_df.iloc[0]['p_value']:.6f}")
print("="*120)

# ==================== СПЕЦИАЛЬНЫЙ АНАЛИЗ TARGET ENCODING ====================
te_features = [f for f in results_df['feature'] if 'target_encoding' in f or 'target_ma' in f or 'te_vs' in f]
if te_features:
    print("\n📊 РЕЗУЛЬТАТЫ ДЛЯ TARGET ENCODING ПРИЗНАКОВ:")
    print("-"*80)
    for te_feat in te_features[:10]:
        te_row = results_df[results_df['feature'] == te_feat].iloc[0]
        print(f"   {te_feat:<45} → r = {te_row['correlation']:8.4f} (p={te_row['p_value']:.6f})")

# ==================== АНАЛИЗ ВЫСОКИХ КОРРЕЛЯЦИЙ ====================
high_corr = results_df[results_df['abs_correlation'] > 0.35]
print(f"\n🔥 Количество признаков с |r| > 0.35: {len(high_corr)}")
print(f"🔥 Количество признаков с |r| > 0.4: {len(results_df[results_df['abs_correlation'] > 0.4])}")
print(f"🔥 Количество признаков с |r| > 0.45: {len(results_df[results_df['abs_correlation'] > 0.45])}")

if len(high_corr) > 0:
    print("\n📋 ПРИЗНАКИ С |r| > 0.35:")
    for i, row in high_corr.head(20).iterrows():
        print(f"   {row['feature']:<50} → r = {row['correlation']:>8.4f}")

# ==================== СОЗДАНИЕ КОМПОЗИТНОГО ПРИЗНАКА ====================
print("\n" + "="*120)
print("🔧 СОЗДАНИЕ КОМПОЗИТНЫХ ПРИЗНАКОВ...")

# Берем лучшие признаки и комбинируем
top_features = results_df.head(10)['feature'].values
available_features = [f for f in top_features if f in df_features.columns]

# Создаем комбинированные признаки
for f1, f2 in combinations(available_features[:5], 2):
    composite_name = f'composite_{f1}_x_{f2}'
    df_features[composite_name] = df_features[f1] * df_features[f2]
    
    corr, p_val = pearsonr(df_features[composite_name], df_features['y'])
    if abs(corr) > 0.35:
        results_df = pd.concat([results_df, pd.DataFrame([{
            'feature': composite_name,
            'correlation': corr,
            'abs_correlation': abs(corr),
            'p_value': p_val
        }])], ignore_index=True)

# Сортируем заново
results_df = results_df.sort_values('abs_correlation', ascending=False).reset_index(drop=True)

# ==================== ИТОГОВЫЙ ЛУЧШИЙ ====================
best_feature_final = results_df.iloc[0]['feature']
best_corr_final = results_df.iloc[0]['correlation']

print(f"\n✨✨✨ ИТОГОВЫЙ ЛУЧШИЙ ПРИЗНАК: {best_feature_final}")
print(f"📈 ФИНАЛЬНАЯ КОРРЕЛЯЦИЯ: {best_corr_final:.4f}")
print("="*120)

# Показываем значения лучшего признака
print(f"\nЗначения лучшего признака:")
for i in range(min(20, len(df_features))):
    if best_feature_final in df_features.columns:
        val = df_features.loc[i, best_feature_final]
        print(f"t={i:2d}: {best_feature_final[:45]:45s} = {val:10.4f} -> y={df_features.loc[i, 'y']}")

# Сравнение с lag_2
if 'lag_2' in df_features.columns:
    lag2_corr = pearsonr(df_features['lag_2'], df_features['y'])[0]
    improvement = (best_corr_final - abs(lag2_corr)) / abs(lag2_corr) * 100
    print(f"\n📈 УЛУЧШЕНИЕ ОТНОСИТЕЛЬНО lag_2 ({lag2_corr:.4f}): {improvement:.1f}%")

# Сохраняем результаты
df_features.to_csv('ultra_features_with_te.csv', index=False)
results_df.to_csv('ultra_correlations_with_te.csv', index=False)
print(f"\n✅ Сохранено в 'ultra_features_with_te.csv' и 'ultra_correlations_with_te.csv'")