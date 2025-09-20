from bs4 import BeautifulSoup
import json
import sys
from datetime import datetime

for i in range(1, 23):
    with open(f"../urls/page_{i}.html", "r", encoding="utf-8") as file:
        html_doc = file.read()

    soup = BeautifulSoup(html_doc, "lxml")

    group = soup.find_all("div", class_="css-0")

    products = []

    today = datetime.today().strftime("%Y-%m-%d")

    for obj in group:
        item = {}
        title = obj.find("p", class_="chakra-text mainInformation_title__ziiEa css-1jdqp4k")
        price = obj.find("p", class_="chakra-text priceContainer_catalogUsualPrice__m97fW css-bx74j9")
        discount = obj.find("p", class_="chakra-text priceContainer_price__AY8C_ css-6uvdux")

        if title:
            item["title"] = title.text
        if price:
            item["price"] = price.text
        if discount:
            item["discount"] = discount.text

        if item:
            products.append(item)

    data = {
        "date": today,
        "products": products
    }

    json_data = json.dumps(data, ensure_ascii=False, indent=4)

    with open(f"../res/products_{i}.json", "w", encoding="utf-8") as f:
        f.write(json_data)

sys.exit(0)