from bs4 import BeautifulSoup
import json
from datetime import datetime

with open("../res/5ka_bakaleya.html", "r", encoding="utf-8") as file:
    html_doc = file.read()

soup = BeautifulSoup(html_doc, "lxml")

group = soup.find_all("div", class_="chakra-stack productListItem_body__M4QKD css-tl3ftk")

products = []

today = datetime.today().strftime("%Y-%m-%d")

for obj in group:
    item = {}
    text = obj.find("p", class_="chakra-text productListItem_title__2OiE4 css-xwxdk9")
    price = obj.find("p", class_="chakra-text priceContainer_color__R0Wjs css-1etcqld")
    if(text and price):
        print(text.text, price.text)
        item["text"] = text.text
        item["price"] = price.text
        products.append(item)

data = {
    "date": today,
    "products": products
}

json_data = json.dumps(data, ensure_ascii=False, indent=4)

with open("products.json", "w", encoding="utf-8") as f:
    f.write(json_data)

