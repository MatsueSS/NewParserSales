import time
import random
import sys
import os
import json
import subprocess
from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC
from selenium.common.exceptions import TimeoutException, WebDriverException
import undetected_chromedriver as uc

def get_chrome_version():
    """Получение установленной версии Chrome"""
    try:
        # Для Linux
        result = subprocess.run(['google-chrome', '--version'], 
                              capture_output=True, text=True)
        version = result.stdout.strip().split()[-1]
        return version.split('.')[0]  # Берём только мажорную версию
    except:
        try:
            # Альтернативный путь для Linux
            result = subprocess.run(['chrome', '--version'], 
                                  capture_output=True, text=True)
            version = result.stdout.strip().split()[-1]
            return version.split('.')[0]
        except:
            print("[!] Не удалось определить версию Chrome, использую автоматическое определение")
            return None

def setup_driver():
    """Настройка драйвера с автоматическим определением версии"""
    
    # Получаем версию Chrome
    chrome_version = get_chrome_version()
    if chrome_version:
        print(f"[*] Обнаружена версия Chrome: {chrome_version}")
    
    options = uc.ChromeOptions()
    
    # Основные опции для маскировки
    options.add_argument("--no-sandbox")
    options.add_argument("--disable-dev-shm-usage")
    options.add_argument("--disable-gpu")
    options.add_argument("--disable-web-security")
    options.add_argument("--disable-features=IsolateOrigins,site-per-process")
    options.add_argument("--disable-blink-features=AutomationControlled")
    options.add_argument("--disable-blink-features")
    options.add_argument("--disable-renderer-backgrounding")
    options.add_argument("--disable-background-timer-throttling")
    options.add_argument("--disable-backgrounding-occluded-windows")
    options.add_argument("--disable-client-side-phishing-detection")
    options.add_argument("--disable-crash-reporter")
    options.add_argument("--disable-oopr-debug-crash-dump")
    options.add_argument("--no-crash-upload")
    options.add_argument("--disable-low-res-tiling")
    options.add_argument("--log-level=3")
    options.add_argument("--silent")
    
    # Случайный User-Agent
    user_agents = [
        "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36",
        "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/119.0.0.0 Safari/537.36",
        "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:109.0) Gecko/20100101 Firefox/121.0",
        "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36"
    ]
    options.add_argument(f'--user-agent={random.choice(user_agents)}')
    
    # Дополнительные параметры
    options.add_argument("--window-size=1920,1080")
    options.add_argument("--start-maximized")
    options.add_argument("--lang=ru-RU,ru;q=0.9,en-US;q=0.8,en;q=0.7")
    
    # Пробуем разные способы запуска
    try:
        # Способ 1: с явным указанием версии
        if chrome_version:
            try:
                driver = uc.Chrome(options=options, version_main=int(chrome_version))
                print(f"[✓] Драйвер запущен с версией Chrome {chrome_version}")
                return driver
            except Exception as e:
                print(f"[!] Не удалось запустить с версией {chrome_version}: {e}")
        
        # Способ 2: без указания версии (автоопределение)
        print("[*] Пробую автоматическое определение версии...")
        driver = uc.Chrome(options=options)
        print("[✓] Драйвер запущен с автоматическим определением версии")
        return driver
        
    except Exception as e:
        print(f"[!] Ошибка при запуске undetected_chromedriver: {e}")
        print("[*] Пробую альтернативный метод...")
        
        # Альтернативный метод с webdriver_manager
        return setup_driver_fallback()

def setup_driver_fallback():
    """Запасной вариант с обычным Selenium"""
    from selenium.webdriver.chrome.service import Service
    from webdriver_manager.chrome import ChromeDriverManager
    from selenium.webdriver.chrome.options import Options
    
    print("[*] Использую запасной вариант с webdriver_manager...")
    
    chrome_options = Options()
    chrome_options.add_argument("--no-sandbox")
    chrome_options.add_argument("--disable-dev-shm-usage")
    chrome_options.add_argument("--disable-gpu")
    chrome_options.add_argument("--disable-blink-features=AutomationControlled")
    chrome_options.add_experimental_option("excludeSwitches", ["enable-automation"])
    chrome_options.add_experimental_option('useAutomationExtension', False)
    chrome_options.add_argument("--window-size=1920,1080")
    
    try:
        service = Service(ChromeDriverManager().install())
        driver = webdriver.Chrome(service=service, options=chrome_options)
        
        # Маскировка
        driver.execute_script("Object.defineProperty(navigator, 'webdriver', {get: () => undefined})")
        print("[✓] Запасной драйвер запущен")
        return driver
    except Exception as e:
        print(f"[!] Критическая ошибка: {e}")
        sys.exit(1)

