import time
import random
import subprocess
import socket
import sys
import os
from selenium import webdriver
import undetected_chromedriver as uc

class SSHProxy:
    def __init__(self, server_user, server_ip, server_port=22, local_port=1080):
        self.server_user = server_user
        self.server_ip = server_ip
        self.server_port = server_port
        self.local_port = local_port
        self.process = None
    
    def start_ssh_tunnel(self):
        """Запуск SSH-туннеля для SOCKS5 прокси"""
        try:
            # Проверяем, свободен ли порт
            if not self._check_port(self.local_port):
                print(f"[!] Порт {self.local_port} занят, пробую другой...")
                self.local_port = self._find_free_port()
            
            # Команда для создания SOCKS5 прокси через SSH
            cmd = [
                'ssh',
                '-D', str(self.local_port),
                '-q', '-C', '-N',
                '-o', 'ExitOnForwardFailure=yes',
                '-o', 'ServerAliveInterval=60',
                '-o', 'StrictHostKeyChecking=no',
                f'{self.server_user}@{self.server_ip}',
                '-p', str(self.server_port)
            ]
            
            print(f"[*] Запускаю SSH-туннель к {self.server_ip}...")
            print(f"[*] Локальный SOCKS5 прокси: 127.0.0.1:{self.local_port}")
            
            self.process = subprocess.Popen(
                cmd,
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE
            )
            
            # Даём время на установку соединения
            time.sleep(3)
            
            # Проверяем, запустился ли туннель
            if self.process.poll() is None:
                print(f"[✓] SSH-туннель успешно запущен")
                return True
            else:
                error = self.process.stderr.read().decode()
                print(f"[!] Ошибка SSH: {error}")
                return False
                
        except Exception as e:
            print(f"[!] Ошибка при запуске туннеля: {e}")
            return False
    
    def _check_port(self, port):
        """Проверка, свободен ли порт"""
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            try:
                s.bind(('127.0.0.1', port))
                return True
            except socket.error:
                return False
    
    def _find_free_port(self):
        """Поиск свободного порта"""
        for port in range(1080, 1100):
            if self._check_port(port):
                return port
        return 1080  # если ничего не нашли
    
    def stop_ssh_tunnel(self):
        """Остановка SSH-туннеля"""
        if self.process:
            self.process.terminate()
            self.process.wait()
            print("[*] SSH-туннель остановлен")

def setup_driver_with_proxy(proxy_port=1080):
    """Настройка драйвера с SOCKS5 прокси"""
    
    options = uc.ChromeOptions()
    
    # Настройка прокси
    options.add_argument(f'--proxy-server=socks5://127.0.0.1:{proxy_port}')
    
    # Маскировка от детекта
    options.add_argument("--no-sandbox")
    options.add_argument("--disable-dev-shm-usage")
    options.add_argument("--disable-gpu")
    options.add_argument("--disable-blink-features=AutomationControlled")
    options.add_argument("--disable-blink-features")
    options.add_argument("--window-size=1920,1080")
    options.add_argument("--start-maximized")
    options.add_argument("--lang=ru-RU,ru;q=0.9,en-US;q=0.8,en;q=0.7")
    
    # Случайный User-Agent
    user_agents = [
        "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36",
        "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/119.0.0.0 Safari/537.36",
        "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:109.0) Gecko/20100101 Firefox/121.0",
        "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36"
    ]
    options.add_argument(f'--user-agent={random.choice(user_agents)}')
    
    # Дополнительные аргументы для стабильности
    options.add_argument("--disable-web-security")
    options.add_argument("--disable-features=IsolateOrigins,site-per-process")
    options.add_argument("--disable-renderer-backgrounding")
    options.add_argument("--disable-background-timer-throttling")
    
    try:
        print("[*] Запускаю браузер с прокси...")
        driver = uc.Chrome(options=options, version_main=144)  # Укажи свою версию Chrome
        
        # Дополнительная маскировка через JavaScript
        driver.execute_script("""
            Object.defineProperty(navigator, 'webdriver', {get: () => undefined});
            Object.defineProperty(navigator, 'plugins', {get: () => [1, 2, 3, 4, 5]});
            Object.defineProperty(navigator, 'languages', {get: () => ['ru-RU', 'ru', 'en-US', 'en']});
        """)
        
        return driver
    except Exception as e:
        print(f"[!] Ошибка при запуске драйвера: {e}")
        return None

def human_like_scroll(driver):
    """Имитация человеческого скролла"""
    print("[*] Прокручиваю страницу...")
    
    total_height = driver.execute_script("return document.body.scrollHeight")
    current_position = 0
    scroll_attempts = 0
    max_attempts = 30
    
    while current_position < total_height and scroll_attempts < max_attempts:
        # Случайный шаг скролла
        scroll_step = random.randint(400, 800)
        current_position += scroll_step
        
        # Плавный скролл
        driver.execute_script(f"window.scrollTo({{top: {current_position}, behavior: 'smooth'}});")
        
        # Случайная пауза
        time.sleep(random.uniform(1.5, 3.0))
        
        # Иногда "читаем" товары
        if random.random() < 0.3:
            time.sleep(random.uniform(2, 4))
        
        # Обновляем высоту страницы
        new_height = driver.execute_script("return document.body.scrollHeight")
        if new_height > total_height:
            total_height = new_height
        
        scroll_attempts += 1
        print(f"  Прогресс: {min(100, int(current_position/total_height*100))}%")
    
    print("[✓] Прокрутка завершена")

