import numpy as np
from sklearn.linear_model import LogisticRegression
from sklearn.metrics import roc_auc_score
from scipy import stats
from sklearn.preprocessing import PolynomialFeatures

def calculate_bic(model, X, y):
    """Рассчитывает BIC для логистической регрессии"""
    n = len(y)
    k = model.coef_.shape[1] + 1
    
    y_pred_proba = model.predict_proba(X)[:, 1]
    epsilon = 1e-10
    y_pred_proba = np.clip(y_pred_proba, epsilon, 1 - epsilon)
    y = np.array(y)
    
    log_likelihood = np.sum(y * np.log(y_pred_proba) + (1 - y) * np.log(1 - y_pred_proba))
    bic = -2 * log_likelihood + k * np.log(n)
    
    return bic, log_likelihood, k

def create_geometric_enhanced_features(sequence, lag=4):
    """
    Создаёт признаки, которые дополняют геометрическую модель
    Вместо 35 случайных признаков - только те, что имеют теоретическое обоснование
    """
    X, y = [], []
    
    for i in range(lag, len(sequence)):
        window = sequence[i-lag:i]
        
        # 1. Основные статистики (нормализованные)
        mean_val = np.mean(window)
        std_val = np.std(window) if np.std(window) > 0 else 0.001
        
        # 2. Отношение числа смен состояния к длине
        transitions = sum(1 for j in range(len(window)-1) if window[j] != window[j+1])
        transition_rate = transitions / (len(window) - 1)
        
        # 3. Экспоненциально взвешенное среднее (подчёркивает недавние события)
        weights = np.exp(np.linspace(-1, 0, lag))
        weights = weights / weights.sum()
        exp_weighted_mean = np.sum(window * weights)
        
        # 4. Количество успехов в окне
        successes = np.sum(window)
        failure_rate = 1 - successes / lag
        
        # 5. Максимальная длина серии успехов
        max_success_run = 0
        current_run = 0
        for val in window:
            if val == 1:
                current_run += 1
                max_success_run = max(max_success_run, current_run)
            else:
                current_run = 0
        
        # 6. Максимальная длина серии неудач
        max_failure_run = 0
        current_run = 0
        for val in window:
            if val == 0:
                current_run += 1
                max_failure_run = max(max_failure_run, current_run)
            else:
                current_run = 0
        
        # 7. Отношение серий (чем больше, тем более "кластеризованы" данные)
        run_ratio = max_success_run / (max_failure_run + 1)
        
        # 8. "Память" - корреляция между первыми и последними значениями
        half = lag // 2
        first_half = np.mean(window[:half])
        second_half = np.mean(window[half:])
        memory_effect = second_half - first_half
        
        # 9. Нормализованные лаги (уже не 0/1, а отклонение от среднего)
        normalized_lags = (window - mean_val) / std_val
        
        # 10. Геометрическая вероятность (предсказание самой геометрической модели)
        # Нужно вычислить на основе твоей C++ модели
        # Здесь для примера - простая эвристика
        p_success = (successes + 1) / (lag + 2)  # Laplace smoothing
        geometric_prob = 1 - (1 - p_success) ** (max_failure_run + 1)
        
        features = [
            transition_rate,           # частота смен состояний
            exp_weighted_mean,         # взвешенное среднее
            successes / lag,           # плотность успехов
            max_success_run,           # максимальная серия успехов
            max_failure_run,           # максимальная серия неудач
            run_ratio,                 # отношение серий
            memory_effect,             # эффект памяти
            normalized_lags[-1],       # последнее нормализованное значение
            geometric_prob,            # геометрическая вероятность
            np.mean(normalized_lags),  # среднее нормализованное
        ]
        
        X.append(features)
        y.append(sequence[i])
    
    return np.array(X), np.array(y)

def create_polynomial_features(sequence, lag=4, degree=2):
    """
    Полиномиальные признаки от базовых лагов
    Могут уловить нелинейные зависимости
    """
    X_base, y = [], []
    for i in range(lag, len(sequence)):
        X_base.append(sequence[i-lag:i])
        y.append(sequence[i])
    
    X_base = np.array(X_base)
    poly = PolynomialFeatures(degree=degree, include_bias=False)
    X_poly = poly.fit_transform(X_base)
    
    return X_poly, np.array(y)

