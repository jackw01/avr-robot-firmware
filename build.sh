printf 'Building: pio run --target upload\n'
pio run --target upload
printf '\nMonitoring: pio device monitor --port COM3 --baud 115200\n\n'
pio device monitor --port COM3 --baud 115200
