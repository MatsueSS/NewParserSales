#!/usr/bin/env python3
# batch_copy.py

import subprocess
import time
import json
import os
from datetime import datetime

# Список URL для парсинга
URLS = [
    {"url": "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C13057/products?mode=delivery&include_restrict=true&limit=350&offset=0", "name": "чай_кофе_какао"},
    {"url": "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C13055/products?mode=delivery&include_restrict=true&limit=250&offset=0", "name": "макароны_крупы"},
    {"url": "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C12886/products?mode=delivery&include_restrict=true&limit=400&offset=0", "name": "овощи_фрукты_орехи"},
    {"url": "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C13093/products?mode=delivery&include_restrict=true&limit=400&offset=0", "name": "молоко_сметана"},
    {"url": "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C13094/products?mode=delivery&include_restrict=true&limit=400&offset=0", "name": "кефир_творог"},
    {"url": "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C13095/products?mode=delivery&include_restrict=true&limit=400&offset=0", "name": "сыр"},
    {"url": "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C13096/products?mode=delivery&include_restrict=true&limit=400&offset=0", "name": "яйцо_масло"},
    {"url": "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C13097/products?mode=delivery&include_restrict=true&limit=400&offset=0", "name": "майонез"},
    {"url": "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C13098/products?mode=delivery&include_restrict=true&limit=400&offset=0", "name": "йогурты_десерты"},
    {"url": "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C12888/products?mode=delivery&include_restrict=true&limit=400&offset=0", "name": "хлеб_выпечка"},
    {"url": "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C12889/products?mode=delivery&include_restrict=true&limit=400&offset=0", "name": "мясо_птица_колбасы"},
    {"url": "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C12890/products?mode=delivery&include_restrict=true&limit=400&offset=0", "name": "рыба_морепродукты"},
    {"url": "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C13070/products?mode=delivery&include_restrict=true&limit=400&offset=0", "name": "шоколад_конфеты"},
    {"url": "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C13071/products?mode=delivery&include_restrict=true&limit=400&offset=0", "name": "печенье_вафли_пряники"},
    {"url": "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C13072/products?mode=delivery&include_restrict=true&limit=400&offset=0", "name": "зефир_мармелад_халва"},
    {"url": "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C13073/products?mode=delivery&include_restrict=true&limit=400&offset=0", "name": "торты_пирожные"},
    {"url": "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C13074/products?mode=delivery&include_restrict=true&limit=400&offset=0", "name": "мороженое"},
    {"url": "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C13075/products?mode=delivery&include_restrict=true&limit=400&offset=0", "name": "жвачка_леденцы_драже"},
    {"url": "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C13076/products?mode=delivery&include_restrict=true&limit=400&offset=0", "name": "варенье_мёд_пасты"},
    {"url": "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C12901/products?mode=delivery&include_restrict=true&limit=400&offset=0", "name": "снеки_чипсы"},
    {"url": "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C12903/products?mode=delivery&include_restrict=true&limit=400&offset=0", "name": "замороженные_продукты"},
    {"url": "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C12904/products?mode=delivery&include_restrict=true&limit=499&offset=0", "name": "вода_напитки"},
    {"url": "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C12905/products?mode=delivery&include_restrict=true&limit=400&offset=0", "name": "здоровый_выбор"},
    {"url": "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C12906/products?mode=delivery&include_restrict=true&limit=499&offset=0", "name": "для_детей"},
    {"url": "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C12907/products?mode=delivery&include_restrict=true&limit=400&offset=0", "name": "для_животных"},
    {"url": "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C12908/products?mode=delivery&include_restrict=true&limit=499&offset=0", "name": "красота_гигиена_аптека"},
    {"url": "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C12909/products?mode=delivery&include_restrict=true&limit=400&offset=0", "name": "стирка_уборка"},
    {"url": "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C13056/products?mode=delivery&include_restrict=true&limit=400&offset=0", "name": "сухие_завтраки_каши"},
    {"url": "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C13058/products?mode=delivery&include_restrict=true&limit=400&offset=0", "name": "сахар_соль_специи"},
    {"url": "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C13059/products?mode=delivery&include_restrict=true&limit=400&offset=0", "name": "масло_уксус"},
    {"url": "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C13060/products?mode=delivery&include_restrict=true&limit=400&offset=0", "name": "майонез_соусы"},
    {"url": "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C13061/products?mode=delivery&include_restrict=true&limit=400&offset=0", "name": "консервы_соленья"},
    {"url": "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C13062/products?mode=delivery&include_restrict=true&limit=400&offset=0", "name": "всё_для_выпечки"},
    {"url": "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C13063/products?mode=delivery&include_restrict=true&limit=400&offset=0", "name": "залить_кипятком"},
]