def extract_products(driver):
    """Извлечение информации о товарах"""
    products = []
    
    # Селекторы для товаров (на основе анализа сайта)
    product_selectors = [
        ".product-card",
        "[class*='catalog-product']",
        "[class*='product']",
        ".css-1d8k9g0",
        ".catalog-item"
    ]
    
    # Ищем все карточки товаров
    product_elements = []
    for selector in product_selectors:
        elements = driver.find_elements(By.CSS_SELECTOR, selector)
        if elements:
            product_elements = elements
            print(f"[+] Найдено товаров по селектору '{selector}': {len(elements)}")
            break
    
    # Извлекаем данные из каждой карточки
    for product in product_elements[:50]:  # Ограничим для теста
        try:
            product_data = {}
            
            # Название
            name_selectors = [".product-title", ".name", "[class*='title']", "[class*='name']"]
            for sel in name_selectors:
                name = product.find_elements(By.CSS_SELECTOR, sel)
                if name:
                    product_data['name'] = name[0].text.strip()
                    break
            
            # Цена
            price_selectors = [".price", "[class*='price']", ".current-price"]
            for sel in price_selectors:
                price = product.find_elements(By.CSS_SELECTOR, sel)
                if price:
                    product_data['price'] = price[0].text.strip()
                    break
            
            # Если нашли хотя бы название, добавляем
            if 'name' in product_data:
                products.append(product_data)
                
        except Exception as e:
            continue
    
    return products

def parse_5ka_with_server():
    """Основная функция парсинга через сервер"""
    
    # Настройки твоего сервера
    SERVER_USER = input("Введите username для сервера (обычно root или admin): ").strip()
    SERVER_IP = "45.38.170.203"  # IP твоего сервера
    LOCAL_PROXY_PORT = 1080
    
    proxy = None
    driver = None
    
    try:
        # 1. Запускаем SSH-туннель
        print("=" * 60)
        print("Парсер 5ka.ru через сервер 63.12.207.203")
        print("=" * 60)
        
        proxy = SSHProxy(SERVER_USER, SERVER_IP, local_port=LOCAL_PROXY_PORT)
        if not proxy.start_ssh_tunnel():
            print("[!] Не удалось запустить SSH-туннель")
            print("[*] Проверьте:")
            print("  1. Правильность username")
            print("  2. Доступность сервера (ping 3)")
            print("  3. Настроены ли SSH ключи")
            return
        
        # 2. Запускаем браузер с прокси
        driver = setup_driver_with_proxy(proxy.local_port)
        if not driver:
            return
        
        # 3. Проверяем IP через прокси
        print("[*] Проверяю IP через прокси...")
        driver.get("https://api.ipify.org?format=text")
        time.sleep(3)
        current_ip = driver.page_source.strip()
        print(f"[+] Текущий IP (должен быть {SERVER_IP}): {current_ip}")
        
        if current_ip != SERVER_IP:
            print("[!] Внимание: IP не совпадает с сервером!")
            cont = input("Продолжить? (y/n): ")
            if cont.lower() != 'y':
                return
        
        # 4. Переходим на 5ka.ru
        url = "https://5ka.ru/search/?text=choco"
        print(f"[→] Открываю {url}")
        driver.get(url)
        
        # 5. Ждём загрузки
        print("[*] Ожидаю загрузки страницы...")
        time.sleep(5)
        
        # Проверяем на блокировку
        page_text = driver.page_source.lower()
        if "forbidden" in page_text or "доступ запрещен" in page_text:
            print("[!] Похоже, доступ всё ещё запрещён!")
            driver.save_screenshot("forbidden.png")
            print("[*] Скриншот сохранён как forbidden.png")
            
            # Показываем HTML ошибки
            print("[*] Получен ответ:")
            print(driver.page_source[:500])
            return
        
        # 6. Прокручиваем страницу
        human_like_scroll(driver)
        
        # 7. Сохраняем HTML
        html = driver.page_source
        filename = f"5ka_choco_{int(time.time())}.html"
        with open(filename, "w", encoding="utf-8") as f:
            f.write(html)
        print(f"[+] HTML сохранён в {filename}")
        
        # 8. Извлекаем товары
        print("[*] Извлекаю данные о товарах...")
        products = extract_products(driver)
        
        # 9. Сохраняем в JSON
        import json
        json_filename = f"5ka_choco_{int(time.time())}.json"
        with open(json_filename, "w", encoding="utf-8") as f:
            json.dump(products, f, ensure_ascii=False, indent=2)
        print(f"[+] Найдено товаров: {len(products)}")
        print(f"[+] Данные сохранены в {json_filename}")
        
        # 10. Сохраняем скриншот
        driver.save_screenshot("final_page.png")
        print("[+] Скриншот сохранён")
        
    except KeyboardInterrupt:
        print("\n[*] Прервано пользователем")
    except Exception as e:
        print(f"[!] Ошибка: {e}")
        import traceback
        traceback.print_exc()
        
        if driver:
            driver.save_screenshot("error.png")
            print("[*] Скриншот ошибки сохранён")
    
    finally:
        # Очистка
        if driver:
            driver.quit()
        if proxy:
            proxy.stop_ssh_tunnel()
        print("[*] Завершено")

def check_ssh_key():
    """Проверка наличия SSH ключа"""
    import os.path
    ssh_key = os.path.expanduser("~/.ssh/id_rsa.pub")
    if not os.path.exists(ssh_key):
        print("[!] SSH ключ не найден. Рекомендую настроить:")
        print("  ssh-keygen -t rsa -b 4096")
        print(f"  ssh-copy-id root@45.38.170.203")

if __name__ == "__main__":
    check_ssh_key()
    parse_5ka_with_server()