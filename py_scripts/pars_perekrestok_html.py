from bs4 import BeautifulSoup
import json
import sys
import re
from datetime import datetime

def clean_price(price_text):
    """
    Очищает цену и оставляет только целую часть
    """
    if not price_text:
        return None
    
    # Убираем лишние слова и символы
    cleaned = price_text.replace('Цена', '').replace('Старая цена', '').replace('₽', '').replace('\xa0', ' ').strip()
    
    # Находим число в строке
    match = re.search(r'(\d+)[,.]?\d*', cleaned)
    if match:
        # Возвращаем только целую часть
        return match.group(1)
    
    # Если не нашли число, возвращаем очищенную строку
    return cleaned

products_set = set()
today = datetime.today().strftime("%Y-%m-%d")

for i in range(1, 31):
    with open(f"../urls_perekrestok/page_{i}.html", "r", encoding="utf-8") as file:
        html_doc = file.read()

    soup = BeautifulSoup(html_doc, "lxml")
    group = soup.find_all("div", class_="sc-eltbHq eRtzZT product-card")

    for obj in group:
        title = obj.find("a", class_="product-card__title product-card__title-link")
        
        price_new = obj.find("div", class_="price-new")
        old_price = obj.find("div", class_="price-old")

        if price_new and old_price:
            item = (
                title.text.strip() if title else None,
                clean_price(old_price.text.strip()),  # старую цену тоже очищаем
                clean_price(price_new.text.strip())   # новую цену очищаем
            )
        else:
            current_price = clean_price(price_new.text.strip()) if price_new else None
            item = (
                title.text.strip() if title else None,
                current_price,
                None
            )
        products_set.add(item)

products = []
for title, price, discount in products_set:
    item = {}
    if title:
        item["title"] = title
    if discount:  # если есть скидка
        item["price"] = price      # это старая цена
        item["discount"] = discount # это новая цена со скидкой
    else:  # если скидки нет
        item["price"] = price
    products.append(item)

data = {
    "date": today,
    "products": products
}

with open("../sensetive_res/perekrestok.json", "w", encoding="utf-8") as f:
    json.dump(data, f, ensure_ascii=False, indent=4)

print(f"Сохранено {len(products)} товаров")
sys.exit(0)