def create_interaction_features(sequence, lag=4):
    """
    Признаки взаимодействия между лагами
    """
    X, y = [], []
    
    for i in range(lag, len(sequence)):
        window = sequence[i-lag:i]
        
        features = list(window)  # базовые лаги
        
        # Попарные взаимодействия
        for j in range(lag):
            for k in range(j+1, lag):
                features.append(window[j] * window[k])  # AND
                features.append(window[j] + window[k])  # OR
        
        # Суммы и разности
        features.append(np.sum(window))  # общее число успехов
        features.append(np.sum([window[j] * (lag - j) for j in range(lag)]))  # взвешенная сумма
        
        X.append(features)
        y.append(sequence[i])
    
    return np.array(X), np.array(y)

# Твои данные
sample = [0,0,0,0,1,1,0,1,0,0,0,0,1,1,0,0,1,1,1,0,0,1,1,1,1,1,0,0,1,0,1,0,1,1]

print("=== Поиск модели, которая победит геометрическую ===\n")
print(f"Бенчмарк - Геометрическая модель: AUC = 0.8056, BIC = 43.33, параметров = 2\n")

# 1. Модель с геометрически-усиленными признаками
X_geom_enhanced, y_geom_enhanced = create_geometric_enhanced_features(sample, lag=4)
model_geom_enhanced = LogisticRegression(random_state=42, max_iter=2000, C=0.1)
model_geom_enhanced.fit(X_geom_enhanced, y_geom_enhanced)
auc_geom_enhanced = roc_auc_score(y_geom_enhanced, model_geom_enhanced.predict_proba(X_geom_enhanced)[:, 1])
bic_geom_enhanced, ll_geom_enhanced, k_geom_enhanced = calculate_bic(model_geom_enhanced, X_geom_enhanced, y_geom_enhanced)

# 2. Полиномиальные признаки (степень 2)
X_poly, y_poly = create_polynomial_features(sample, lag=4, degree=2)
model_poly = LogisticRegression(random_state=42, max_iter=2000, C=0.01)
model_poly.fit(X_poly, y_poly)
auc_poly = roc_auc_score(y_poly, model_poly.predict_proba(X_poly)[:, 1])
bic_poly, ll_poly, k_poly = calculate_bic(model_poly, X_poly, y_poly)

# 3. Признаки взаимодействия
X_interact, y_interact = create_interaction_features(sample, lag=4)
model_interact = LogisticRegression(random_state=42, max_iter=2000, C=0.01)
model_interact.fit(X_interact, y_interact)
auc_interact = roc_auc_score(y_interact, model_interact.predict_proba(X_interact)[:, 1])
bic_interact, ll_interact, k_interact = calculate_bic(model_interact, X_interact, y_interact)

# 4. Комбинация лучших признаков (отбор вручную)
def create_best_features(sequence, lag=4):
    """Комбинация самых многообещающих признаков"""
    X, y = [], []
    
    for i in range(lag, len(sequence)):
        window = sequence[i-lag:i]
        
        # Лучшие признаки из предыдущих экспериментов
        pattern_00 = sum(1 for j in range(len(window)-1) if window[j] == 0 and window[j+1] == 0)
        pattern_11 = sum(1 for j in range(len(window)-1) if window[j] == 1 and window[j+1] == 1)
        
        # Длины серий
        runs = []
        current_run = 1
        for j in range(1, len(window)):
            if window[j] == window[j-1]:
                current_run += 1
            else:
                runs.append(current_run)
                current_run = 1
        runs.append(current_run)
        
        max_run = max(runs) if runs else 0
        min_run = min(runs) if runs else 0
        
        # Экспоненциальное взвешивание
        weights = np.exp(np.linspace(-1, 0, lag))
        weights = weights / weights.sum()
        exp_weighted = np.sum(window * weights)
        
        # Плотность успехов в первой и второй половине
        half = lag // 2
        density_first = np.mean(window[:half]) if half > 0 else 0
        density_second = np.mean(window[half:]) if half < lag else 0
        density_trend = density_second - density_first
        
        features = [
            pattern_00 / (lag - 1),    # нормализованный паттерн 00
            pattern_11 / (lag - 1),    # нормализованный паттерн 11
            max_run / lag,              # максимальная серия
            min_run / lag,              # минимальная серия  
            exp_weighted,               # экспоненциальное среднее
            density_trend,              # тренд плотности
            np.sum(window) / lag,       # общая плотность
            np.std(window)              # стандартное отклонение
        ]
        
        X.append(features)
        y.append(sequence[i])
    
    return np.array(X), np.array(y)

X_best, y_best = create_best_features(sample, lag=4)
model_best = LogisticRegression(random_state=42, max_iter=2000, C=0.05)
model_best.fit(X_best, y_best)
auc_best = roc_auc_score(y_best, model_best.predict_proba(X_best)[:, 1])
bic_best, ll_best, k_best = calculate_bic(model_best, X_best, y_best)

