from bs4 import BeautifulSoup
import json
import sys
from datetime import datetime

# множество для уникальности
products_set = set()
today = datetime.today().strftime("%Y-%m-%d")

for i in range(1, 26):
    with open(f"../urls/page_{i}.html", "r", encoding="utf-8") as file:
        html_doc = file.read()

    soup = BeautifulSoup(html_doc, "lxml")
    group = soup.find_all("div", class_="css-0")

    for obj in group:
        title = obj.find("p", class_="chakra-text mainInformation_title__ziiEa css-1jdqp4k")

        discount = obj.find("p", class_="chakra-text priceContainer_price__AY8C_ css-6uvdux")
        old_price = obj.find("p", class_="chakra-text priceContainer_catalogUsualPrice__m97fW css-bx74j9")

        if discount and old_price:
            # есть скидка → старая цена + новая цена
            item = (
                title.text.strip() if title else None,
                old_price.text.strip(),
                discount.text.strip()
            )
        else:
            # скидки нет → текущая цена из discount-тега
            current_price = discount.text.strip() if discount else None
            item = (
                title.text.strip() if title else None,
                current_price,
                None
            )

        products_set.add(item)

# преобразуем обратно в список словарей
products = []
for title, price, discount in products_set:
    item = {}
    if title:
        item["title"] = title
    if discount:  # если есть скидка
        item["price"] = price
        item["discount"] = discount
    else:  # если скидки нет
        item["price"] = price
    products.append(item)

data = {
    "date": today,
    "products": products
}

with open("../sensetive_res/products.json", "w", encoding="utf-8") as f:
    json.dump(data, f, ensure_ascii=False, indent=4)

sys.exit(0)
