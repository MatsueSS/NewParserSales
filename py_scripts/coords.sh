#!/bin/bash

# Проверка установки xdotool
if ! command -v xdotool &> /dev/null; then
    echo "xdotool не установлен. Установите его командой:"
    echo "sudo apt install xdotool"
    exit 1
fi

echo "Нажмите Ctrl+C для выхода"
echo "Координаты курсора (X, Y):"

while true; do
    eval $(xdotool getmouselocation --shell)
    echo -ne "\rX: $X, Y: $Y    "
    sleep 0.1
done