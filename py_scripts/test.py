from selenium import webdriver
from selenium.webdriver.chrome.service import Service as ChromeService
from webdriver_manager.chrome import ChromeDriverManager
from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC
import time
import random
import sys

urls = [
    "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C13057/products?mode=delivery&include_restrict=true&limit=350&offset=0", #чай, кофе, какао
    "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C13055/products?mode=delivery&include_restrict=true&limit=250&offset=0", #макароны, крупы
    "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C12886/products?mode=delivery&include_restrict=true&limit=400&offset=0", #овощи, фрукты, орехи
    "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C13093/products?mode=delivery&include_restrict=true&limit=400&offset=0", #молоко, сметана
    "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C13094/products?mode=delivery&include_restrict=true&limit=400&offset=0", #кефир, творог
    "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C13095/products?mode=delivery&include_restrict=true&limit=400&offset=0", #сыр
    "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C13096/products?mode=delivery&include_restrict=true&limit=400&offset=0", #яйцо, масло
    "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C13097/products?mode=delivery&include_restrict=true&limit=400&offset=0", #майонез
    "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C13098/products?mode=delivery&include_restrict=true&limit=400&offset=0", #йогурты, десерты
    "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C12888/products?mode=delivery&include_restrict=true&limit=400&offset=0", #хлеб и выпечка
    "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C12889/products?mode=delivery&include_restrict=true&limit=400&offset=0", #мясо, птица, колбасы
    "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C12890/products?mode=delivery&include_restrict=true&limit=400&offset=0", #рыба, морепродукты
    "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C13070/products?mode=delivery&include_restrict=true&limit=400&offset=0", #шоколад, конфеты
    "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C13071/products?mode=delivery&include_restrict=true&limit=400&offset=0", #печенье, вафли, пряники
    "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C13072/products?mode=delivery&include_restrict=true&limit=400&offset=0", #зефир, мармелад, халва
    "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C13073/products?mode=delivery&include_restrict=true&limit=400&offset=0", #торты, пирожные 
    "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C13074/products?mode=delivery&include_restrict=true&limit=400&offset=0", #мороженое
    "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C13075/products?mode=delivery&include_restrict=true&limit=400&offset=0", #жвачка, леденцы, драже
    "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C13076/products?mode=delivery&include_restrict=true&limit=400&offset=0", #варенье, мёд, пасты
    "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C12901/products?mode=delivery&include_restrict=true&limit=400&offset=0", #снеки, чипсы
    "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C12903/products?mode=delivery&include_restrict=true&limit=400&offset=0", #замороженные продукты
    "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C12904/products?mode=delivery&include_restrict=true&limit=499&offset=0", #вода и напитки
    "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C12905/products?mode=delivery&include_restrict=true&limit=400&offset=0", #Здоровый выбор
    "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C12906/products?mode=delivery&include_restrict=true&limit=499&offset=0", #для детей
    "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C12907/products?mode=delivery&include_restrict=true&limit=400&offset=0", #для животных
    "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C12908/products?mode=delivery&include_restrict=true&limit=499&offset=0", #красота, гигиена, аптекс
    "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C12909/products?mode=delivery&include_restrict=true&limit=400&offset=0", #стирка, уборка
    "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C13056/products?mode=delivery&include_restrict=true&limit=400&offset=0", #сухие завтраки, каши
    "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C13058/products?mode=delivery&include_restrict=true&limit=400&offset=0", #сахар соль специи
    "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C13059/products?mode=delivery&include_restrict=true&limit=400&offset=0", #масло, уксус
    "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C13060/products?mode=delivery&include_restrict=true&limit=400&offset=0", #Майонез, соусы
    "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C13061/products?mode=delivery&include_restrict=true&limit=400&offset=0", #Консервы, соленья
    "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C13062/products?mode=delivery&include_restrict=true&limit=400&offset=0", #Всё для выпечки
    "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C13063/products?mode=delivery&include_restrict=true&limit=400&offset=0", #залить кипятком
]

options = webdriver.ChromeOptions()
options.add_argument('--disable-blink-features=AutomationControlled')
options.add_experimental_option("excludeSwitches", ["enable-automation"])
options.add_experimental_option('useAutomationExtension', False)

# Другие полезные опции
options.add_argument("--no-sandbox")
options.add_argument("--disable-dev-shm-usage")
options.add_argument("--disable-extensions")
options.add_argument("--disable-plugins")
options.add_argument("--disable-images")
# Осторожно с отключением JavaScript - может сломать сайт
# options.add_argument("--disable-javascript")

# Случайный User-Agent
user_agents = [
    "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36",
    "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36",
    "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36"
]
options.add_argument(f'--user-agent={random.choice(user_agents)}')

# Если нужно, раскомментируйте headless
# options.add_argument("--headless")

try:
    service = ChromeService(ChromeDriverManager().install())
    driver = webdriver.Chrome(service=service, options=options)
    print("[✓] ChromeDriver успешно установлен и запущен")
except Exception as e:
    print(f"[!] Ошибка при запуске ChromeDriver: {e}")
    sys.exit(1)

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
    filename = f"../urls/page_{i}.json"
    with open(filename, "w", encoding="utf-8") as f:
        f.write(html)

    print(f"[+] Сохранена страница {url} → {filename}")

driver.quit()
sys.exit(0)
