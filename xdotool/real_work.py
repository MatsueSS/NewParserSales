#!/usr/bin/env python3
# simple_copy.py

import subprocess
import time
import json
from datetime import datetime

def save_clipboard():
    """Сохраняет содержимое буфера обмена в файл"""
    try:
        result = subprocess.run(['xclip', '-selection', 'clipboard', '-o'], 
                               capture_output=True, text=True)
        
        if result.stdout and len(result.stdout) > 10:
            filename = f"api_data_{datetime.now().strftime('%Y%m%d_%H%M%S')}.json"
            with open(filename, 'w', encoding='utf-8') as f:
                f.write(result.stdout)
            
            print(f"\n✅ Сохранено в {filename}")
            print(f"📊 Размер: {len(result.stdout)} символов")
            
            # Проверяем, похоже ли на JSON
            if result.stdout.strip().startswith(('{', '[')):
                print(f"✅ Похоже на JSON")
                try:
                    json.loads(result.stdout)
                    print(f"✅ JSON валидный!")
                except:
                    print(f"⚠️ Не валидный JSON, но данные сохранены")
            else:
                print(f"⚠️ Данные не начинаются с {{ или [. Первые 100 символов:")
                print(result.stdout[:100])
            
            return True
        else:
            print(f"❌ Буфер обмена пуст или слишком маленький")
            return False
    except Exception as e:
        print(f"❌ Ошибка: {e}")
        return False

def main():
    API_URL = "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C13057/products?mode=delivery&include_restrict=true&limit=350&offset=0"
    
    print("="*60)
    print("📦 Простой сбор JSON с ручным копированием")
    print("="*60)
    
    # Открываем Firefox с URL
    print(f"\n🌐 Открываю браузер...")
    subprocess.Popen(['firefox', API_URL])
    
    print(f"\n📋 Инструкция:")
    print(f"1. Дождитесь загрузки страницы в Firefox")
    print(f"2. Нажмите кнопку 'Копировать' или выделите весь JSON (Ctrl+A)")
    print(f"3. Скопируйте JSON в буфер (Ctrl+C)")
    print(f"4. Вернитесь в этот терминал и нажмите Enter")
    
    input(f"\n⏎ Нажмите Enter после того, как скопировали JSON...")
    
    # Пытаемся сохранить несколько раз
    for attempt in range(3):
        print(f"\n🔄 Попытка {attempt + 1}...")
        if save_clipboard():
            break
        if attempt < 2:
            print("Попробуйте скопировать JSON ещё раз и нажмите Enter")
            input("⏎ Нажмите Enter...")
    
    print("\n✨ Готово!")

if __name__ == "__main__":
    # Проверяем наличие xclip
    try:
        subprocess.run(['xclip', '-version'], capture_output=True, check=True)
    except:
        print("❌ Установите xclip: sudo apt install xclip")
        exit(1)
    
    main()