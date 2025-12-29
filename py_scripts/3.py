from bs4 import BeautifulSoup
import json
import sys
from datetime import datetime

products_set = set()

with open(f"../urls/page_1.html", "r", encoding="utf-8") as file:
    html_doc = file.read()

soup = BeautifulSoup(html_doc, "lxml")
group = soup.find_all("div", class_="css-6n4fw9")

count = 0

for obj in group:
    title = obj.find("p", class_="css-y50q0q")

    discount = obj.find("span", class_="css-1kmcjcs")
    old_price = obj.find("span", class_="css-8696l")

    if discount and old_price:
        print(discount.text, old_price.text)
        # есть скидка → старая цена + новая цена
        item = (
            title.text.strip() if title else None,
            old_price.text.strip(),
            discount.text.strip()
        )
    else:
        print(discount.text)
        # скидки нет → текущая цена из discount-тега
        current_price = discount.text.strip() if discount else None
        item = (
            title.text.strip() if title else None,
            current_price,
            None
        )

    products_set.add(item)
    print(item)