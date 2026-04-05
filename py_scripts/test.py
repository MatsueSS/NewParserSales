#!/usr/bin/env python3
# batch_copy_final.py

import subprocess
import time
import json
import os

URLS = [
    {"url": "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C13057/products?mode=delivery&include_restrict=true&limit=350&offset=0", "name": "чай_кофе_какао"},
    {"url": "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C13055/products?mode=delivery&include_restrict=true&limit=250&offset=0", "name": "макароны_крупы"},
    # добавь остальные URL
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
            
            try:
                json.loads(result.stdout)
                print(f"   ✅ JSON валидный")
                return True
            except:
                print(f"   ⚠️ Внимание: не валидный JSON")
                return True
        else:
            print(f"   ❌ Буфер пуст или слишком маленький")
            return False
    except Exception as e:
        print(f"   ❌ Ошибка: {e}")
        return False

def open_url_in_firefox(url):
    """Открывает URL в Firefox"""
    subprocess.Popen(['firefox', url])
    time.sleep(2)

def click_copy_button():
    """Нажимает кнопку копирования через ydotool"""
    try:
        # Перемещаем мышь на кнопку (координаты 80, 75)
        subprocess.run(['ydotool', 'mousemove', '80', '75'], check=False)
        time.sleep(0.3)
        
        # Делаем клик
        subprocess.run(['ydotool', 'click', '0xC0'], check=False)
        time.sleep(0.5)
        
        print(f"   🖱️ Кнопка нажата")
        return True
    except Exception as e:
        print(f"   ❌ Ошибка при нажатии: {e}")
        return False

def main():
    print("="*70)
    print("📦 ПАКЕТНЫЙ СБОР JSON ДАННЫХ")
    print("="*70)
    print(f"\n📊 Всего категорий: {len(URLS)}")
    
    input("\n⏎ Нажмите Enter, чтобы начать...")
    
    success_count = 0
    
    for i, item in enumerate(URLS, 1):
        print("\n" + "="*70)
        print(f"📌 Категория {i}/{len(URLS)}: {item['name']}")
        print("="*70)
        
        # Открываем URL
        print(f"\n🌐 Открываю страницу...")
        open_url_in_firefox(item['url'])
        
        # Ждём загрузки
        print(f"⏳ Ожидание загрузки (8 секунд)...")
        time.sleep(8)
        
        # Нажимаем кнопку копирования
        print(f"\n📋 Нажимаю кнопку копирования...")
        click_copy_button()
        
        # Сохраняем в файл
        filename = f"../temps/temp_{i}.json"
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
    
    print("\n✨ Готово!")

if __name__ == "__main__":
    os.makedirs('../temps', exist_ok=True)
    
    main()