def human_like_scroll(driver):
    """Имитация человеческого скролла"""
    print("[*] Прокручиваю страницу...")
    
    total_height = driver.execute_script("return document.body.scrollHeight")
    viewport_height = driver.execute_script("return window.innerHeight")
    
    current_position = 0
    scroll_attempts = 0
    max_attempts = 40
    
    while current_position < total_height and scroll_attempts < max_attempts:
        # Случайный шаг скролла
        scroll_step = random.randint(300, 800)
        current_position += scroll_step
        
        # Плавный скролл
        driver.execute_script(f"window.scrollTo({{top: {current_position}, behavior: 'smooth'}});")
        
        # Случайная пауза
        time.sleep(random.uniform(1.0, 2.5))
        
        # Иногда останавливаемся и "читаем"
        if random.random() < 0.25:
            time.sleep(random.uniform(2, 4))
            
            # Иногда скроллим немного вверх
            if random.random() < 0.15:
                back_scroll = random.randint(100, 250)
                driver.execute_script(f"window.scrollTo({{top: {current_position - back_scroll}, behavior: 'smooth'}});")
                time.sleep(random.uniform(0.8, 1.5))
                # Снова скроллим вниз
                driver.execute_script(f"window.scrollTo({{top: {current_position}, behavior: 'smooth'}});")
        
        # Обновляем высоту страницы
        new_height = driver.execute_script("return document.body.scrollHeight")
        if new_height > total_height:
            total_height = new_height
        
        scroll_attempts += 1
        
        # Прогресс
        if scroll_attempts % 5 == 0:
            print(f"  Прогресс: {min(100, int(current_position/total_height*100))}%")
    
    print("[✓] Прокрутка завершена")

def parse_5ka_search(query="choco"):
    """Основная функция парсинга"""
    
    url = f"https://5ka.ru/search/?text={query}"
    timestamp = int(time.time())
    html_filename = f"5ka_{query}_{timestamp}.html"
    
    driver = None
    try:
        # Настройка драйвера
        print("[*] Настраиваю драйвер...")
        driver = setup_driver()
        
        # Открываем страницу
        print(f"[→] Открываю: {url}")
        driver.get(url)
        
        # Ждём загрузки
        print("[*] Ожидаю загрузки страницы...")
        time.sleep(random.uniform(5, 8))
        
        # Проверяем, не заблокировали ли нас
        if "captcha" in driver.page_source.lower() or "проверка" in driver.page_source.lower():
            print("[!] Обнаружена капча или проверка!")
            driver.save_screenshot("captcha_detected.png")
            print("[*] Скриншот сохранён как captcha_detected.png")
            
            # Даём время на ручное решение капчи
            input("[?] Решите капчу в браузере и нажмите Enter...")
        
        # Поиск контента
        print("[*] Ищу товары на странице...")
        
        # Ждём появления любого контента
        wait = WebDriverWait(driver, 20)
        try:
            wait.until(lambda d: len(d.find_elements(By.CSS_SELECTOR, 
                "[class*='product'], [class*='Product'], [class*='catalog'], [class*='Catalog']")) > 0)
            print("[+] Товары найдены!")
        except TimeoutException:
            print("[!] Таймаут ожидания товаров")
        
        # Сохраняем начальное состояние
        driver.save_screenshot("initial_state.png")
        
        # Прокрутка
        human_like_scroll(driver)
        
        # Дополнительная прокрутка для подгрузки
        time.sleep(random.uniform(3, 5))
        
        # Извлекаем HTML
        html = driver.page_source
        
        # Сохраняем HTML
        with open(html_filename, "w", encoding="utf-8") as f:
            f.write(html)
        print(f"[+] HTML сохранён в {html_filename}")
        
        # Сохраняем скриншот результата
        driver.save_screenshot("final_state.png")
        print("[+] Финальный скриншот сохранён")
        
        # Подсчёт приблизительного количества товаров
        product_count = len(driver.find_elements(By.CSS_SELECTOR, 
            "[class*='product'], [class*='Product'], [class*='catalog'], [class*='Catalog'], .css-0"))
        print(f"[+] Примерное количество товаров: {product_count}")
        
    except Exception as e:
        print(f"[!] Ошибка: {e}")
        import traceback
        traceback.print_exc()
        
        if driver:
            driver.save_screenshot("error_screenshot.png")
            print("[*] Скриншот ошибки сохранён")
        
    finally:
        if driver:
            time.sleep(random.uniform(2, 3))
            driver.quit()
            print("[*] Браузер закрыт")

def main():
    """Главная функция"""
    print("=" * 50)
    print("Парсер 5ka.ru")
    print("=" * 50)
    
    # Проверка установки undetected-chromedriver
    try:
        import undetected_chromedriver
        print("[✓] undetected-chromedriver установлен")
    except ImportError:
        print("[!] Устанавливаю undetected-chromedriver...")
        subprocess.check_call([sys.executable, "-m", "pip", "install", "undetected-chromedriver"])
        print("[✓] undetected-chromedriver установлен")
    
    # Запуск парсинга
    parse_5ka_search("choco")
    
    print("\n[✓] Готово!")

if __name__ == "__main__":
    main()