def save_clipboard_to_file(filename):
    """Сохраняет содержимое буфера обмена в указанный файл"""
    try:
        result = subprocess.run(['xclip', '-selection', 'clipboard', '-o'], 
                               capture_output=True, text=True)
        
        if result.stdout and len(result.stdout) > 100:
            with open(filename, 'w', encoding='utf-8') as f:
                f.write(result.stdout)
            
            print(f"   ✅ Сохранено: {filename}")
            print(f"   📊 Размер: {len(result.stdout)} символов")
            
            # Проверяем JSON
            try:
                json.loads(result.stdout)
                print(f"   ✅ JSON валидный")
                return True
            except:
                print(f"   ⚠️ Внимание: не валидный JSON")
                return True  # Всё равно сохраняем
        else:
            print(f"   ❌ Буфер пуст или слишком маленький ({len(result.stdout) if result.stdout else 0} символов)")
            return False
    except Exception as e:
        print(f"   ❌ Ошибка: {e}")
        return False

def open_url_in_firefox(url):
    """Открывает URL в Firefox"""
    subprocess.Popen(['firefox', url])
    time.sleep(2)

def main():
    print("="*70)
    print("📦 ПАКЕТНЫЙ СБОР JSON ДАННЫХ")
    print("="*70)
    print(f"\n📊 Всего категорий: {len(URLS)}")
    print("\n⚠️  Важно: Для каждой страницы нужно вручную скопировать JSON")
    print("   Используйте кнопку 'Копировать' или Ctrl+A -> Ctrl+C\n")
    
    input("⏎ Нажмите Enter, чтобы начать...")
    
    success_count = 0
    
    for i, item in enumerate(URLS, 1):
        print("\n" + "="*70)
        print(f"📌 Категория {i}/{len(URLS)}: {item['name']}")
        print("="*70)
        
        # Открываем URL
        print(f"\n🌐 Открываю страницу...")
        open_url_in_firefox(item['url'])
        
        # Ждём загрузки
        print(f"⏳ Ожидание загрузки (5 секунд)...")
        time.sleep(5)
        
        # Инструкция для пользователя
        print(f"\n📋 Инструкция для категории '{item['name']}':")
        print(f"   1. Дождитесь полной загрузки страницы в Firefox")
        print(f"   2. Нажмите кнопку 'Копировать' или Ctrl+A -> Ctrl+C")
        print(f"   3. Вернитесь в терминал и нажмите Enter")
        
        # Ждём подтверждения
        input(f"\n⏎ Нажмите Enter после копирования JSON...")
        
        # Сохраняем в файл temp_i.json
        filename = f"temp_{i}.json"
        print(f"\n💾 Сохраняю данные...")
        
        if save_clipboard_to_file(filename):
            success_count += 1
            
            # Показываем первые 100 символов для проверки
            with open(filename, 'r') as f:
                preview = f.read(100)
            print(f"   📄 Начало данных: {preview[:80]}...")
        else:
            print(f"\n❌ Не удалось сохранить данные для {item['name']}")
            retry = input("   Повторить попытку? (y/n): ")
            if retry.lower() == 'y':
                print(f"\n💾 Повторная попытка...")
                if save_clipboard_to_file(filename):
                    success_count += 1
        
        # Пауза между категориями
        if i < len(URLS):
            print(f"\n⏭️  Переход к следующей категории через 3 секунды...")
            time.sleep(3)
    
    # Итоговый отчёт
    print("\n" + "="*70)
    print("📊 ИТОГОВЫЙ ОТЧЁТ")
    print("="*70)
    print(f"✅ Успешно сохранено: {success_count}/{len(URLS)} файлов")
    print(f"📁 Файлы сохранены как: temp_1.json, temp_2.json, ...")
    
    # Проверяем созданные файлы
    print("\n📋 Список созданных файлов:")
    for i in range(1, len(URLS) + 1):
        filename = f"../temps/temp_{i}.json"
        if os.path.exists(filename):
            size = os.path.getsize(filename)
            print(f"   ✅ temp_{i}.json - {size} байт")
        else:
            print(f"   ❌ temp_{i}.json - не создан")
    
    print("\n✨ Готово!")

if __name__ == "__main__":
    # Проверяем наличие xclip
    try:
        subprocess.run(['xclip', '-version'], capture_output=True, check=True)
        subprocess.run(['which', 'firefox'], capture_output=True, check=True)
    except:
        print("❌ Установите необходимые программы:")
        print("   sudo apt install xclip firefox")
        exit(1)
    
    main()