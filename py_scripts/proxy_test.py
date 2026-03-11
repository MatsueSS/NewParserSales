from selenium import webdriver
from selenium.webdriver.chrome.service import Service as ChromeService
from webdriver_manager.chrome import ChromeDriverManager
from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC
import time
import random
import sys
import socket

# ==================== НАСТРОЙКИ ПРОКСИ ====================
# Данные вашего сервера
PROXY_HOST = "45.38.170.203"  # IP вашего сервера
PROXY_PORT = "1080"            # Порт для прокси
USE_PROXY = True                # Включить/выключить прокси

# Функция проверки доступности прокси
def check_proxy(host, port):
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.settimeout(3)
    result = sock.connect_ex((host, port))
    sock.close()
    return result == 0

# Проверяем прокси перед запуском
if USE_PROXY:
    print(f"[*] Проверка прокси {PROXY_HOST}:{PROXY_PORT}...")
    if check_proxy('127.0.0.1', int(PROXY_PORT)):
        print("[✓] Прокси доступен!")
    else:
        print("[✗] Прокси НЕ доступен!")
        print(f"    Убедитесь, что SSH-туннель запущен: ssh -D {PROXY_PORT} -C -N root@{PROXY_HOST}")
        response = input("Продолжить без прокси? (y/n): ")
        if response.lower() != 'y':
            sys.exit(1)
        USE_PROXY = False

# ==================== НАСТРОЙКИ БРАУЗЕРА ====================
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
    "https://5ka.ru/search/?text=%D1%82%D1%83%D1%88%D1%91%D0%BD%D0%BA%D0%B0",
    "https://5ka.ru/search/?text=%D1%82%D0%B2%D0%BE%D1%80%D0%BE%D0%B3",
    "https://5ka.ru/search/?text=%D0%BC%D0%B0%D0%BD%D0%B4%D0%B0%D1%80%D0%B8%D0%BD%D1%8B",
    "https://5ka.ru/catalog/sgushchyonka--251C13168/",
    "https://5ka.ru/search/?text=%D0%9C%D0%B0%D1%81%D0%BB%D0%BE%20%D1%81%D0%BB%D0%B8%D0%B2%D0%BE%D1%87%D0%BD%D0%BE%D0%B5"
]

options = webdriver.ChromeOptions()

# ДОБАВЛЯЕМ ПРОКСИ (если включено)
if USE_PROXY:
    options.add_argument(f'--proxy-server=socks5://127.0.0.1:{PROXY_PORT}')
    print(f"[+] Использую прокси: socks5://127.0.0.1:{PROXY_PORT}")

# Убираем признаки автоматизации
options.add_argument("--disable-blink-features=AutomationControlled")
options.add_experimental_option("excludeSwitches", ["enable-automation"])
options.add_experimental_option('useAutomationExtension', False)

# Добавляем реальные заголовки
options.add_argument("--accept-lang=ru-RU,ru;q=0.9,en;q=0.8")
options.add_argument("--accept=text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8")

# Другие полезные опции
options.add_argument("--no-sandbox")
options.add_argument("--disable-dev-shm-usage")
options.add_argument("--disable-extensions")
options.add_argument("--disable-plugins")
options.add_argument("--disable-images")
options.add_argument("--window-size=1920,1080")  # Фиксированный размер окна

# Случайный User-Agent
user_agents = [
    "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36",
    "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36",
    "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36"
]
options.add_argument(f'--user-agent={random.choice(user_agents)}')

# Запуск драйвера
try:
    service = ChromeService(ChromeDriverManager().install())
    driver = webdriver.Chrome(service=service, options=options)
    print("[✓] ChromeDriver успешно установлен и запущен")
except Exception as e:
    print(f"[!] Ошибка при запуске ChromeDriver: {e}")
    sys.exit(1)

