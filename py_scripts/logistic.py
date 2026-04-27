import numpy as np
from sklearn.linear_model import LogisticRegression
from sklearn.metrics import roc_auc_score

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

def create_selected_features(sequence, lag=4):
    """Создаёт только ТОП-6 самых важных признаков (коэффициент > 0.3)"""
    X, y = [], []
    
    for i in range(lag, len(sequence)):
        window = sequence[i-lag:i]
        
        # 1. pattern_00 (количество пар 00)
        pattern_00 = sum(1 for j in range(len(window)-1) if window[j] == 0 and window[j+1] == 0)
        
        # 2. min_run (минимальная длина серии)
        runs = []
        current_run = 1
        for j in range(1, len(window)):
            if window[j] == window[j-1]:
                current_run += 1
            else:
                runs.append(current_run)
                current_run = 1
        runs.append(current_run)
        min_run = min(runs) if runs else 0
        
        # 3. autocorr_2 (автокорреляция с лагом 2)
        autocorr_2 = 0
        if len(window) > 2 and np.std(window[:-2]) > 1e-8 and np.std(window[2:]) > 1e-8:
            corr_matrix = np.corrcoef(window[:-2], window[2:])
            if not np.isnan(corr_matrix[0, 1]):
                autocorr_2 = corr_matrix[0, 1]
        
        # 4. lag_2 (значение на позиции -2)
        lag_2 = window[-2] if len(window) >= 2 else 0
        
        # 5. lag_4 (значение на позиции -4)
        lag_4 = window[-4] if len(window) >= 4 else 0
        
        # 6. kurtosis (эксцесс)
        mean_val = np.mean(window)
        std_val = np.std(window)
        if len(window) > 2 and std_val > 1e-8:
            kurtosis = ((window - mean_val)**4).mean() / (std_val**4 + 1e-8) - 3
        else:
            kurtosis = 0
        
        # Собираем только выбранные признаки
        features = [pattern_00, min_run, autocorr_2, lag_2, lag_4, kurtosis]
        
        X.append(features)
        y.append(sequence[i])
    
    return np.array(X), np.array(y)

# Твои данные
sample = [0,0,0,0,1,1,0,1,0,0,0,0,1,1,0,0,1,1,1,0,0,1,1,1,1,1,0,0,1,0,1,0,1,1]

print("=== Сравнение моделей с разным количеством признаков ===\n")

# 1. Исходная супер-расширенная модель (35 признаков)
lag = 4
X_full, y_full = [], []
for i in range(lag, len(sample)):
    X_full.append(sample[i-lag:i])
    y_full.append(sample[i])
X_full = np.array(X_full)

# Добавляем признаки из v3 (здесь нужно использовать полную версию из предыдущего кода)
# Для простоты используем существующие X3, y3 из предыдущего запуска
# Если их нет, загрузим из прошлого расчета

# 2. Модель только с топ-6 признаками (коэффициент > 0.3)
X_selected, y_selected = create_selected_features(sample, lag=4)

# 3. Модель только с топ-3 признаками (самые сильные)
def create_top3_features(sequence, lag=4):
    """Только 3 самых важных признака"""
    X, y = [], []
    
    for i in range(lag, len(sequence)):
        window = sequence[i-lag:i]
        
        # pattern_00 (самый важный)
        pattern_00 = sum(1 for j in range(len(window)-1) if window[j] == 0 and window[j+1] == 0)
        
        # min_run (второй по важности)
        runs = []
        current_run = 1
        for j in range(1, len(window)):
            if window[j] == window[j-1]:
                current_run += 1
            else:
                runs.append(current_run)
                current_run = 1
        runs.append(current_run)
        min_run = min(runs) if runs else 0
        
        # autocorr_2 (третий по важности)
        autocorr_2 = 0
        if len(window) > 2 and np.std(window[:-2]) > 1e-8 and np.std(window[2:]) > 1e-8:
            corr_matrix = np.corrcoef(window[:-2], window[2:])
            if not np.isnan(corr_matrix[0, 1]):
                autocorr_2 = corr_matrix[0, 1]
        
        features = [pattern_00, min_run, autocorr_2]
        X.append(features)
        y.append(sequence[i])
    
    return np.array(X), np.array(y)

X_top3, y_top3 = create_top3_features(sample, lag=4)

# Обучаем модели
print("Обучаем модели...")

# Модель с топ-6 признаками
model_selected = LogisticRegression(random_state=42, max_iter=1000)
model_selected.fit(X_selected, y_selected)
auc_selected = roc_auc_score(y_selected, model_selected.predict_proba(X_selected)[:, 1])
bic_selected, ll_selected, k_selected = calculate_bic(model_selected, X_selected, y_selected)

# Модель с топ-3 признаками
model_top3 = LogisticRegression(random_state=42, max_iter=1000)
model_top3.fit(X_top3, y_top3)
auc_top3 = roc_auc_score(y_top3, model_top3.predict_proba(X_top3)[:, 1])
bic_top3, ll_top3, k_top3 = calculate_bic(model_top3, X_top3, y_top3)

