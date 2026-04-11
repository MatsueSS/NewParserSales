import numpy as np
import pandas as pd
from sklearn.linear_model import LogisticRegression

# Ваши данные
data = [0,0,0,0,1,1,0,1,0,0,0,0,1,1,0,0,1,1,1,0,0,1,1,1,1,1,0,0,1,0,1]

# Создаём DataFrame
df = pd.DataFrame({'discount': data})

# Создаём признаки
df['week'] = range(1, len(data)+1)
for lag in range(1, 5):
    df[f'lag_{lag}'] = df['discount'].shift(lag)
df['rolling_mean_3'] = df['discount'].rolling(3).mean().shift(1)
df['rolling_sum_4'] = df['discount'].rolling(4).sum().shift(1)

df = df.dropna()

# Признаки и цель
feature_cols = ['week', 'lag_1', 'lag_2', 'lag_3', 'lag_4', 'rolling_mean_3', 'rolling_sum_4']
X = df[feature_cols]
y = df['discount']

# Обучаем модель
model = LogisticRegression(max_iter=1000)
model.fit(X, y)

# Прогноз для недели 32
new_data = pd.DataFrame([{
    'week': 32,
    'lag_1': data[-1],      # последнее значение = 1
    'lag_2': data[-2],      # предпоследнее = 0
    'lag_3': data[-3],      # = 1
    'lag_4': data[-4],      # = 0
    'rolling_mean_3': np.mean(data[-3:]),  # среднее за 29,30,31
    'rolling_sum_4': sum(data[-4:])        # сумма за 28,29,30,31
}])

probability = model.predict_proba(new_data)[0, 1]
print(f'Вероятность скидки на неделе 32: {probability:.3f} ({probability*100:.1f}%)')

# Интерпретация
if probability > 0.7:
    print("→ Высокая вероятность скидки")
elif probability > 0.3:
    print("→ Средняя вероятность скидки")
else:
    print("→ Низкая вероятность скидки")