# Вывод результатов
print(f"{'Модель':<35} {'AUC':<8} {'BIC':<12} {'Параметры':<10} {'BIC разница'}")
print("-" * 80)
print(f"{'Геометрическая (бенчмарк)':<35} {0.8056:.4f}    {43.33:.2f}      {2:<10} {'0.00 (baseline)'}")
print(f"{'Геометрически-усиленная':<35} {auc_geom_enhanced:.4f}    {bic_geom_enhanced:.2f}      {k_geom_enhanced:<10} {bic_geom_enhanced - 43.33:+.2f}")
print(f"{'Полиномиальные (deg 2)':<35} {auc_poly:.4f}    {bic_poly:.2f}      {k_poly:<10} {bic_poly - 43.33:+.2f}")
print(f"{'Взаимодействия':<35} {auc_interact:.4f}    {bic_interact:.2f}      {k_interact:<10} {bic_interact - 43.33:+.2f}")
print(f"{'Отобранные вручную':<35} {auc_best:.4f}    {bic_best:.2f}      {k_best:<10} {bic_best - 43.33:+.2f}")

# Анализ лучшей альтернативы
print(f"\n=== Лучшая альтернатива геометрической модели ===")
candidates = [
    (auc_geom_enhanced, bic_geom_enhanced, k_geom_enhanced, "Геометрически-усиленная"),
    (auc_poly, bic_poly, k_poly, "Полиномиальная"),
    (auc_interact, bic_interact, k_interact, "Взаимодействия"),
    (auc_best, bic_best, k_best, "Отобранная вручную")
]

best_candidate = min(candidates, key=lambda x: x[1])  # min BIC

if best_candidate[1] < 43.33:
    print(f"✅ {best_candidate[3]} ПОБЕДИЛА геометрическую!")
    print(f"   AUC: {best_candidate[0]:.4f} vs 0.8056")
    print(f"   BIC: {best_candidate[1]:.2f} vs 43.33")
    print(f"   Параметров: {best_candidate[2]}")
else:
    print(f"❌ Геометрическая модель остаётся лучшей")
    print(f"   Ближайший преследователь: {best_candidate[3]}")
    print(f"   BIC разница: {best_candidate[1] - 43.33:.2f}")

# Предсказание следующего значения для лучшей модели
def predict_next_best(sequence, model, lag=4):
    window = sequence[-lag:]
    
    pattern_00 = sum(1 for j in range(len(window)-1) if window[j] == 0 and window[j+1] == 0)
    pattern_11 = sum(1 for j in range(len(window)-1) if window[j] == 1 and window[j+1] == 1)
    
    runs = []
    current_run = 1
    for j in range(1, len(window)):
        if window[j] == window[j-1]:
            current_run += 1
        else:
            runs.append(current_run)
            current_run = 1
    runs.append(current_run)
    
    max_run = max(runs) if runs else 0
    min_run = min(runs) if runs else 0
    
    weights = np.exp(np.linspace(-1, 0, lag))
    weights = weights / weights.sum()
    exp_weighted = np.sum(window * weights)
    
    half = lag // 2
    density_first = np.mean(window[:half]) if half > 0 else 0
    density_second = np.mean(window[half:]) if half < lag else 0
    density_trend = density_second - density_first
    
    features = [
        pattern_00 / (lag - 1),
        pattern_11 / (lag - 1),
        max_run / lag,
        min_run / lag,
        exp_weighted,
        density_trend,
        np.sum(window) / lag,
        np.std(window)
    ]
    
    proba = model.predict_proba([features])[0][1]
    return proba

next_proba_best = predict_next_best(sample, model_best, lag=4)
print(f"\n=== Предсказание лучшей альтернативной модели ===")
print(f"Вероятность следующего значения = 1: {next_proba_best:.4f}")
print(f"Прогноз: {'УСПЕХ (1)' if next_proba_best >= 0.5 else 'НЕУДАЧА (0)'}")

# Финальный вердикт
print(f"\n=== Финальный вердикт ===")
if best_candidate[1] < 43.33 * 0.95:  # на 5% лучше
    print("🎉 УРА! Найдена модель, которая заметно лучше геометрической!")
    print(f"   Рекомендуется использовать {best_candidate[3]}")
elif best_candidate[1] < 43.33:
    print("📊 Найдена модель, немного лучше геометрической, но разница незначительна")
    print("   Рекомендуется оставить геометрическую модель (проще и надёжнее)")
else:
    print("🏆 Геометрическая модель — абсолютный чемпион!")
    print("   Примите это и радуйтесь простому и эффективному решению")