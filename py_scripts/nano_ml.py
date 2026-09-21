import os
import warnings

import numpy as np
import pandas as pd

from sqlalchemy import create_engine

from sklearn.metrics import (
    roc_auc_score,
    average_precision_score,
    accuracy_score,
    precision_score,
    recall_score,
    f1_score,
    mean_absolute_error,
    mean_squared_error,
)

from catboost import (
    CatBoostClassifier,
    CatBoostRegressor,
)


warnings.filterwarnings("ignore")


# ============================================================
# CONFIGURATION
# ============================================================

DB_HOST = os.getenv("DB_HOST", "localhost")
DB_PORT = os.getenv("DB_PORT", "5432")
DB_NAME = os.getenv("DB_NAME", "remstocks")
DB_USER = os.getenv("DB_USER", "postgres")
DB_PASSWORD = os.getenv("DB_PASSWORD", "731177889232")

RESULTS_DIR = "results_v2"
MODELS_DIR = "models_v2"

os.makedirs(RESULTS_DIR, exist_ok=True)
os.makedirs(MODELS_DIR, exist_ok=True)

RANDOM_STATE = 42


# ============================================================
# DATABASE
# ============================================================

def get_engine():

    connection_string = (
        f"postgresql+psycopg2://"
        f"{DB_USER}:{DB_PASSWORD}"
        f"@{DB_HOST}:{DB_PORT}/{DB_NAME}"
    )

    return create_engine(connection_string)


def load_data():

    print("Загрузка данных из PostgreSQL...")

    engine = get_engine()

    query = """
        SELECT
            c.price,
            p.id AS product_id,
            c.discount,
            c.date
        FROM cards c
        JOIN products p
            ON p.title = c.title
        ORDER BY
            p.id,
            c.date
    """

    df = pd.read_sql(query, engine)

    df["date"] = pd.to_datetime(df["date"])

    df["price"] = pd.to_numeric(
        df["price"],
        errors="coerce"
    )

    df["discount"] = pd.to_numeric(
        df["discount"],
        errors="coerce"
    )

    df = df.dropna(
        subset=[
            "product_id",
            "date",
            "price",
        ]
    )

    df = (
        df
        .sort_values(
            ["product_id", "date"]
        )
        .drop_duplicates(
            subset=[
                "product_id",
                "date",
            ],
            keep="last",
        )
        .reset_index(drop=True)
    )

    print(
        "Количество строк:",
        len(df)
    )

    print(
        "Количество товаров:",
        df["product_id"].nunique()
    )

    print(
        "Диапазон дат:",
        df["date"].min(),
        "->",
        df["date"].max()
    )

    return df


# ============================================================
# FEATURE ENGINEERING
# ============================================================

