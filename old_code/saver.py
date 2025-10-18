from bs4 import BeautifulSoup
import json
import sys
from datetime import datetime

for i in range(1, 23):
    with open(f"../urls/page_{i}.html", "r", encoding="utf-8") as file:
        html_doc = file.read()

    soup = BeautifulSoup(html_doc, "lxml")
    group = soup.find_all("div", class_="css-0")

    # используем множество для уникальности
    products_set = set()
    today = datetime.today().strftime("%Y-%m-%d")

    for obj in group:
        title = obj.find("p", class_="chakra-text mainInformation_title__ziiEa css-1jdqp4k")
        price = obj.find("p", class_="chakra-text priceContainer_catalogUsualPrice__m97fW css-bx74j9")
        discount = obj.find("p", class_="chakra-text priceContainer_price__AY8C_ css-6uvdux")

        # кортежи удобнее хранить в set, так как они хешируемые
        item = (
            title.text if title else None,
            price.text if price else None,
            discount.text if discount else None
        )

        products_set.add(item)

    # преобразуем обратно в список словарей
    products = []
    for title, price, discount in products_set:
        item = {}
        if title:
            item["title"] = title
        if price:
            item["price"] = price
        if discount:
            item["discount"] = discount
        products.append(item)

    data = {
        "date": today,
        "products": products
    }

