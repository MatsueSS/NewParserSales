from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC
import time
import random
import sys

urls = [
    "https://5ka.ru/search/?text=choco", #choco
    "https://5ka.ru/search/?text=%D1%81%D0%BF%D0%B0%D0%B3%D0%B5%D1%82%D1%82%D0%B8", #спагетти
    "https://5ka.ru/search/?text=%D1%80%D0%B8%D1%81", #рис
    "https://5ka.ru/search/?text=%D1%8F%D0%B1%D0%BB%D0%BE%D0%BA%D0%BE", #яблоко
    "https://5ka.ru/search/?text=%D0%B1%D0%B0%D0%BD%D0%B0%D0%BD%D1%8B", #бананы
    "https://5ka.ru/search/?text=%D1%87%D0%B0%D0%B9", #чай
    "https://5ka.ru/search/?text=%D0%A1%D1%82%D0%B8%D1%80%D0%B0%D0%BB%D1%8C%D0%BD%D1%8B%D0%B9%20%D0%BF%D0%BE%D1%80%D0%BE%D1%88%D0%BE%D0%BA", #стиральный порошок
    "https://5ka.ru/search/?text=%D0%BF%D1%8E%D1%80%D0%B5", #пюре
    "https://5ka.ru/search/?text=%D0%BA%D1%83%D0%BA%D1%83%D1%80%D1%83%D0%B7%D0%B0", #кукуруза
    "https://5ka.ru/search/?text=%D0%BA%D1%80%D0%B5%D0%BA%D0%B5%D1%80", #крекер
    "https://5ka.ru/search/?text=%D0%BC%D0%BE%D0%BB%D0%BE%D0%BA%D0%BE", #молоко
    "https://5ka.ru/search/?text=%D0%B3%D0%BE%D1%80%D0%BE%D1%88%D0%B5%D0%BA", #горошек
    "https://5ka.ru/search/?text=%D0%BC%D1%8E%D1%81%D0%BB%D0%B8", #мюсли
    "https://5ka.ru/search/?text=%D1%81%D1%80%D0%B5%D0%B4%D1%81%D1%82%D0%B2%D0%BE%20%D0%B4%D0%BB%D1%8F%20%D0%BC%D1%8B%D1%82%D1%8C%D1%8F%20%D0%BF%D0%BE%D1%81%D1%83%D0%B4%D1%8B", #средство для мытья посуды
    "https://5ka.ru/search/?text=%D1%81%D0%BC%D0%B5%D1%81%D1%8C", #смесь
    "https://5ka.ru/search/?text=%D1%81%D1%8B%D1%80", #сыр
    "https://5ka.ru/search/?text=%D0%99%D0%BE%D0%B3%D1%83%D1%80%D1%82", #йогурт
    "https://5ka.ru/search/?text=%D0%B2%D0%B0%D1%80%D0%B5%D0%BD%D0%B8%D0%BA%D0%B8", #вареники
    "https://5ka.ru/search/?text=%D1%82%D0%B2%D0%BE%D1%80%D0%BE%D0%B3", #творог
    "https://5ka.ru/search/?text=%D0%BF%D0%B5%D1%87%D0%B5%D0%BD%D1%8C%D0%B5", #печенье
    "https://5ka.ru/search/?text=%D1%85%D0%BB%D0%BE%D0%BF%D1%8C%D1%8F", #хлопья
    "https://5ka.ru/search/?text=%D1%8F%D0%B9%D1%86%D0%B0", #яйца
    "https://5ka.ru/search/?text=%D0%B7%D1%83%D0%B1%D0%BD%D0%B0%D1%8F%20%D0%BF%D0%B0%D1%81%D1%82%D0%B0", #зубная паста
    "http://5ka.ru/search/?text=%D0%BE%D0%B3%D1%83%D1%80%D1%86%D1%8B", #огурцы
    "https://5ka.ru/search/?text=%D1%82%D1%83%D1%88%D1%91%D0%BD%D0%BA%D0%B0", #тушёнка
    "https://5ka.ru/search/?text=%D0%BC%D0%B0%D0%BD%D0%B4%D0%B0%D1%80%D0%B8%D0%BD%D1%8B", #мандарины
    "https://5ka.ru/catalog/sgushchyonka--251C13168/", #Сгущёнка
    "https://5ka.ru/search/?text=%D0%9C%D0%B0%D1%81%D0%BB%D0%BE%20%D1%81%D0%BB%D0%B8%D0%B2%D0%BE%D1%87%D0%BD%D0%BE%D0%B5", #масло сливочное
    "https://5ka.ru/search/?text=%D0%BB%D0%B8%D0%BC%D0%BE%D0%BD%D1%8B", #лимоны
    "https://5ka.ru/search/?text=%D0%BF%D0%BE%D0%BC%D0%B8%D0%B4%D0%BE%D1%80%D1%8B", #помидоры
]

options = webdriver.ChromeOptions()

# Основные настройки для обхода детекта
options.add_argument("--disable-blink-features=AutomationControlled")
options.add_experimental_option("excludeSwitches", ["enable-automation"])
options.add_experimental_option('useAutomationExtension', False)

# Добавляем реальные заголовки
options.add_argument("--accept-lang=ru-RU,ru;q=0.9,en;q=0.8")
options.add_argument("--accept=text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8")

# Случайный User-Agent
user_agents = [
    "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36",
    "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/119.0.0.0 Safari/537.36",
    "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36"
]
options.add_argument(f'--user-agent={random.choice(user_agents)}')

# Важно: НЕ отключайте JavaScript!
# options.add_argument("--disable-javascript")  # Удалите эту строку!

driver = webdriver.Chrome(options=options)

# Исполняем JavaScript для скрытия признаков автоматизации
driver.execute_script("Object.defineProperty(navigator, 'webdriver', {get: () => undefined})")
driver.execute_cdp_cmd('Network.setUserAgentOverride', {
    "userAgent": random.choice(user_agents),
    "platform": "Windows"
})

# Устанавливаем реальные заголовки через CDP
driver.execute_cdp_cmd('Network.setExtraHTTPHeaders', {
    "headers": {
        "Accept-Language": "ru-RU,ru;q=0.9,en;q=0.8",
        "Accept": "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8",
        "Referer": "https://www.google.com/",
        "DNT": "1",
        "Connection": "keep-alive",
        "Upgrade-Insecure-Requests": "1",
        "Sec-Fetch-Dest": "document",
        "Sec-Fetch-Mode": "navigate",
        "Sec-Fetch-Site": "cross-site"
    }
})

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

