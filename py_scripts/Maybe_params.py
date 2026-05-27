import numpy as np
import pandas as pd
from sklearn.linear_model import LogisticRegression
from sklearn.preprocessing import StandardScaler
from sklearn.metrics import log_loss
import warnings
warnings.filterwarnings('ignore')

# Исходные данные
data = np.array([0,1,1,0,1,0,0,0,0,1,1,0,0,1,1,1,0,0,1,1,1,1,1,0,0,1,0,1,0,1,1,1,0,0,0,0,1])
print(f"Исходная выборка (n={len(data)}):\n{data}\n")

# Целевая переменная: предсказываем следующий элемент
X_raw = []
y = []

window_size = 15  # для первых признаков достаточно истории

for i in range(window_size, len(data) - 1):
    X_raw.append(data[i-window_size:i])  # окно истории
    y.append(data[i+1])  # следующий класс

X_raw = np.array(X_raw)
y = np.array(y)

print(f"Размер матрицы признаков (до генерации): {X_raw.shape}")
print(f"Размер целевой переменной: {y.shape}\n")

# === ГЕНЕРАЦИЯ ПРИЗНАКОВ ===
def generate_features(series_window):
    """
    series_window: одномерный массив бинарных значений (окно истории)
    Возвращает словарь с признаками
    """
    features = {}
    
    # Базовые статистики
    features['mean'] = np.mean(series_window)
    features['std'] = np.std(series_window)
    features['var'] = np.var(series_window)
    features['sum'] = np.sum(series_window)
    features['min'] = np.min(series_window)
    features['max'] = np.max(series_window)
    features['median'] = np.median(series_window)
    features['range'] = features['max'] - features['min']
    
    # Лаги (значения на разных позициях в окне)
    for lag in [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]:
        features[f'lag_{lag}'] = series_window[-lag] if lag <= len(series_window) else 0
    
    # Разности лагов
    for lag in [1, 2, 3]:
        if lag < len(series_window):
            features[f'diff_lag_{lag}'] = series_window[-1] - series_window[-1-lag]
    
    # Скользящие средние разных порядков
    for window in [2, 3, 4, 5, 7, 10]:
        if window <= len(series_window):
            features[f'ma_{window}'] = np.mean(series_window[-window:])
    
    # Скользящие стандартные отклонения
    for window in [2, 3, 4, 5, 7]:
        if window <= len(series_window):
            features[f'rolling_std_{window}'] = np.std(series_window[-window:])
    
    # Взвешенные средние (экспоненциальные)
    weights = np.exp(np.linspace(-1, 0, len(series_window)))
    weights /= weights.sum()
    features['exp_weighted_mean'] = np.sum(series_window * weights)
    
    # Асимметрия (skewness) и куртозис (kurtosis)
    if len(series_window) >= 4:
        centered = series_window - features['mean']
        n = len(series_window)
        m2 = np.sum(centered**2) / n
        m3 = np.sum(centered**3) / n
        m4 = np.sum(centered**4) / n
        
        features['skewness'] = m3 / (m2**1.5) if m2 > 0 else 0
        features['kurtosis'] = m4 / (m2**2) - 3 if m2 > 0 else 0
    
    # Количество переходов 0->1 и 1->0
    transitions_01 = np.sum((series_window[:-1] == 0) & (series_window[1:] == 1))
    transitions_10 = np.sum((series_window[:-1] == 1) & (series_window[1:] == 0))
    features['transitions_01'] = transitions_01
    features['transitions_10'] = transitions_10
    features['transition_ratio'] = transitions_01 / (transitions_10 + 1e-5)
    
    # Длина последней серии (одинаковых значений)
    last_val = series_window[-1]
    streak = 0
    for val in reversed(series_window):
        if val == last_val:
            streak += 1
        else:
            break
    features['last_streak'] = streak
    
    # Количество единиц в последних 3,5,7 элементах
    for k in [3, 5, 7]:
        if k <= len(series_window):
            features[f'ones_last_{k}'] = np.sum(series_window[-k:])
    
    # Вероятность (частота) единиц в разных частях окна
    split1 = len(series_window) // 3
    split2 = 2 * len(series_window) // 3
    features['freq_first_third'] = np.mean(series_window[:split1])
    features['freq_mid_third'] = np.mean(series_window[split1:split2])
    features['freq_last_third'] = np.mean(series_window[split2:])
    
    # Разница частот между частями
    features['freq_diff_last_first'] = features['freq_last_third'] - features['freq_first_third']
    
    # Энтропия (бинарная)
    p1 = features['mean']
    p0 = 1 - p1
    eps = 1e-10
    features['entropy'] = -(p0 * np.log(p0 + eps) + p1 * np.log(p1 + eps))
    
    # Количество повторяющихся паттернов (простые биграммы)
    bigrams = [tuple(series_window[i:i+2]) for i in range(len(series_window)-1)]
    unique_bigrams = len(set(bigrams))
    features['unique_bigrams_ratio'] = unique_bigrams / max(1, len(bigrams))
    
    return features