# ==================== УЛУЧШЕННАЯ ПРОКРУТКА ====================
def human_like_scroll(driver, max_scrolls=30):
    """
    Имитация человеческого скролла с постепенной прокруткой
    """
    print("[*] Начинаю прокрутку страницы...")
    
    # Получаем высоту страницы
    last_height = driver.execute_script("return document.body.scrollHeight")
    scroll_count = 0
    no_change_count = 0
    
    while scroll_count < max_scrolls:
        # Случайная высота прокрутки (как человек)
        scroll_step = random.randint(300, 700)
        current_scroll = driver.execute_script("return window.pageYOffset")
        
        # Плавная прокрутка
        driver.execute_script(f"window.scrollTo({{top: {current_scroll + scroll_step}, behavior: 'smooth'}});")
        
        # Случайная пауза между скроллами
        time.sleep(random.uniform(1.5, 3.0))
        
        # Иногда скроллим немного вверх (имитация человека)
        if random.random() < 0.15:  # 15% шанс
            scroll_up = random.randint(100, 200)
            current = driver.execute_script("return window.pageYOffset")
            driver.execute_script(f"window.scrollTo({{top: {current - scroll_up}, behavior: 'smooth'}});")
            time.sleep(random.uniform(1.0, 2.0))
        
        # Проверяем, загрузились ли новые элементы
        new_height = driver.execute_script("return document.body.scrollHeight")
        
        if new_height > last_height:
            print(f"  [+] Загружены новые товары! Высота: {new_height}")
            last_height = new_height
            no_change_count = 0
        else:
            no_change_count += 1
        
        # Если долго нет новых элементов, возможно достигли конца
        if no_change_count > 5:
            print("  [!] Достигнут конец страницы (нет новых товаров)")
            break
        
        scroll_count += 1
        
        # Случайная длинная пауза (имитация чтения)
        if scroll_count % 7 == 0:
            pause = random.uniform(3.0, 6.0)
            print(f"  [*] Пауза {pause:.1f} сек (имитация чтения)...")
            time.sleep(pause)
    
    # Возвращаемся наверх для следующей страницы
    driver.execute_script("window.scrollTo({top: 0, behavior: 'smooth'});")
    time.sleep(2)
    print("[✓] Прокрутка завершена")

def check_ip():
    """Проверка текущего IP"""
    try:
        driver.get("https://api.ipify.org?format=json")
        time.sleep(3)
        import json
        ip_info = json.loads(driver.find_element(By.TAG_NAME, "body").text)
        print(f"[+] Текущий IP: {ip_info['ip']}")
        return ip_info['ip']
    except Exception as e:
        print(f"[!] Не удалось проверить IP: {e}")
        return None

# ==================== ОСНОВНОЙ ЦИКЛ ====================
print("\n" + "="*50)
print("НАЧАЛО ПАРСИНГА")
print("="*50 + "\n")

# Проверяем IP через прокси (если используется)
if USE_PROXY:
    print("[*] Проверка IP через прокси...")
    current_ip = check_ip()
    if current_ip:
        print(f"[✓] Прокси работает! Ваш IP: {current_ip}")
    else:
        print("[✗] Прокси не работает!")
        response = input("Продолжить без прокси? (y/n): ")
        if response.lower() != 'y':
            driver.quit()
            sys.exit(1)

# Основной цикл по URL
for i, url in enumerate(urls, start=1):
    print(f"\n[{i}/{len(urls)}] → Открываю: {url}")
    
    # Случайная задержка перед открытием
    time.sleep(random.uniform(3, 6))
    
    try:
        driver.get(url)
        
        # Ждём загрузки страницы
        time.sleep(random.uniform(4, 7))
        
        # Проверяем, не заблокированы ли мы
        if "forbidden" in driver.page_source.lower() or "доступ запрещен" in driver.page_source.lower():
            print("[!] ВНИМАНИЕ: Страница вернула Forbidden!")
            if USE_PROXY:
                print("[*] Проверяем текущий IP...")
                check_ip()
            continue
        
        # Ждём появления карточек товаров
        try:
            WebDriverWait(driver, 30).until(
                EC.presence_of_element_located((By.CSS_SELECTOR, ".css-0"))
            )
            print("[✓] Карточки товаров найдены")
        except Exception as e:
            print(f"[!] Не удалось дождаться карточек: {e}")
            # Пробуем другой селектор
            try:
                WebDriverWait(driver, 10).until(
                    EC.presence_of_element_located((By.CSS_SELECTOR, ".product-item"))
                )
                print("[✓] Карточки товаров найдены (альтернативный селектор)")
            except:
                print("[!] Карточки не найдены, сохраняем как есть")
        
        # Человеческая прокрутка
        human_like_scroll(driver, max_scrolls=25)
        
        # Сохраняем HTML
        html = driver.page_source
        filename = f"../urls/page_{i}.html"
        with open(filename, "w", encoding="utf-8") as f:
            f.write(html)
        
        print(f"[+] Сохранена страница {i} → {filename}")
        print(f"    Размер файла: {len(html)} символов")
        
    except Exception as e:
        print(f"[!] Ошибка при обработке {url}: {e}")
        continue

print("\n" + "="*50)
print("ПАРСИНГ ЗАВЕРШЕН")
print("="*50)
driver.quit()
sys.exit(0)