def create_features(df):

    print("\nСоздание признаков...")

    df = (
        df
        .sort_values(
            ["product_id", "date"]
        )
        .copy()
    )

    # --------------------------------------------------------
    # HAS DISCOUNT
    # --------------------------------------------------------

    df["has_discount"] = (
        df["discount"]
        .notna()
        .astype(int)
    )

    # --------------------------------------------------------
    # CALENDAR FEATURES
    # --------------------------------------------------------

    df["week_of_year"] = (
        df["date"]
        .dt
        .isocalendar()
        .week
        .astype(int)
    )

    df["month"] = (
        df["date"].dt.month
    )

    df["quarter"] = (
        df["date"].dt.quarter
    )

    # --------------------------------------------------------
    # PRICE FEATURES
    # --------------------------------------------------------

    df["price_prev_1"] = (
        df
        .groupby("product_id")["price"]
        .shift(1)
    )

    df["price_change_1"] = (
        df["price"]
        - df["price_prev_1"]
    )

    df["price_change_pct_1"] = (
        df["price_change_1"]
        /
        df["price_prev_1"]
        .replace(0, np.nan)
    )

    # --------------------------------------------------------
    # ROLLING PRICE FEATURES
    # --------------------------------------------------------

    for window in [4, 8, 12]:

        grouped_price = (
            df
            .groupby("product_id")["price"]
        )

        df[f"price_mean_{window}"] = (
            grouped_price
            .transform(
                lambda x:
                x.shift(1)
                .rolling(
                    window=window,
                    min_periods=1,
                )
                .mean()
            )
        )

        df[f"price_std_{window}"] = (
            grouped_price
            .transform(
                lambda x:
                x.shift(1)
                .rolling(
                    window=window,
                    min_periods=2,
                )
                .std()
            )
        )

        df[f"price_min_{window}"] = (
            grouped_price
            .transform(
                lambda x:
                x.shift(1)
                .rolling(
                    window=window,
                    min_periods=1,
                )
                .min()
            )
        )

        df[f"price_max_{window}"] = (
            grouped_price
            .transform(
                lambda x:
                x.shift(1)
                .rolling(
                    window=window,
                    min_periods=1,
                )
                .max()
            )
        )

    # --------------------------------------------------------
    # DISCOUNT HISTORY
    # --------------------------------------------------------

    df["discount_prev_week"] = (
        df
        .groupby("product_id")["has_discount"]
        .shift(1)
    )

    for window in [4, 8, 12]:

        df[
            f"discount_count_last_{window}"
        ] = (
            df
            .groupby("product_id")["has_discount"]
            .transform(
                lambda x:
                x.shift(1)
                .rolling(
                    window=window,
                    min_periods=1,
                )
                .sum()
            )
        )

        df[
            f"discount_frequency_{window}"
        ] = (
            df[
                f"discount_count_last_{window}"
            ]
            / window
        )

    # --------------------------------------------------------
    # TIME SINCE LAST DISCOUNT
    # --------------------------------------------------------

    df["discount_event_date"] = (
        df["date"]
        .where(
            df["has_discount"] == 1
        )
    )

    df["last_discount_date"] = (
        df
        .groupby("product_id")[
            "discount_event_date"
        ]
        .ffill()
    )

    df["previous_discount_date"] = (
        df
        .groupby("product_id")[
            "last_discount_date"
        ]
        .shift(1)
    )

    df["weeks_since_last_discount"] = (
        df["date"]
        - df["previous_discount_date"]
    ).dt.days / 7

    # --------------------------------------------------------
    # INTERVAL BETWEEN DISCOUNTS
    # --------------------------------------------------------

    previous_event = (
        df
        .groupby("product_id")[
            "discount_event_date"
        ]
        .shift(1)
    )

    df["discount_interval"] = (
        df["discount_event_date"]
        - previous_event
    ).dt.days / 7

    df["last_discount_interval"] = (
        df
        .groupby("product_id")[
            "discount_interval"
        ]
        .ffill()
    )

    # --------------------------------------------------------
    # PREVIOUS DISCOUNT VALUE
    # --------------------------------------------------------

    df["discount_prev_value"] = (
        df
        .groupby("product_id")["discount"]
        .shift(1)
    )

    # --------------------------------------------------------
    # TARGET: NEXT WEEK DISCOUNT
    # --------------------------------------------------------

    df["target_discount_next_week"] = (
        df
        .groupby("product_id")[
            "has_discount"
        ]
        .shift(-1)
    )

    # --------------------------------------------------------
    # TARGET: NEXT WEEK DISCOUNT VALUE
    # --------------------------------------------------------

    df[
        "target_discount_value_next_week"
    ] = (
        df
        .groupby("product_id")["discount"]
        .shift(-1)
    )

    return df


# ============================================================
# FEATURE LIST
# ============================================================

def get_features():

    return [
        "price",

        "price_change_1",
        "price_change_pct_1",

        "price_mean_4",
        "price_std_4",
        "price_min_4",
        "price_max_4",

        "price_mean_8",
        "price_std_8",

        "price_mean_12",
        "price_std_12",

        "discount_prev_week",

        "weeks_since_last_discount",

        "discount_count_last_4",
        "discount_count_last_8",
        "discount_count_last_12",

        "discount_frequency_4",
        "discount_frequency_8",
        "discount_frequency_12",

        "last_discount_interval",

        "discount_prev_value",

        "week_of_year",
        "month",
        "quarter",
    ]