# Создаем DataFrame со всеми признаками
feature_list = []
for window in X_raw:
    feature_list.append(generate_features(window))

df_features = pd.DataFrame(feature_list)
print(f"Сгенерировано признаков: {df_features.shape[1]}")
print("Список признаков:")
for i, col in enumerate(df_features.columns, 1):
    print(f"{i:3d}. {col}")

# === ЛОГИСТИЧЕСКАЯ РЕГРЕССИЯ ===
# Стандартизация
scaler = StandardScaler()
X_scaled = scaler.fit_transform(df_features)

# Обучение модели (L2-регуляризация по умолчанию)
model = LogisticRegression(max_iter=1000, random_state=42, C=1e6)  # слабая регуляризация
model.fit(X_scaled, y)

# Предсказания
y_pred_prob = model.predict_proba(X_scaled)[:, 1]
y_pred_class = model.predict(X_scaled)

# === ВЫЧИСЛЕНИЕ BIC ===
n = len(y)  # количество наблюдений
k = X_scaled.shape[1]  # количество признаков
log_likelihood = -log_loss(y, y_pred_prob, normalize=False)  # логарифм правдоподобия
bic = k * np.log(n) - 2 * log_likelihood

# === ВЫВОД РЕЗУЛЬТАТОВ ===
print("\n" + "="*80)
print("РЕЗУЛЬТАТЫ ЛОГИСТИЧЕСКОЙ РЕГРЕССИИ")
print("="*80)

print(f"\nКоличество наблюдений: {n}")
print(f"Количество признаков: {k}")
print(f"Log-likelihood: {log_likelihood:.4f}")
print(f"BIC (Bayesian Information Criterion): {bic:.4f}")

print("\n--- ВЕСА МОДЕЛИ (коэффициенты) ---")
weights_df = pd.DataFrame({
    'Признак': df_features.columns,
    'Вес': model.coef_[0],
    'Абс.вес': np.abs(model.coef_[0])
}).sort_values('Абс.вес', ascending=False)

for idx, row in weights_df.iterrows():
    print(f"{row['Признак']:25s} : {row['Вес']:+8.4f}")

print("\n--- МЕТРИКИ КАЧЕСТВА ---")
accuracy = np.mean(y_pred_class == y)
precision = np.sum((y_pred_class == 1) & (y == 1)) / max(1, np.sum(y_pred_class == 1))
recall = np.sum((y_pred_class == 1) & (y == 1)) / max(1, np.sum(y == 1))
f1 = 2 * precision * recall / max(1e-10, precision + recall)

print(f"Accuracy:  {accuracy:.4f}")
print(f"Precision: {precision:.4f}")
print(f"Recall:    {recall:.4f}")
print(f"F1-score:  {f1:.4f}")

print("\n--- САМЫЕ ВЛИЯТЕЛЬНЫЕ ПРИЗНАКИ (топ-10 по модулю веса) ---")
top10 = weights_df.head(10)
for idx, row in top10.iterrows():
    print(f"{row['Признак']:25s} : {row['Вес']:+8.4f}")