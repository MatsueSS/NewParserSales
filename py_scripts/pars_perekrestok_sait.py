from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC
from selenium.webdriver.chrome.service import Service
from selenium.webdriver.common.action_chains import ActionChains
from webdriver_manager.chrome import ChromeDriverManager
import time
import random
import sys

urls = [
    "https://www.perekrestok.ru/cat/search?search=choco",
    "https://www.perekrestok.ru/cat/search?search=%D1%81%D0%BF%D0%B0%D0%B3%D0%B5%D1%82%D1%82%D0%B8",
    "https://www.perekrestok.ru/cat/search?search=%D1%80%D0%B8%D1%81",
    "https://www.perekrestok.ru/cat/search?search=%D1%8F%D0%B1%D0%BB%D0%BE%D0%BA%D0%BE",
    "https://www.perekrestok.ru/cat/search?search=%D0%B1%D0%B0%D0%BD%D0%B0%D0%BD%D1%8B",
    "https://www.perekrestok.ru/cat/search?search=%D1%87%D0%B0%D0%B9",
    "https://www.perekrestok.ru/cat/search?search=%D1%81%D1%82%D0%B8%D1%80%D0%B0%D0%BB%D1%8C%D0%BD%D1%8B%D0%B9%20%D0%BF%D0%BE%D1%80%D0%BE%D1%88%D0%BE%D0%BA",
    "https://www.perekrestok.ru/cat/search?search=%D0%BF%D1%8E%D1%80%D0%B5",
    "https://www.perekrestok.ru/cat/search?search=%D0%BA%D1%83%D0%BA%D1%83%D1%80%D1%83%D0%B7%D0%B0",
    "https://www.perekrestok.ru/cat/search?search=%D0%BA%D1%80%D0%B5%D0%BA%D0%B5%D1%80",
    "https://www.perekrestok.ru/cat/search?search=%D0%BC%D0%BE%D0%BB%D0%BE%D0%BA%D0%BE",
    "https://www.perekrestok.ru/cat/search?search=%D0%B3%D0%BE%D1%80%D0%BE%D1%88%D0%B5%D0%BA",
    "https://www.perekrestok.ru/cat/search?search=%D0%BC%D1%8E%D1%81%D0%BB%D0%B8",
    "https://www.perekrestok.ru/cat/search?search=%D1%81%D1%80%D0%B5%D0%B4%D1%81%D1%82%D0%B2%D0%BE%20%D0%B4%D0%BB%D1%8F%20%D0%BC%D1%8B%D1%82%D1%8C%D1%8F%20%D0%BF%D0%BE%D1%81%D1%83%D0%B4%D1%8B",
    "https://www.perekrestok.ru/cat/search?search=%D1%81%D0%BC%D0%B5%D1%81%D1%8C",
    "https://www.perekrestok.ru/cat/search?search=%D1%81%D1%8B%D1%80",
    "https://www.perekrestok.ru/cat/search?search=%D0%B9%D0%BE%D0%B3%D1%83%D1%80%D1%82",
    "https://www.perekrestok.ru/cat/search?search=%D0%B2%D0%B0%D1%80%D0%B5%D0%BD%D0%B8%D0%BA%D0%B8",
    "https://www.perekrestok.ru/cat/search?search=%D1%82%D0%B2%D0%BE%D1%80%D0%BE%D0%B3",
    "https://www.perekrestok.ru/cat/search?search=%D0%BF%D0%B5%D1%87%D0%B5%D0%BD%D1%8C%D0%B5",
    "https://www.perekrestok.ru/cat/search?search=%D1%85%D0%BB%D0%BE%D0%BF%D1%8C%D1%8F",
    "https://www.perekrestok.ru/cat/search?search=%D1%8F%D0%B9%D1%86%D0%B0",
    "https://www.perekrestok.ru/cat/search?search=%D0%B7%D1%83%D0%B1%D0%BD%D0%B0%D1%8F%20%D0%BF%D0%B0%D1%81%D1%82%D0%B0",
    "https://www.perekrestok.ru/cat/search?search=%D0%BE%D0%B3%D1%83%D1%80%D1%86%D1%8B",
    "https://www.perekrestok.ru/cat/search?search=%D1%82%D1%83%D1%88%D1%91%D0%BD%D0%BA%D0%B0",
    "https://www.perekrestok.ru/cat/search?search=%D0%BC%D0%B0%D0%BD%D0%B4%D0%B0%D1%80%D0%B8%D0%BD%D1%8B",
    "https://www.perekrestok.ru/cat/search?search=%D1%81%D0%B3%D1%83%D1%89%D1%91%D0%BD%D0%BA%D0%B0",
    "https://www.perekrestok.ru/cat/search?search=%D0%BC%D0%B0%D1%81%D0%BB%D0%BE%20%D1%81%D0%BB%D0%B8%D0%B2%D0%BE%D1%87%D0%BD%D0%BE%D0%B5",
    "https://www.perekrestok.ru/cat/search?search=%D0%BB%D0%B8%D0%BC%D0%BE%D0%BD%D1%8B",
    "https://www.perekrestok.ru/cat/search?search=%D0%BF%D0%BE%D0%BC%D0%B8%D0%B4%D0%BE%D1%80",
]

