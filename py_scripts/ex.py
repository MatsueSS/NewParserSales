from selenium import webdriver
from selenium.webdriver.chrome.service import Service as ChromeService
from webdriver_manager.chrome import ChromeDriverManager
from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC
import time
import random
import sys

urls = [
    "https://5ka.ru/search/?text=choco", #choco
    "https://5ka.ru/search/?text=%D1%81%D0%BF%D0%B0%D0%B3%D0%B5%D1%82%D1%82%D0%B8", #спагетти
    "https://5ka.ru/search/?text=%D1%80%D0%B8%D1%81", #рис
    "https://5ka.ru/search/?text=%D1%8F%D0%B1%D0%BB%D0%BE%D0%BA%D0%BE", #яблоко
    "https://5ka.ru/search/?text=%D0%B1%D0%B0%D0%BD%D0%B0%D0%BD%D1%8B", #бананы
    "https://5ka.ru/search/?text=%D1%87%D0%B0%D0%B9", #чай
    "https://5ka.ru/search/?text=%D0%A1%D1%82%D0%B8%D1%80%D0%B0%D0%BB%D1%8C%D0%BD%D1%8B%D0%B9%20%D0%BF%D0%BE%D1%80%D0%BE%D1%88%D0%BE%D0%BA", #стиральный порошок
    "https://5ka.ru/search/?text=%D0%BF%D1%8E%D1%80%D0%B5", #пюре
    "https://5ka.ru/search/?text=%D0%BA%D1%83%D0%BA%D1%83%D1%80%D1%83%D0%B7%D0%B0", #кукуруза
    "https://5ka.ru/search/?text=%D0%BA%D1%80%D0%B5%D0%BA%D0%B5%D1%80", #крекер
    "https://5ka.ru/search/?text=%D0%BC%D0%BE%D0%BB%D0%BE%D0%BA%D0%BE", #молоко
    "https://5ka.ru/search/?text=%D0%B3%D0%BE%D1%80%D0%BE%D1%88%D0%B5%D0%BA", #горошек
    "https://5ka.ru/search/?text=%D0%BC%D1%8E%D1%81%D0%BB%D0%B8", #мюсли
    "https://5ka.ru/search/?text=%D1%81%D1%80%D0%B5%D0%B4%D1%81%D1%82%D0%B2%D0%BE%20%D0%B4%D0%BB%D1%8F%20%D0%BC%D1%8B%D1%82%D1%8C%D1%8F%20%D0%BF%D0%BE%D1%81%D1%83%D0%B4%D1%8B", #средство для мытья посуды
    "https://5ka.ru/search/?text=%D1%81%D0%BC%D0%B5%D1%81%D1%8C", #смесь
    "https://5ka.ru/search/?text=%D1%81%D1%8B%D1%80", #сыр
    "https://5ka.ru/search/?text=%D0%99%D0%BE%D0%B3%D1%83%D1%80%D1%82", #йогурт
    "https://5ka.ru/search/?text=%D0%B2%D0%B0%D1%80%D0%B5%D0%BD%D0%B8%D0%BA%D0%B8", #вареники
    "https://5ka.ru/search/?text=%D1%82%D0%B2%D0%BE%D1%80%D0%BE%D0%B3", #творог
    "https://5ka.ru/search/?text=%D0%BF%D0%B5%D1%87%D0%B5%D0%BD%D1%8C%D0%B5", #печенье
    "https://5ka.ru/search/?text=%D1%85%D0%BB%D0%BE%D0%BF%D1%8C%D1%8F", #хлопья
    "https://5ka.ru/search/?text=%D1%8F%D0%B9%D1%86%D0%B0", #яйца
    "https://5ka.ru/search/?text=%D0%B7%D1%83%D0%B1%D0%BD%D0%B0%D1%8F%20%D0%BF%D0%B0%D1%81%D1%82%D0%B0", #зубная паста
    "http://5ka.ru/search/?text=%D0%BE%D0%B3%D1%83%D1%80%D1%86%D1%8B", #огурцы
    "https://5ka.ru/search/?text=%D1%82%D1%83%D1%88%D1%91%D0%BD%D0%BA%D0%B0", #тушёнка
    "https://5ka.ru/search/?text=%D0%BC%D0%B0%D0%BD%D0%B4%D0%B0%D1%80%D0%B8%D0%BD%D1%8B", #мандарины
    "https://5ka.ru/catalog/sgushchyonka--251C13168/", #Сгущёнка
    "https://5ka.ru/search/?text=%D0%9C%D0%B0%D1%81%D0%BB%D0%BE%20%D1%81%D0%BB%D0%B8%D0%B2%D0%BE%D1%87%D0%BD%D0%BE%D0%B5", #масло сливочное
    "https://5ka.ru/search/?text=%D0%BB%D0%B8%D0%BC%D0%BE%D0%BD%D1%8B", #лимоны
    "https://5ka.ru/search/?text=%D0%BF%D0%BE%D0%BC%D0%B8%D0%B4%D0%BE%D1%80%D1%8B", #помидоры
    "https://5ka.ru/search/?text=%D0%A1%D1%83%D0%BF%D0%B5%D1%80%D1%84%D1%83%D0%B4%D1%8B", #суперфуды
    "https://5ka.ru/search/?text=%D0%9F%D1%80%D0%BE%D1%82%D0%B5%D0%B8%D0%BD%D0%BE%D0%B2%D1%8B%D0%B5%20%D0%B1%D0%B0%D1%82%D0%BE%D0%BD%D1%87%D0%B8%D0%BA%D0%B8%20%D0%B1%D0%B5%D0%B7%20%D1%81%D0%B0%D1%85%D0%B0%D1%80%D0%B0", #протеиновые батончики без сахара
    "https://5ka.ru/search/?text=%D0%9E%D1%80%D0%B5%D1%85%D0%BE%D0%B2%D1%8B%D0%B5%20%D1%81%D0%BC%D0%B5%D1%81%D0%B8", #Ореховые смеси
    "https://5ka.ru/search/?text=%D0%A1%D1%83%D1%85%D0%B8%D0%B5%20%D0%B7%D0%B0%D0%B2%D1%82%D1%80%D0%B0%D0%BA%D0%B8", #Сухие завтраки
    "https://5ka.ru/search/?text=%D0%A0%D0%B0%D1%81%D1%82%D0%B8%D1%82%D0%B5%D0%BB%D1%8C%D0%BD%D0%BE%D0%B5%20%D0%BC%D0%B0%D1%81%D0%BB%D0%BE", #Растительное масло
    "https://5ka.ru/search/?text=%D1%81%D1%83%D1%85%D0%BE%D1%84%D1%80%D1%83%D0%BA%D1%82%D1%8B", #сухофрукты
    "https://5ka.ru/search/?text=%D0%A1%D0%B5%D0%BC%D0%B5%D1%87%D0%BA%D0%B8%20%D1%82%D1%8B%D0%BA%D0%B2%D0%B5%D0%BD%D0%BD%D1%8B%D0%B5", #Семечки тыквенные
    "https://5ka.ru/search/?text=%D0%A1%D1%8B%D1%80", #Сыры
    "https://5ka.ru/search/?text=%D0%9E%D0%BB%D0%B8%D0%B2%D0%BA%D0%BE%D0%B2%D0%BE%D0%B5%20%D0%BC%D0%B0%D1%81%D0%BB%D0%BE", #Оливковое масло
    "https://5ka.ru/search/?text=%D0%A0%D0%B0%D1%81%D1%82%D0%B2%D0%BE%D1%80%D0%B8%D0%BC%D1%8B%D0%B9%20%D0%BA%D0%BE%D1%84%D0%B5%20%D1%81%D1%83%D0%B1%D0%BB%D0%B8%D0%BC%D0%B8%D1%80%D0%BE%D0%B2%D0%B0%D0%BD%D0%BD%D1%8B%D0%B9", #Растворимый кофе сублимированный
    "https://5ka.ru/search/?text=%D0%97%D0%B5%D0%BB%D0%B5%D0%BD%D1%8B%D0%B9%20%D1%87%D0%B0%D0%B9", #Зеленый чай
    "https://5ka.ru/search/?text=%D0%9A%D0%B0%D0%BA%D0%B0%D0%BE", #Какао
    "https://5ka.ru/search/?text=%D0%A8%D0%BE%D0%BA%D0%BE%D0%BB%D0%B0%D0%B4%20%D0%B3%D0%BE%D1%80%D1%8C%D0%BA%D0%B8%D0%B9%2075%", #Шоколад горький 75%
    "https://5ka.ru/search/?text=%D0%9C%D0%BE%D0%BB%D0%BE%D1%87%D0%BD%D1%8B%D0%B9%20%D1%88%D0%BE%D0%BA%D0%BE%D0%BB%D0%B0%D0%B4", #Молочный шоколад 
    "https://5ka.ru/search/?text=%D0%9C%D0%B0%D1%80%D0%BC%D0%B5%D0%BB%D0%B0%D0%B4%20%D0%B6%D0%B5%D0%B2%D0%B0%D1%82%D0%B5%D0%BB%D1%8C%D0%BD%D1%8B%D0%B9", #Мармелад жевательный
    "https://5ka.ru/search/?text=%D0%9F%D0%B5%D1%87%D0%B5%D0%BD%D1%8C%D0%B5%20%D0%BE%D0%B2%D1%81%D1%8F%D0%BD%D0%BE%D0%B5", #Печенье овсяное
    "https://5ka.ru/search/?text=%D0%92%D0%B0%D1%84%D0%BB%D0%B8", #Вафли
    "https://5ka.ru/search/?text=%D0%A1%D0%B3%D1%83%D1%89%D0%B5%D0%BD%D0%BD%D0%BE%D0%B5%20%D0%BC%D0%BE%D0%BB%D0%BE%D0%BA%D0%BE", #Сгущенное молоко
    "https://5ka.ru/search/?text=%D0%9C%D0%B8%D0%BD%D0%B5%D1%80%D0%B0%D0%BB%D1%8C%D0%BD%D0%B0%D1%8F%20%D0%B2%D0%BE%D0%B4%D0%B0", #Минеральная вода
    "https://5ka.ru/search/?text=%D0%A1%D0%BE%D1%83%D1%81%20%D0%BF%D0%B5%D1%81%D1%82%D0%BE", #Соус песто
    "https://5ka.ru/search/?text=%D0%A2%D0%BE%D0%BC%D0%B0%D1%82%D0%BD%D0%B0%D1%8F%20%D0%BF%D0%B0%D1%81%D1%82%D0%B0", #Томатная паста
    "https://5ka.ru/search/?text=%D0%A1%D0%BE%D0%B5%D0%B2%D1%8B%D0%B9%20%D1%81%D0%BE%D1%83%D1%81", #Соевый соус
    "https://5ka.ru/search/?text=%D0%A3%D0%BA%D1%81%D1%83%D1%81%20%D0%B1%D0%B0%D0%BB%D1%8C%D0%B7%D0%B0%D0%BC%D0%B8%D1%87%D0%B5%D1%81%D0%BA%D0%B8%D0%B9", #Уксус бальзамический
    "https://5ka.ru/search/?text=%D0%A0%D0%B8%D1%81%20%D0%B1%D0%B0%D1%81%D0%BC%D0%B0%D1%82%D0%B8", #Рис басмати
    "https://5ka.ru/search/?text=%D0%BC%D1%83%D0%BA%D0%B0", #Мука
    "https://5ka.ru/search/?text=%D0%BA%D1%80%D1%83%D0%BF%D0%B0", #Крупа
    "https://5ka.ru/search/?text=%D0%BA%D0%B8%D0%BD%D0%BE%D0%B0", #киноа
    "https://5ka.ru/search/?text=%D0%B3%D1%80%D0%B5%D1%87%D0%BA%D0%B0", #гречка
    "https://5ka.ru/search/?text=%D0%94%D0%B6%D0%B5%D0%BC%D1%8B%20%D0%B8%20%D0%BA%D0%BE%D0%BD%D1%84%D0%B8%D1%82%D1%8E%D1%80%D1%8B", #Джемы и конфитюры
    "https://5ka.ru/search/?text=%D0%92%D0%B0%D1%80%D0%B5%D0%BD%D1%8C%D0%B5", #Варенье
    "https://5ka.ru/search/?text=%D0%9F%D0%B0%D1%88%D1%82%D0%B5%D1%82", #Паштет
    "https://5ka.ru/search/?text=%D0%A0%D1%8B%D0%B1%D0%BD%D1%8B%D0%B5%20%D0%BA%D0%BE%D0%BD%D1%81%D0%B5%D1%80%D0%B2%D1%8B", #Рыбные консервы
    "https://5ka.ru/search/?text=%D0%98%D0%BA%D1%80%D0%B0", #Икра
    "https://5ka.ru/search/?text=%D0%9A%D1%80%D0%B0%D0%B1%D0%BE%D0%B2%D0%BE%D0%B5%20%D0%BC%D1%8F%D1%81%D0%BE", #Крабовое мясо
    "https://5ka.ru/search/?text=%D0%9F%D0%B5%D0%BB%D1%8C%D0%BC%D0%B5%D0%BD%D0%B8/%D0%B2%D0%B0%D1%80%D0%B5%D0%BD%D0%B8%D0%BA%D0%B8", #Пельмени/вареники
    "https://5ka.ru/search/?text=%D0%97%D0%B0%D0%BC%D0%BE%D1%80%D0%BE%D0%B6%D0%B5%D0%BD%D0%BD%D1%8B%D0%B5%20%D0%BE%D0%B2%D0%BE%D1%89%D0%B8", #Замороженные овощи
    "https://5ka.ru/search/?text=%D0%9F%D0%B8%D1%86%D1%86%D0%B0%20%D0%B7%D0%B0%D0%BC%D0%BE%D1%80%D0%BE%D0%B6%D0%B5%D0%BD%D0%BD%D0%B0%D1%8F", #Пицца замороженная
    "https://5ka.ru/search/?text=%D0%9A%D1%83%D1%80%D0%B8%D0%BD%D0%BE%D0%B5%20%D1%84%D0%B8%D0%BB%D0%B5", #Куриное филе
    "https://5ka.ru/search/?text=%D0%A4%D0%BE%D1%80%D0%B5%D0%BB%D1%8C/%D0%A1%D0%B5%D0%BC%D0%B3%D0%B0", #Форель/Семга
    "https://5ka.ru/search/?text=%D0%9A%D1%80%D0%B5%D0%B2%D0%B5%D1%82%D0%BA%D0%B8%20%D0%BA%D0%BE%D1%80%D0%BE%D0%BB%D0%B5%D0%B2%D1%81%D0%BA%D0%B8%D0%B5", #Креветки королевские
    "https://5ka.ru/search/?text=%D0%BA%D0%B0%D0%BB%D1%8C%D0%BC%D0%B0%D1%80", #Кальмары
    "https://5ka.ru/search/?text=%D0%98%D0%BD%D0%B4%D0%B5%D0%B9%D0%BA%D0%B0", #Индейка
    "https://5ka.ru/search/?text=%D0%9F%D0%B5%D1%87%D0%B5%D0%BD%D1%8C%20%D1%82%D1%80%D0%B5%D1%81%D0%BA%D0%B8", #Печень трески
    "https://5ka.ru/search/?text=%D0%9A%D1%83%D1%80%D0%B8%D0%BD%D0%B0%D1%8F%20%D0%BF%D0%B5%D1%87%D0%B5%D0%BD%D1%8C", #Куриная печень
    "https://5ka.ru/search/?text=%D0%93%D0%BE%D0%B2%D1%8F%D0%B6%D0%B8%D0%B9%20%D1%8F%D0%B7%D1%8B%D0%BA", #Говяжий язык
    "https://5ka.ru/search/?text=%D0%A1%D0%B5%D0%BB%D1%8C%D0%B4%D1%8C", #Сельдь
    "https://5ka.ru/search/?text=%D0%A1%D0%BA%D1%83%D0%BC%D0%B1%D1%80%D0%B8%D1%8F", #Скумбрия
    "https://5ka.ru/search/?text=%D0%A3%D1%82%D0%B8%D0%BD%D0%B0%D1%8F%20%D0%B3%D1%80%D1%83%D0%B4%D0%BA%D0%B0", #Утиная грудка
    "https://5ka.ru/search/?text=%D0%A2%D0%B2%D0%BE%D1%80%D0%BE%D0%B3%20%D0%BE%D0%B1%D0%B5%D0%B7%D0%B6%D0%B8%D1%80%D0%B5%D0%BD%D0%BD%D1%8B%D0%B9", #Творог обезжиренный
    "https://5ka.ru/search/?text=%D0%A2%D0%B2%D0%BE%D1%80%D0%BE%D0%B3%209%", #Творог 9%
    "https://5ka.ru/search/?text=%D0%A2%D0%B2%D0%BE%D1%80%D0%BE%D0%B3%20%D0%B7%D0%B5%D1%80%D0%BD%D0%B5%D0%BD%D1%8B%D0%B9", #Творог зерненый
    "https://5ka.ru/search/?text=%D0%A1%D0%BC%D0%B5%D1%82%D0%B0%D0%BD%D0%B0", #Сметана
    "https://5ka.ru/search/?text=%D0%9A%D0%B5%D1%84%D0%B8%D1%80", #Кефир
    "https://5ka.ru/search/?text=%D0%A0%D1%8F%D0%B6%D0%B5%D0%BD%D0%BA%D0%B0", #Ряженка
    "https://5ka.ru/search/?text=%D0%99%D0%BE%D0%B3%D1%83%D1%80%D1%82%20%D0%B3%D1%80%D0%B5%D1%87%D0%B5%D1%81%D0%BA%D0%B8%D0%B9", #Йогурт греческий
    "https://5ka.ru/search/?text=%D0%9C%D0%B0%D1%81%D0%BB%D0%BE%20%D1%81%D0%BB%D0%B8%D0%B2%D0%BE%D1%87%D0%BD%D0%BE%D0%B5", #Масло сливочное
    "https://5ka.ru/search/?text=%D0%A1%D1%8B%D1%80%20%D0%BC%D0%BE%D1%86%D0%B0%D1%80%D0%B5%D0%BB%D0%BB%D0%B0", #Сыр моцарелла
    "https://5ka.ru/search/?text=%D0%A1%D1%8B%D1%80%20%D0%BC%D0%B0%D0%B0%D1%81%D0%B4%D0%B0%D0%BC/%D0%B3%D0%B0%D1%83%D0%B4%D0%B0", #Сыр маасдам/гауда
    "https://5ka.ru/search/?text=%D0%A1%D1%8B%D1%80%20%D1%81%20%D0%BF%D0%BB%D0%B5%D1%81%D0%B5%D0%BD%D1%8C%D1%8E", #Сыр с плесенью
    "https://5ka.ru/search/?text=%D0%AF%D0%B9%D1%86%D0%B0%20%D0%BA%D1%83%D1%80%D0%B8%D0%BD%D1%8B%D0%B5%20C0", #Яйца куриные C0
    "https://5ka.ru/search/?text=%D1%8F%D0%B9%D1%86%D0%B0%20%D0%A11", #Яйца куриные C1
    "https://5ka.ru/search/?text=%D1%8F%D0%B9%D1%86%D0%B0%20%D0%A12", #Яйца куриные C2
    "https://5ka.ru/search/?text=%D0%AF%D0%B9%D1%86%D0%B0%20%D0%BF%D0%B5%D1%80%D0%B5%D0%BF%D0%B5%D0%BB%D0%B8%D0%BD%D1%8B%D0%B5", #Яйца перепелиные
    "https://5ka.ru/search/?text=%D0%A1%D0%BB%D0%B8%D0%B2%D0%BA%D0%B8", #Сливки
    "https://5ka.ru/search/?text=%D0%A2%D0%BE%D0%BF%D0%BB%D0%B5%D0%BD%D0%BE%D0%B5%20%D0%BC%D0%B0%D1%81%D0%BB%D0%BE", #Топленое масло
    "https://5ka.ru/search/?text=%D0%90%D0%B2%D0%BE%D0%BA%D0%B0%D0%B4%D0%BE", #Авокадо
    "https://5ka.ru/search/?text=%D0%AF%D0%B3%D0%BE%D0%B4%D1%8B%20%D0%B7%D0%B0%D0%BC%D0%BE%D1%80%D0%BE%D0%B6%D0%B5%D0%BD%D0%BD%D1%8B%D0%B5", #Ягоды замороженные
    "https://5ka.ru/search/?text=%D0%93%D1%80%D0%B0%D0%BD%D0%B0%D1%82%D1%8B", #Гранаты
    "https://5ka.ru/search/?text=%D0%A6%D0%B8%D1%82%D1%80%D1%83%D1%81%D0%BE%D0%B2%D1%8B%D0%B5", #Цитрусовые
    "https://5ka.ru/search/?text=%D0%98%D0%BC%D0%B1%D0%B8%D1%80%D1%8C", #Имбирь
    "https://5ka.ru/search/?text=%D0%A7%D0%B5%D1%81%D0%BD%D0%BE%D0%BA", #Чеснок
    "https://5ka.ru/search/?text=%D0%91%D1%80%D0%BE%D0%BA%D0%BA%D0%BE%D0%BB%D0%B8", #Брокколи
    "https://5ka.ru/search/?text=%D0%A8%D0%BF%D0%B8%D0%BD%D0%B0%D1%82", #Шпинат
    "https://5ka.ru/search/?text=%D0%A2%D0%BE%D0%BC%D0%B0%D1%82%D1%8B%20%D1%87%D0%B5%D1%80%D1%80%D0%B8", #Томаты черри
    "https://5ka.ru/search/?text=%D0%9E%D0%B3%D1%83%D1%80%D1%86%D1%8B%20%D0%B4%D0%BB%D0%B8%D0%BD%D0%BD%D0%BE%D0%BF%D0%BB%D0%BE%D0%B4%D0%BD%D1%8B%D0%B5", #Огурцы длинноплодные
    "https://5ka.ru/search/?text=%D0%9F%D0%B5%D1%80%D0%B5%D1%86%20%D0%B1%D0%BE%D0%BB%D0%B3%D0%B0%D1%80%D1%81%D0%BA%D0%B8%D0%B9", #Перец болгарский
    "https://5ka.ru/search/?text=%D0%9A%D0%B0%D1%80%D1%82%D0%BE%D1%84%D0%B5%D0%BB%D1%8C", #Картофель
    "https://5ka.ru/search/?text=%D0%9C%D0%BE%D1%80%D0%BA%D0%BE%D0%B2%D1%8C", #Морковь
    "https://5ka.ru/search/?text=%D0%A2%D1%8B%D0%BA%D0%B2%D0%B0", #Тыква
    "https://5ka.ru/search/?text=%D0%9A%D0%B0%D0%B1%D0%B0%D1%87%D0%BA%D0%B8/%D0%A6%D1%83%D0%BA%D0%B8%D0%BD%D0%B8", #Кабачки/Цукини
    "https://5ka.ru/search/?text=%D0%97%D0%B5%D0%BB%D0%B5%D0%BD%D1%8C", #Зелень
    "https://5ka.ru/search/?text=%D0%93%D1%80%D0%B5%D1%86%D0%BA%D0%B8%D0%B5%20%D0%BE%D1%80%D0%B5%D1%85%D0%B8", #Грецкие орехи
    "https://5ka.ru/search/?text=%D0%9C%D0%B8%D0%BD%D0%B4%D0%B0%D0%BB%D1%8C", #Миндаль
    "https://5ka.ru/search/?text=%D0%A4%D1%83%D0%BD%D0%B4%D1%83%D0%BA", #Фундук
    "https://5ka.ru/search/?text=%D0%9A%D0%B5%D1%88%D1%8C%D1%8E", #Кешью
    "https://5ka.ru/search/?text=%D0%A4%D0%B8%D1%81%D1%82%D0%B0%D1%88%D0%BA%D0%B8", #Фисташки
    "https://5ka.ru/search/?text=%D0%91%D1%80%D0%B0%D0%B7%D0%B8%D0%BB%D1%8C%D1%81%D0%BA%D0%B8%D0%B9%20%D0%BE%D1%80%D0%B5%D1%85", #Бразильский орех
    "https://5ka.ru/search/?text=%D0%9A%D0%B5%D0%B4%D1%80%D0%BE%D0%B2%D1%8B%D0%B5%20%D0%BE%D1%80%D0%B5%D1%85%D0%B8", #Кедровые орехи
    "https://5ka.ru/search/?text=%D0%A1%D0%B5%D0%BC%D0%B5%D0%BD%D0%B0%20%D1%87%D0%B8%D0%B0", #Семена чиа
    "https://5ka.ru/search/?text=%D0%A1%D0%B5%D0%BC%D0%B5%D0%BD%D0%B0%20%D0%BB%D1%8C%D0%BD%D0%B0", #Семена льна
    "https://5ka.ru/search/?text=%D0%9A%D1%83%D0%BD%D0%B6%D1%83%D1%82", #Кунжут
    "https://5ka.ru/search/?text=%D0%9C%D0%B0%D0%BA", #Мак
    "https://5ka.ru/search/?text=%D0%A1%D0%B5%D0%BC%D0%B5%D0%BD%D0%B0%20%D0%BA%D0%BE%D0%BD%D0%BE%D0%BF%D0%BB%D0%B8", #Семена конопли
    "https://5ka.ru/search/?text=%D0%A7%D0%B5%D1%87%D0%B5%D0%B2%D0%B8%D1%86%D0%B0", #Чечевица
    "https://5ka.ru/search/?text=%D0%9D%D1%83%D1%82", #Нут
    "https://5ka.ru/search/?text=%D0%A4%D0%B0%D1%81%D0%BE%D0%BB%D1%8C", #Фасоль
    "https://5ka.ru/search/?text=%D0%9C%D1%83%D0%BA%D0%B0%20%D1%86%D0%B5%D0%BB%D1%8C%D0%BD%D0%BE%D0%B7%D0%B5%D1%80%D0%BD%D0%BE%D0%B2%D0%B0%D1%8F", #Мука цельнозерновая
    "https://5ka.ru/search/?text=%D0%9E%D1%82%D1%80%D1%83%D0%B1%D0%B8", #Отруби
    "https://5ka.ru/search/?text=%D0%9A%D0%B8%D0%BD%D0%BE%D0%B0", #Киноа
    "https://5ka.ru/search/?text=%D0%91%D1%83%D0%BB%D0%B3%D1%83%D1%80", #Булгур
    "https://5ka.ru/search/?text=%D0%A5%D0%BB%D0%B5%D0%B1%D1%86%D1%8B", #Хлебцы
    "https://5ka.ru/search/?text=%D0%9F%D0%B0%D1%81%D1%82%D0%B0%20%D0%B8%D0%B7%20%D1%82%D0%B2%D0%B5%D1%80%D0%B4%D1%8B%D1%85%20%D1%81%D0%BE%D1%80%D1%82%D0%BE%D0%B2", #Паста из твердых сортов
    "https://5ka.ru/search/?text=%D0%9B%D0%B0%D0%BF%D1%88%D0%B0%20%D1%80%D0%B8%D1%81%D0%BE%D0%B2%D0%B0%D1%8F/%D0%B3%D1%80%D0%B5%D1%87%D0%BD%D0%B5%D0%B2%D0%B0%D1%8F", #Лапша рисовая/гречневая
    "https://5ka.ru/search/?text=%D0%A5%D0%B0%D0%BB%D0%B2%D0%B0", #Халва
    "https://5ka.ru/search/?text=%D0%9A%D0%BE%D0%B7%D0%B8%D0%BD%D0%B0%D0%BA%D0%B8", #Козинаки
    "https://5ka.ru/search/?text=%D0%A8%D0%BE%D0%BA%D0%BE%D0%BB%D0%B0%D0%B4%D0%BD%D0%B0%D1%8F%20%D0%BF%D0%B0%D1%81%D1%82%D0%B0", #Шоколадная паста
    "https://5ka.ru/search/?text=%D0%9C%D0%B5%D0%B4", #Мед
    "https://5ka.ru/search/?text=%D0%A4%D1%80%D1%83%D0%BA%D1%82%D0%BE%D0%B2%D1%8B%D0%B5%20%D1%87%D0%B8%D0%BF%D1%81%D1%8B", #Фруктовые чипсы
    "https://5ka.ru/search/?text=%D0%9C%D0%BE%D1%80%D0%BE%D0%B6%D0%B5%D0%BD%D0%BE%D0%B5", #Мороженое
    "https://5ka.ru/search/?text=%D0%9F%D1%80%D1%8F%D0%BD%D0%B8%D0%BA%D0%B8", #Пряники
    "https://5ka.ru/search/?text=%D0%A1%D1%83%D1%85%D0%B0%D1%80%D0%B8%D0%BA%D0%B8/%D0%93%D1%80%D0%B5%D0%BD%D0%BA%D0%B8", #Сухарики/Гренки
    "https://5ka.ru/search/?text=%D0%A7%D0%B0%D0%B9%20%D1%87%D0%B5%D1%80%D0%BD%D1%8B%D0%B9", #Чай черный
    "https://5ka.ru/search/?text=%D0%BA%D0%B0%D1%80%D0%BA%D0%B0%D0%B4%D0%B5", #каркаде
    "https://5ka.ru/search/?text=%D0%A6%D0%B8%D0%BA%D0%BE%D1%80%D0%B8%D0%B9%20%D1%80%D0%B0%D1%81%D1%82%D0%B2%D0%BE%D1%80%D0%B8%D0%BC%D1%8B%D0%B9", #Цикорий растворимый
    "https://5ka.ru/search/?text=%D0%9A%D0%BE%D0%BC%D0%BF%D0%BE%D1%82%D1%8B/%D0%A1%D0%BE%D0%BA%D0%B8", #Компоты/Соки
    "https://5ka.ru/search/?text=%D0%9A%D0%B2%D0%B0%D1%81", #Квас
    "https://5ka.ru/search/?text=%D0%92%D0%BE%D0%B4%D0%B0%20%D0%BF%D0%B8%D1%82%D1%8C%D0%B5%D0%B2%D0%B0%D1%8F", #Вода питьевая
    "https://5ka.ru/search/?text=%D0%A1%D0%BE%D0%BB%D1%8C", #Соль
    "https://5ka.ru/search/?text=%D0%A1%D0%B0%D1%85%D0%B0%D1%80", #Сахар
    "https://5ka.ru/search/?text=%D0%9F%D0%BE%D0%B4%D1%81%D0%BE%D0%BB%D0%BD%D0%B5%D1%87%D0%BD%D0%BE%D0%B5%20%D0%BC%D0%B0%D1%81%D0%BB%D0%BE", #Подсолнечное масло
    "https://5ka.ru/search/?text=%D0%9A%D0%B5%D1%82%D1%87%D1%83%D0%BF", #Кетчуп
    "https://5ka.ru/search/?text=%D0%9C%D0%B0%D0%B9%D0%BE%D0%BD%D0%B5%D0%B7", #Майонез
    "https://5ka.ru/search/?text=%D0%93%D0%BE%D1%80%D1%87%D0%B8%D1%86%D0%B0", #Горчица
    "https://5ka.ru/search/?text=%D0%A5%D1%80%D0%B5%D0%BD", #Хрен
    "https://5ka.ru/search/?text=%D0%90%D0%B4%D0%B6%D0%B8%D0%BA%D0%B0", #Аджика
    "https://5ka.ru/search/?text=%D0%A2%D0%BE%D0%BC%D0%B0%D1%82%D0%BD%D0%B0%D1%8F%20%D0%BF%D0%B0%D1%81%D1%82%D0%B0", #Томатная паста
    "https://5ka.ru/search/?text=%D0%9B%D0%B0%D0%B2%D1%80%D0%BE%D0%B2%D1%8B%D0%B9%20%D0%BB%D0%B8%D1%81%D1%82/%D0%9F%D0%B5%D1%80%D0%B5%D1%86%20%D0%B3%D0%BE%D1%80%D0%BE%D1%88%D0%B5%D0%BA", #Лавровый лист/Перец горошек
    "https://5ka.ru/search/?text=%D0%BF%D1%80%D0%BE%D1%81%D1%82%D0%BE%D0%BA%D0%B2%D0%B0%D1%88%D0%B8%D0%BD%D0%BE%20%D0%B9%D0%BE%D0%B3%D1%83%D1%80%D1%82", #простоквашино йогурт
    "https://5ka.ru/search/?text=%D0%BF%D1%80%D0%BE%D1%81%D1%82%D0%BE%D0%BA%D0%B2%D0%B0%D1%88%D0%B8%D0%BD%D0%BE%20%D1%82%D0%B2%D0%BE%D1%80%D0%BE%D0%B6%D0%BE%D0%BA", #простоквашино творожок    
]

