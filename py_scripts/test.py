# test_driver.py - простая проверка драйвера
from selenium import webdriver

try:
    options = webdriver.ChromeOptions()
    options.add_argument("--no-sandbox")
    options.add_argument("--disable-dev-shm-usage")
    
    driver = webdriver.Chrome(options=options)
    driver.get("https://www.google.com")
    print("Драйвер работает корректно!")
    driver.quit()
except Exception as e:
    print(f"Ошибка драйвера: {e}")