options = webdriver.ChromeOptions()

# Убираем признаки автоматизации
options.add_argument("--disable-blink-features=AutomationControlled")
options.add_experimental_option("excludeSwitches", ["enable-automation"])
options.add_experimental_option('useAutomationExtension', False)

# Другие полезные опции
options.add_argument("--no-sandbox")
options.add_argument("--disable-dev-shm-usage")
options.add_argument("--disable-extensions")
options.add_argument("--disable-plugins")
options.add_argument("--disable-images")
# options.add_argument("--disable-javascript")  # ⚠️ НИ В КОЕМ СЛУЧАЕ НЕ РАСКОММЕНТИРУЙТЕ!

# Случайный User-Agent
user_agents = [
    "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36",
    "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36",
    "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36"
]
options.add_argument(f'--user-agent={random.choice(user_agents)}')

# Установка драйвера
service = Service(ChromeDriverManager().install())
driver = webdriver.Chrome(service=service, options=options)

def close_help_popup(driver):
    """
    Многоступенчатая система клика для закрытия всплывающей помощи
    """
    print("🖱️ Пытаюсь закрыть всплывающую помощь...")
    
    # Способ 1: Клик по координатам через ActionChains (самый надежный для видимых элементов)
    try:
        from selenium.webdriver.common.action_chains import ActionChains
        
        page_width = driver.execute_script("return document.documentElement.clientWidth")
        page_height = driver.execute_script("return document.documentElement.clientHeight")
        
        # Левая средняя часть (15% от левого края, середина по высоте)
        x = int(page_width * 0.15)
        y = int(page_height * 0.5)
        
        # Небольшое случайное смещение
        x += random.randint(-10, 10)
        y += random.randint(-10, 10)
        
        print(f"   Способ 1: ActionChains клик по ({x}, {y})")
        
        # Находим элемент в этой точке
        element = driver.execute_script(f"return document.elementFromPoint({x}, {y});")
        if element:
            print(f"   Найден элемент: {element.tag_name}")
            ActionChains(driver).move_to_element(element).click().perform()
            print("   ✅ ActionChains клик выполнен")
            time.sleep(0.5)
            return True
    except Exception as e:
        print(f"   ⚠️ Способ 1 не сработал: {e}")
    
    # Способ 2: JavaScript клик с более детальным подходом
    try:
        page_width = driver.execute_script("return document.documentElement.clientWidth")
        page_height = driver.execute_script("return document.documentElement.clientHeight")
        
        x = int(page_width * 0.15)
        y = int(page_height * 0.5)
        
        print(f"   Способ 2: JavaScript клик по ({x}, {y})")
        
        # Более сложный JavaScript клик
        result = driver.execute_script(f"""
            // Функция для создания и dispatch события
            function simulateClick(x, y) {{
                // Получаем элемент под курсором
                var element = document.elementFromPoint(x, y);
                if (!element) return false;
                
                console.log('Элемент для клика:', element);
                
                // Создаем последовательность событий для реалистичности
                var events = [
                    new MouseEvent('mouseover', {{clientX: x, clientY: y, bubbles: true}}),
                    new MouseEvent('mousemove', {{clientX: x, clientY: y, bubbles: true}}),
                    new MouseEvent('mousedown', {{clientX: x, clientY: y, bubbles: true}}),
                    new MouseEvent('mouseup', {{clientX: x, clientY: y, bubbles: true}}),
                    new MouseEvent('click', {{clientX: x, clientY: y, bubbles: true}})
                ];
                
                // Отправляем события
                events.forEach(event => element.dispatchEvent(event));
                
                // Также пробуем нативный click если это возможно
                if (element.click && typeof element.click === 'function') {{
                    try {{
                        element.click();
                    }} catch(e) {{
                        console.log('Нативный click не сработал');
                    }}
                }}
                
                return true;
            }}
            
            return simulateClick({x}, {y});
        """)
        
        if result:
            print("   ✅ JavaScript клик выполнен")
            time.sleep(0.5)
            return True
    except Exception as e:
        print(f"   ⚠️ Способ 2 не сработал: {e}")
    
    # Способ 3: Клик по первому попавшемуся продукту
    try:
        print("   Способ 3: Клик по первому продукту")
        first_product = WebDriverWait(driver, 5).until(
            EC.element_to_be_clickable((By.CSS_SELECTOR, ".product-card"))
        )
        first_product.click()
        print("   ✅ Клик по продукту выполнен")
        time.sleep(0.5)
        return True
    except Exception as e:
        print(f"   ⚠️ Способ 3 не сработал: {e}")
    
    # Способ 4: Нажатие ESC
    try:
        print("   Способ 4: Нажатие ESC")
        from selenium.webdriver.common.keys import Keys
        driver.find_element(By.TAG_NAME, "body").send_keys(Keys.ESCAPE)
        print("   ✅ ESC нажат")
        time.sleep(0.5)
        return True
    except Exception as e:
        print(f"   ⚠️ Способ 4 не сработал: {e}")
    
    # Способ 5: Принудительное удаление всплывающих окон через JavaScript
    try:
        print("   Способ 5: Удаление всплывающих элементов")
        driver.execute_script("""
            // Удаляем возможные попапы и оверлеи
            var selectors = [
                '[class*="popup"]',
                '[class*="modal"]',
                '[class*="overlay"]',
                '[class*="tooltip"]',
                '[class*="guide"]',
                '[class*="help"]',
                '[aria-modal="true"]',
                'div[role="dialog"]'
            ];
            
            selectors.forEach(selector => {
                document.querySelectorAll(selector).forEach(el => {
                    if (el && el.parentNode) {
                        console.log('Удаляем элемент:', el);
                        el.style.display = 'none';
                        el.style.visibility = 'hidden';
                        el.remove();
                    }
                });
            });
        """)
        print("   ✅ Всплывающие элементы скрыты")
        time.sleep(0.5)
        return True
    except Exception as e:
        print(f"   ⚠️ Способ 5 не сработал: {e}")
    
    print("❌ Не удалось закрыть всплывающую помощь")
    return False

