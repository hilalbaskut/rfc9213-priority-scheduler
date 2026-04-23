@echo off
:: Windows için derleme betiği

gcc -Wall -Wextra -g -Iinclude src/main.c src/priority_queue.c src/rfc_parser.c -o gateway_scheduler.exe
if %errorlevel% neq 0 (
    echo Derleme basarisiz oldu.
    exit /b %errorlevel%
)

echo Derleme tamamlandi.

gateway_scheduler.exe