options = webdriver.ChromeOptions()
options.add_argument('--disable-blink-features=AutomationControlled')
options.add_experimental_option("excludeSwitches", ["enable-automation"])
options.add_experimental_option('useAutomationExtension', False)

# Другие полезные опции
options.add_argument("--no-sandbox")
options.add_argument("--disable-dev-shm-usage")
options.add_argument("--disable-extensions")
options.add_argument("--disable-plugins")
options.add_argument("--disable-images")
# Осторожно с отключением JavaScript - может сломать сайт
# options.add_argument("--disable-javascript")

# Случайный User-Agent
user_agents = [
    "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36",
    "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36",
    "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36"
]
options.add_argument(f'--user-agent={random.choice(user_agents)}')

# Если нужно, раскомментируйте headless
# options.add_argument("--headless")

try:
    service = ChromeService(ChromeDriverManager().install())
    driver = webdriver.Chrome(service=service, options=options)
    print("[✓] ChromeDriver успешно установлен и запущен")
except Exception as e:
    print(f"[!] Ошибка при запуске ChromeDriver: {e}")
    sys.exit(1)

def scroll_to_bottom(driver, pause_time=1, max_wait=20):
    """
    Прокручивает страницу до конца, пока подгружаются новые элементы.
    Ждёт появления новых карточек товаров.
    """
    last_height = driver.execute_script("return document.body.scrollHeight")
    start_time = time.time()

    while True:
        driver.execute_script("window.scrollTo(0, document.body.scrollHeight);")
        time.sleep(pause_time)

        # ждём появления новых карточек (ожидаем, что их станет больше)
        try:
            WebDriverWait(driver, 10).until(
                lambda d: d.execute_script("return document.body.scrollHeight") > last_height
            )
            last_height = driver.execute_script("return document.body.scrollHeight")
            start_time = time.time()  # сбрасываем таймер ожидания
        except:
            # если долго нет изменений — выходим
            if time.time() - start_time > max_wait:
                print("[!] Достигнут конец страницы.")
                break

for i, url in enumerate(urls, start=1):
    print(f"[→] Открываю: {url}")
    driver.get(url)

    # ждём начальной загрузки карточек
    try:
        WebDriverWait(driver, 30).until(
            EC.presence_of_element_located((By.CSS_SELECTOR, ".css-0"))
        )
    except Exception as e:
        print(f"[!] Не удалось дождаться карточек на {url}: {e}")
        continue

    # плавная прокрутка
    scroll_to_bottom(driver, pause_time=1, max_wait=20)

    # сохраняем HTML
    html = driver.page_source
    filename = f"../urls/page_{i}.html"
    with open(filename, "w", encoding="utf-8") as f:
        f.write(html)

    print(f"[+] Сохранена страница {url} → {filename}")

driver.quit()
sys.exit(0)
