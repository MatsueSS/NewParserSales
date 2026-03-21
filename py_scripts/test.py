from bs4 import BeautifulSoup
import json
import sys
from datetime import datetime

with open(f"../urls/page_3.html", "r", encoding="utf-8") as file:
    html_doc = file.read()

soup = BeautifulSoup(html_doc, "lxml")
group = soup.find_all("div", class_="css-6n4fw9")

for obj in group:
    title = obj.find("p", class_="css-y50q0q")
    
    discount = obj.find("span", class_="css-8696l")
    old_price = obj.find("span", class_="css-gv6vdh")

    print(old_price)
    print(discount)
