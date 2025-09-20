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

    # название товара
    title_tag = obj.find("p", class_="chakra-text productListItem_title__2OiE4 css-xwxdk9")
    item["title"] = title_tag.get_text(strip=True) if title_tag else ""

    # блоки цен
    price_blocks = obj.find_all("div", class_="priceContainer_price__kqnwL")
    prices = []
    for block in price_blocks:
        parts = block.find_all("p")
        rub = parts[0].get_text(strip=True)
        kop = parts[1].get_text(strip=True) if len(parts) > 2 else "00"
        currency = parts[-1].get_text(strip=True)
        full_price = f"{rub}.{kop}{currency}"  # например "119.99₽"
        prices.append(full_price)

    # распределяем цены по полям
    if len(prices) == 2:
        item["old_price"] = prices[1]
    elif len(prices) >= 3:
        item["old_price"] = prices[2]
        item["discount_price"] = prices[1]

    products.append(item)

# формируем JSON
data = {
    "date": today,
    "products": products
}

json_data = json.dumps(data, ensure_ascii=False, indent=4)

with open("products.json", "w", encoding="utf-8") as f:
    f.write(json_data)