# Модель с полными признаками (из предыдущего запуска)
# Используем сохраненные значения или пересчитываем
print("\n=== Результаты ===")
print(f"{'Модель':<35} {'AUC':<8} {'BIC':<12} {'Log-Likelihood':<16} {'Параметры':<10}")
print("-" * 85)

# Эти значения из твоего предыдущего запуска
print(f"{'Полная модель (35 признаков)':<35} {0.7917:.4f}    {147.47:.2f}      {'-14.41':<16} {36}")
print(f"{'Топ-6 признаков (>0.3)':<35} {auc_selected:.4f}    {bic_selected:.2f}      {ll_selected:.2f}           {k_selected}")
print(f"{'Топ-3 признака':<35} {auc_top3:.4f}    {bic_top3:.2f}      {ll_top3:.2f}           {k_top3}")

print(f"\n=== Детали топ-6 модели ===")
print(f"Использованные признаки: pattern_00, min_run, autocorr_2, lag_2, lag_4, kurtosis")
print(f"Коэффициенты модели:")
coeffs_selected = model_selected.coef_[0]
feature_names_selected = ['pattern_00', 'min_run', 'autocorr_2', 'lag_2', 'lag_4', 'kurtosis']
for name, coeff in zip(feature_names_selected, coeffs_selected):
    print(f"  {name:15s}: {coeff:+.4f}")

print(f"\n=== Детали топ-3 модели ===")
print(f"Использованные признаки: pattern_00, min_run, autocorr_2")
coeffs_top3 = model_top3.coef_[0]
feature_names_top3 = ['pattern_00', 'min_run', 'autocorr_2']
for name, coeff in zip(feature_names_top3, coeffs_top3):
    print(f"  {name:15s}: {coeff:+.4f}")

# Предсказание следующего значения для топ-6 модели
def predict_next_selected(sequence, model, lag=4):
    window = sequence[-lag:]
    
    # Вычисляем только нужные признаки
    pattern_00 = sum(1 for j in range(len(window)-1) if window[j] == 0 and window[j+1] == 0)
    
    runs = []
    current_run = 1
    for j in range(1, len(window)):
        if window[j] == window[j-1]:
            current_run += 1
        else:
            runs.append(current_run)
            current_run = 1
    runs.append(current_run)
    min_run = min(runs) if runs else 0
    
    autocorr_2 = 0
    if len(window) > 2 and np.std(window[:-2]) > 1e-8 and np.std(window[2:]) > 1e-8:
        corr_matrix = np.corrcoef(window[:-2], window[2:])
        if not np.isnan(corr_matrix[0, 1]):
            autocorr_2 = corr_matrix[0, 1]
    
    lag_2 = window[-2] if len(window) >= 2 else 0
    lag_4 = window[-4] if len(window) >= 4 else 0
    
    mean_val = np.mean(window)
    std_val = np.std(window)
    if len(window) > 2 and std_val > 1e-8:
        kurtosis = ((window - mean_val)**4).mean() / (std_val**4 + 1e-8) - 3
    else:
        kurtosis = 0
    
    features = [pattern_00, min_run, autocorr_2, lag_2, lag_4, kurtosis]
    proba = model.predict_proba([features])[0][1]
    return proba

next_proba_selected = predict_next_selected(sample, model_selected, lag=4)
print(f"\n=== Предсказание топ-6 модели ===")
print(f"Вероятность следующего значения = 1: {next_proba_selected:.4f}")
print(f"Прогноз: {'УСПЕХ (1)' if next_proba_selected >= 0.5 else 'НЕУДАЧА (0)'}")

# Сравнение с геометрической моделью
print(f"\n=== Сравнение с геометрической моделью ===")
print(f"Геометрическая модель:        AUC = 0.7813, BIC = 42.07, параметров = 2")
print(f"Топ-3 логистическая модель:   AUC = {auc_top3:.4f}, BIC = {bic_top3:.2f}, параметров = {k_top3}")
print(f"Топ-6 логистическая модель:   AUC = {auc_selected:.4f}, BIC = {bic_selected:.2f}, параметров = {k_selected}")

# Рассчитываем, насколько лучше геометрическая модель
if bic_top3 < 42.07:
    improvement = 42.07 - bic_top3
    print(f"\nТоп-3 модель лучше геометрической на {improvement:.2f} по BIC")
else:
    degradation = bic_top3 - 42.07
    print(f"\nГеометрическая модель лучше топ-3 на {degradation:.2f} по BIC")

# Визуализация сравнения
print("\n=== Рекомендация ===")
best_bic = min(42.07, bic_top3, bic_selected)
if best_bic == 42.07:
    print("✅ ЛУЧШАЯ: Геометрическая модель (BIC = 42.07)")
    print("   - Самый низкий BIC")
    print("   - Всего 2 параметра")
    print("   - Отличное обобщение на новых данных")
elif best_bic == bic_top3:
    print("✅ ЛУЧШАЯ: Топ-3 логистическая модель")
    print(f"   - BIC = {bic_top3:.2f}")
    print(f"   - {k_top3} параметров")
else:
    print("✅ ЛУЧШАЯ: Топ-6 логистическая модель")
    print(f"   - BIC = {bic_selected:.2f}")
    print(f"   - {k_selected} параметров")