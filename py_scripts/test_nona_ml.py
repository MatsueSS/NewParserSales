from catboost import CatBoostClassifier
import math

# Загружаем уже обученную модель
model = CatBoostClassifier()
model.load_model("models_v2/catboost_classifier.cbm")

# 24 признака.
# Здесь пока поставь свои реальные значения.
features = [
    100.0,   # 0  price
    -5.0,    # 1  price_change_1
    -0.0476, # 2  price_change_pct_1
    102.0,   # 3  price_mean_4
    3.0,     # 4  price_std_4
    98.0,    # 5  price_min_4
    106.0,   # 6  price_max_4
    104.0,   # 7  price_mean_8
    4.0,     # 8  price_std_8
    105.0,   # 9  price_mean_12
    5.0,     # 10 price_std_12
    1.0,     # 11 discount_prev_week
    2.0,     # 12 weeks_since_last_discount
    2.0,     # 13 discount_count_last_4
    3.0,     # 14 discount_count_last_8
    5.0,     # 15 discount_count_last_12
    0.5,     # 16 discount_frequency_4
    0.375,   # 17 discount_frequency_8
    0.4167,  # 18 discount_frequency_12
    3.0,     # 19 last_discount_interval
    15.0,    # 20 discount_prev_value
    38.0,    # 21 week_of_year
    9.0,     # 22 month
    3.0      # 23 quarter
]

# Проверяем количество признаков
print("Number of features:", len(features))

# Raw prediction
raw_prediction = model.predict(
    [features],
    prediction_type="RawFormulaVal"
)[0]

# Probability
probability = 1.0 / (1.0 + math.exp(-raw_prediction))

# Класс при стандартном пороге 0.5
prediction = 1 if probability >= 0.5 else 0

print("Raw prediction:", raw_prediction)
print("Probability:", probability)
print("Class:", prediction)