def slow_scroll_to_bottom(driver, pause_time=0.5, scroll_step=300, max_wait=30):
    last_height = driver.execute_script("return document.body.scrollHeight")
    start_time = time.time()
    card_count = len(driver.find_elements(By.CSS_SELECTOR, ".product-card"))
    current_position = 0
    
    print(f"Начальное количество карточек: {card_count}")
    
    while True:
        # Медленная прокрутка небольшими шагами
        for _ in range(5):  # Делаем несколько маленьких шагов
            current_position += scroll_step
            driver.execute_script(f"window.scrollTo(0, {current_position});")
            time.sleep(pause_time / 2)  # Маленькая пауза между микропрокрутками
            
            # Случайная пауза для имитации человека
            if random.random() < 0.3:  # 30% шанс
                time.sleep(random.uniform(0.2, 0.5))
        
        # Проверяем изменения после серии прокруток
        new_card_count = len(driver.find_elements(By.CSS_SELECTOR, ".product-card"))
        new_height = driver.execute_script("return document.body.scrollHeight")
        
        print(f"Позиция: {current_position}/{new_height}, Карточек: {new_card_count}")
        
        # Если карточек стало больше
        if new_card_count > card_count:
            card_count = new_card_count
            last_height = new_height
            start_time = time.time()
            print(f"✅ Загружено новых карточек, всего: {card_count}")
            
            # Случайная пауза после загрузки новых карточек
            time.sleep(random.uniform(1, 2))
        
        # Если достигли конца страницы
        elif current_position >= new_height - 1000:
            print(f"[!] Достигнут конец страницы. Всего карточек: {card_count}")
            break
        
        # Если долго нет новых карточек
        elif time.time() - start_time > max_wait:
            print(f"[!] Превышено время ожидания. Всего карточек: {card_count}")
            break
        
        else:
            # Случайная пауза перед следующей итерацией
            time.sleep(random.uniform(pause_time, pause_time * 2))