# ============================================================
# CLASSIFICATION METRICS
# ============================================================

def classification_metrics(
    y_true,
    probabilities,
    threshold=0.5,
):

    predictions = (
        probabilities >= threshold
    ).astype(int)

    return {
        "roc_auc": roc_auc_score(
            y_true,
            probabilities,
        ),

        "pr_auc": average_precision_score(
            y_true,
            probabilities,
        ),

        "accuracy": accuracy_score(
            y_true,
            predictions,
        ),

        "precision": precision_score(
            y_true,
            predictions,
            zero_division=0,
        ),

        "recall": recall_score(
            y_true,
            predictions,
            zero_division=0,
        ),

        "f1": f1_score(
            y_true,
            predictions,
            zero_division=0,
        ),
    }


# ============================================================
# THRESHOLD OPTIMIZATION
# ============================================================

def find_best_threshold(
    y_true,
    probabilities,
):

    rows = []

    thresholds = np.arange(
        0.05,
        0.96,
        0.01,
    )

    for threshold in thresholds:

        predictions = (
            probabilities >= threshold
        ).astype(int)

        precision = precision_score(
            y_true,
            predictions,
            zero_division=0,
        )

        recall = recall_score(
            y_true,
            predictions,
            zero_division=0,
        )

        f1 = f1_score(
            y_true,
            predictions,
            zero_division=0,
        )

        rows.append({
            "threshold": threshold,
            "precision": precision,
            "recall": recall,
            "f1": f1,
        })

    threshold_df = pd.DataFrame(rows)

    best_row = (
        threshold_df
        .sort_values(
            "f1",
            ascending=False,
        )
        .iloc[0]
    )

    return (
        float(best_row["threshold"]),
        threshold_df,
    )


# ============================================================
# CATBOOST CLASSIFIER
# ============================================================

