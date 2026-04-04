#!/bin/bash
# simple_click.sh

BUTTON_X=177
BUTTON_Y=154

echo "Нажимаю кнопку 'Копировать'..."

# Активируем Firefox
xdotool search --class "Firefox" windowactivate
sleep 1

# Кликаем
xdotool mousemove $BUTTON_X $BUTTON_Y click 1
sleep 1

# Сохраняем
xclip -selection clipboard -o > "api_data_$(date +%Y%m%d_%H%M%S).json"

echo "Готово!"