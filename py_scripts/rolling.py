from selenium import webdriver
from selenium.webdriver.chrome.options import Options
import time

# Настройка браузера в "безголовом" режиме
options = Options()
#options.add_argument('--headless')
options.add_argument('--disable-gpu')
options.add_argument('--no-sandbox')

# Запуск Chrome
driver = webdriver.Chrome(options=options)

try:
    url = "https://5ka.ru/catalog/cart/"
    driver.get(url)

    # Дадим время JavaScript подгрузить товары
    time.sleep(100)

    # Получаем итоговый HTML
    html = driver.page_source

    # Сохраняем в файл
    with open("../res/5ka_bakaleya.html", "w", encoding="utf-8") as f:
        f.write(html)

    print("HTML сохранён в 5ka_bakaleya.html")
finally:
    driver.quit()