for i, url in enumerate(urls, start=1):
    print(f"\n{'='*50}")
    print(f"[→] Открываю URL #{i}: {url}")
    print(f"{'='*50}")
    
    driver.get(url)
    
    # Даем время на начальную загрузку
    time.sleep(3)
    
    # Кликаем для закрытия всплывающей помощи
    close_help_popup(driver)
    
    # Ждем появления карточек товаров
    try:
        WebDriverWait(driver, 30).until(
            EC.presence_of_element_located((By.CSS_SELECTOR, ".product-card"))
        )
        print("✅ Карточки товаров найдены!")
        
        cards = driver.find_elements(By.CSS_SELECTOR, ".product-card")
        print(f"Найдено карточек: {len(cards)}")
        
        if cards:
            try:
                title = cards[0].find_element(By.CSS_SELECTOR, ".product-card__title").text
                price = cards[0].find_element(By.CSS_SELECTOR, ".price-new").text
                print(f"Пример: {title} - {price}")
            except:
                print("Не удалось получить данные первой карточки")
            
    except Exception as e:
        print(f"[!] Не удалось дождаться карточек на {url}")
        with open(f"debug_page_{i}.html", "w", encoding="utf-8") as f:
            f.write(driver.page_source)
        print("Страница сохранена для анализа в debug_page.html")
        continue

    # Медленная прокрутка
    print("🚀 Начинаю медленную прокрутку...")
    slow_scroll_to_bottom(driver, pause_time=0.7, scroll_step=250, max_wait=25)
    
    # Финальная пауза перед сохранением
    time.sleep(random.uniform(2, 3))
    
    # Сохраняем HTML
    html = driver.page_source
    filename = f"../urls_perekrestok/page_{i}.html"
    with open(filename, "w", encoding="utf-8") as f:
        f.write(html)

    print(f"[+] Сохранена страница {i} → {filename}")
    
    final_cards = len(driver.find_elements(By.CSS_SELECTOR, ".product-card"))
    print(f"📊 Итого сохранено карточек: {final_cards}")
    
    # Случайная пауза между разными URL (чтобы не быть забаненным)
    if i < len(urls):
        sleep_time = random.uniform(3, 7)
        print(f"⏳ Ожидание {sleep_time:.1f} секунд перед следующим URL...")
        time.sleep(sleep_time)

print("\n✅ Все URL обработаны!")
driver.quit()
sys.exit(0)