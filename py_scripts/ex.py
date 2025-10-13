from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC
import time
import sys

urls = [
    "https://5ka.ru/search/?text=choco",
    "https://5ka.ru/search/?text=%D1%81%D0%BF%D0%B0%D0%B3%D0%B5%D1%82%D1%82%D0%B8",
    "https://5ka.ru/search/?text=%D1%80%D0%B8%D1%81",
    "https://5ka.ru/search/?text=%D1%8F%D0%B1%D0%BB%D0%BE%D0%BA%D0%BE",
    "https://5ka.ru/search/?text=%D0%B1%D0%B0%D0%BD%D0%B0%D0%BD%D1%8B",
    "https://5ka.ru/search/?text=%D1%87%D0%B0%D0%B9",
    "https://5ka.ru/search/?text=%D0%A1%D1%82%D0%B8%D1%80%D0%B0%D0%BB%D1%8C%D0%BD%D1%8B%D0%B9%20%D0%BF%D0%BE%D1%80%D0%BE%D1%88%D0%BE%D0%BA",
    "https://5ka.ru/search/?text=%D0%BF%D1%8E%D1%80%D0%B5",
    "https://5ka.ru/search/?text=%D0%BA%D1%83%D0%BA%D1%83%D1%80%D1%83%D0%B7%D0%B0",
    "https://5ka.ru/search/?text=%D0%BA%D1%80%D0%B5%D0%BA%D0%B5%D1%80",
    "https://5ka.ru/search/?text=%D0%BC%D0%BE%D0%BB%D0%BE%D0%BA%D0%BE",
    "https://5ka.ru/search/?text=%D0%B3%D0%BE%D1%80%D0%BE%D1%88%D0%B5%D0%BA",
    "https://5ka.ru/search/?text=%D0%BC%D1%8B%D1%81%D0%BB%D0%B8",
    "https://5ka.ru/search/?text=%D1%81%D1%80%D0%B5%D0%B4%D1%81%D1%82%D0%B2%D0%BE%20%D0%B4%D0%BB%D1%8F%20%D0%BC%D1%8B%D1%82%D1%8C%D1%8F%20%D0%BF%D0%BE%D1%81%D1%83%D0%B4%D1%8B",
    "https://5ka.ru/search/?text=%D1%81%D0%BC%D0%B5%D1%81%D1%8C",
    "https://5ka.ru/search/?text=%D1%81%D1%8B%D1%80",
    "https://5ka.ru/search/?text=%D0%99%D0%BE%D0%B3%D1%83%D1%80%D1%82",
    "https://5ka.ru/search/?text=%D0%B2%D0%B0%D1%80%D0%B5%D0%BD%D0%B8%D0%BA%D0%B8",
    "https://5ka.ru/search/?text=%D1%82%D0%B2%D0%BE%D1%80%D0%BE%D0%B3",
    "https://5ka.ru/search/?text=%D0%BF%D0%B5%D1%87%D0%B5%D0%BD%D1%8C%D0%B5",
    "https://5ka.ru/search/?text=%D1%85%D0%BB%D0%BE%D0%BF%D1%8C%D1%8F",
    "https://5ka.ru/search/?text=%D1%8F%D0%B9%D1%86%D0%B0",
    "https://5ka.ru/search/?text=%D0%B7%D1%83%D0%B1%D0%BD%D0%B0%D1%8F%20%D0%BF%D0%B0%D1%81%D1%82%D0%B0",
    "http://5ka.ru/search/?text=%D0%BE%D0%B3%D1%83%D1%80%D1%86%D1%8B",
    "https://5ka.ru/search/?text=%D1%82%D1%83%D1%88%D1%91%D0%BD%D0%BA%D0%B0"
]

options = webdriver.ChromeOptions()
# options.add_argument("--headless")
driver = webdriver.Chrome(options=options)

def scroll_to_bottom(driver, pause_time=1, max_wait=20):
    """
    Прокручивает страницу до конца, пока подгружаются новые элементы.
    Ждёт появления новых карточек товаров.
    """
    last_height = driver.execute_script("return document.body.scrollHeight")
    start_time = time.time()

    while True:
        driver.execute_script("window.scrollTo(0, document.body.scrollHeight);")
        time.sleep(pause_time)

        # ждём появления новых карточек (ожидаем, что их станет больше)
        try:
            WebDriverWait(driver, 10).until(
                lambda d: d.execute_script("return document.body.scrollHeight") > last_height
            )
            last_height = driver.execute_script("return document.body.scrollHeight")
            start_time = time.time()  # сбрасываем таймер ожидания
        except:
            # если долго нет изменений — выходим
            if time.time() - start_time > max_wait:
                print("[!] Достигнут конец страницы.")
                break

for i, url in enumerate(urls, start=1):
    print(f"[→] Открываю: {url}")
    driver.get(url)

    # ждём начальной загрузки карточек
    try:
        WebDriverWait(driver, 30).until(
            EC.presence_of_element_located((By.CSS_SELECTOR, ".css-0"))
        )
    except Exception as e:
        print(f"[!] Не удалось дождаться карточек на {url}: {e}")
        continue

    # плавная прокрутка
    scroll_to_bottom(driver, pause_time=1, max_wait=20)

    # сохраняем HTML
    html = driver.page_source
    filename = f"../urls/page_{i}.html"
    with open(filename, "w", encoding="utf-8") as f:
        f.write(html)

    print(f"[+] Сохранена страница {url} → {filename}")

driver.quit()
sys.exit(0)