def train_classifier(
    train,
    test,
    features,
):

    print(
        "\n" + "=" * 60
    )

    print(
        "CATBOOST CLASSIFIER"
    )

    print(
        "=" * 60
    )

    train_classification = (
        train
        .dropna(
            subset=[
                "target_discount_next_week"
            ]
        )
        .copy()
    )

    test_classification = (
        test
        .dropna(
            subset=[
                "target_discount_next_week"
            ]
        )
        .copy()
    )

    X_train = (
        train_classification[
            features
        ]
        .replace(
            [np.inf, -np.inf],
            np.nan,
        )
        .fillna(-999)
    )

    y_train = (
        train_classification[
            "target_discount_next_week"
        ]
        .astype(int)
    )

    X_test = (
        test_classification[
            features
        ]
        .replace(
            [np.inf, -np.inf],
            np.nan,
        )
        .fillna(-999)
    )

    y_test = (
        test_classification[
            "target_discount_next_week"
        ]
        .astype(int)
    )

    print(
        "Train:",
        len(X_train)
    )

    print(
        "Test:",
        len(X_test)
    )

    print(
        "Features:",
        len(features)
    )

    model = CatBoostClassifier(
        iterations=500,
        depth=8,
        learning_rate=0.05,
        loss_function="Logloss",
        eval_metric="AUC",
        random_seed=RANDOM_STATE,
        verbose=100,
        auto_class_weights="Balanced",
    )

    print(
        "\nОбучение CatBoostClassifier..."
    )

    model.fit(
        X_train,
        y_train,
        eval_set=(
            X_test,
            y_test,
        ),
        early_stopping_rounds=50,
    )

    probabilities = (
        model
        .predict_proba(X_test)[:, 1]
    )

    # --------------------------------------------------------
    # METRICS
    # --------------------------------------------------------

    metrics = classification_metrics(
        y_test,
        probabilities,
        threshold=0.5,
    )

    best_threshold, threshold_df = (
        find_best_threshold(
            y_test,
            probabilities,
        )
    )

    best_metrics = (
        classification_metrics(
            y_test,
            probabilities,
            threshold=best_threshold,
        )
    )

    print(
        "\n=== CLASSIFICATION RESULTS ==="
    )

    print(
        f"ROC-AUC: "
        f"{metrics['roc_auc']:.6f}"
    )

    print(
        f"PR-AUC: "
        f"{metrics['pr_auc']:.6f}"
    )

    print(
        f"Accuracy: "
        f"{metrics['accuracy']:.6f}"
    )

    print(
        f"Precision: "
        f"{metrics['precision']:.6f}"
    )

    print(
        f"Recall: "
        f"{metrics['recall']:.6f}"
    )

    print(
        f"F1: "
        f"{metrics['f1']:.6f}"
    )

    print(
        f"\nBest threshold: "
        f"{best_threshold:.2f}"
    )

    print(
        f"Best F1: "
        f"{best_metrics['f1']:.6f}"
    )

    # --------------------------------------------------------
    # SAVE MODEL
    # --------------------------------------------------------

    classifier_path = os.path.join(
        MODELS_DIR,
        "catboost_classifier.cbm",
    )

    model.save_model(
        classifier_path
    )

    print(
        "\nМодель сохранена:",
        classifier_path,
    )

    # --------------------------------------------------------
    # SAVE THRESHOLDS
    # --------------------------------------------------------

    threshold_path = os.path.join(
        RESULTS_DIR,
        "catboost_classifier_thresholds.csv",
    )

    threshold_df.to_csv(
        threshold_path,
        index=False,
    )

    # --------------------------------------------------------
    # SAVE METRICS
    # --------------------------------------------------------

    result = {
        "model":
            "CatBoostClassifier",

        "roc_auc":
            metrics["roc_auc"],

        "pr_auc":
            metrics["pr_auc"],

        "accuracy":
            metrics["accuracy"],

        "precision":
            metrics["precision"],

        "recall":
            metrics["recall"],

        "f1":
            metrics["f1"],

        "best_threshold":
            best_threshold,

        "precision_best_threshold":
            best_metrics["precision"],

        "recall_best_threshold":
            best_metrics["recall"],

        "f1_best_threshold":
            best_metrics["f1"],
    }

    pd.DataFrame(
        [result]
    ).to_csv(
        os.path.join(
            RESULTS_DIR,
            "catboost_classifier_results.csv",
        ),
        index=False,
    )

    # --------------------------------------------------------
    # FEATURE IMPORTANCE
    # --------------------------------------------------------

    importance = (
        model
        .get_feature_importance()
    )

    importance_df = pd.DataFrame({
        "feature": features,
        "importance": importance,
    }).sort_values(
        "importance",
        ascending=False,
    )

    print(
        "\n=== CATBOOST FEATURE IMPORTANCE ==="
    )

    print(
        importance_df
        .round(6)
    )

    importance_df.to_csv(
        os.path.join(
            RESULTS_DIR,
            "catboost_feature_importance.csv",
        ),
        index=False,
    )

    return model


# ============================================================
# CATBOOST REGRESSOR
# ============================================================

def train_regressor(
    train,
    test,
    features,
):

    print(
        "\n" + "=" * 60
    )

    print(
        "CATBOOST REGRESSOR"
    )

    print(
        "=" * 60
    )

    train_regression = (
        train[
            train[
                "target_discount_value_next_week"
            ].notna()
        ]
        .copy()
    )

    test_regression = (
        test[
            test[
                "target_discount_value_next_week"
            ].notna()
        ]
        .copy()
    )

    X_train = (
        train_regression[
            features
        ]
        .replace(
            [np.inf, -np.inf],
            np.nan,
        )
        .fillna(-999)
    )

    y_train = (
        train_regression[
            "target_discount_value_next_week"
        ]
    )

    X_test = (
        test_regression[
            features
        ]
        .replace(
            [np.inf, -np.inf],
            np.nan,
        )
        .fillna(-999)
    )

    y_test = (
        test_regression[
            "target_discount_value_next_week"
        ]
    )

    print(
        "Train:",
        len(X_train)
    )

    print(
        "Test:",
        len(X_test)
    )

    print(
        "Features:",
        len(features)
    )

    model = CatBoostRegressor(
        iterations=500,
        depth=8,
        learning_rate=0.05,
        loss_function="MAE",
        random_seed=RANDOM_STATE,
        verbose=100,
    )

    print(
        "\nОбучение CatBoostRegressor..."
    )

    model.fit(
        X_train,
        y_train,
        eval_set=(
            X_test,
            y_test,
        ),
        early_stopping_rounds=50,
    )

    predictions = model.predict(
        X_test
    )

    mae = mean_absolute_error(
        y_test,
        predictions,
    )

    rmse = np.sqrt(
        mean_squared_error(
            y_test,
            predictions,
        )
    )

    print(
        "\n=== REGRESSION RESULTS ==="
    )

    print(
        f"MAE:  {mae:.6f}"
    )

    print(
        f"RMSE: {rmse:.6f}"
    )

    # --------------------------------------------------------
    # SAVE MODEL
    # --------------------------------------------------------

    regressor_path = os.path.join(
        MODELS_DIR,
        "catboost_regressor.cbm",
    )

    model.save_model(
        regressor_path
    )

    print(
        "\nМодель сохранена:",
        regressor_path,
    )

    # --------------------------------------------------------
    # SAVE METRICS
    # --------------------------------------------------------

    pd.DataFrame([
        {
            "model":
                "CatBoostRegressor",

            "MAE":
                mae,

            "RMSE":
                rmse,
        }
    ]).to_csv(
        os.path.join(
            RESULTS_DIR,
            "catboost_regressor_results.csv",
        ),
        index=False,
    )

    return model


# ============================================================
# TEMPORAL TRAIN / TEST
# ============================================================

def train_test_experiment(
    df,
    features,
):

    print(
        "\n" + "=" * 60
    )

    print(
        "SINGLE TEMPORAL TRAIN / TEST EXPERIMENT"
    )

    print(
        "=" * 60
    )

    unique_dates = np.sort(
        df["date"].unique()
    )

    split_index = int(
        len(unique_dates) * 0.75
    )

    split_date = (
        unique_dates[split_index]
    )

    train = (
        df[
            df["date"] < split_date
        ]
        .copy()
    )

    test = (
        df[
            df["date"] >= split_date
        ]
        .copy()
    )

    print(
        "\nВременное разделение:"
    )

    print(
        "Train:",
        train["date"].min(),
        "->",
        train["date"].max()
    )

    print(
        "Test:",
        test["date"].min(),
        "->",
        test["date"].max()
    )

    print(
        "\nРазмер train:",
        len(train)
    )

    print(
        "Размер test:",
        len(test)
    )

    classifier = train_classifier(
        train,
        test,
        features,
    )

    regressor = train_regressor(
        train,
        test,
        features,
    )

    return classifier, regressor


# ============================================================
# MAIN
# ============================================================

def main():

    # --------------------------------------------------------
    # LOAD
    # --------------------------------------------------------

    df = load_data()

    # --------------------------------------------------------
    # FEATURES
    # --------------------------------------------------------

    df = create_features(df)

    features = get_features()

    # --------------------------------------------------------
    # REMOVE LAST OBSERVATION
    # --------------------------------------------------------

    df_model = (
        df
        .dropna(
            subset=[
                "target_discount_next_week"
            ]
        )
        .copy()
    )

    print(
        "\nКоличество строк после "
        "формирования target:",
        len(df_model),
    )

    print(
        "\nКоличество признаков:",
        len(features),
    )

    print(
        "\nПризнаки:"
    )

    for index, feature in enumerate(
        features
    ):
        print(
            f"  [{index}] {feature}"
        )

    # --------------------------------------------------------
    # TRAIN
    # --------------------------------------------------------

    train_test_experiment(
        df_model,
        features,
    )

    print(
        "\nОбучение завершено."
    )

    print(
        f"\nРезультаты сохранены в: "
        f"{RESULTS_DIR}"
    )

    print(
        f"Модели сохранены в: "
        f"{MODELS_DIR}"
    )


if __name__ == "__main__":
    main()