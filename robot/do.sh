arduino-cli compile -b arduino:avr:uno
arduino-cli upload -b arduino:avr:uno -p /dev/cu.usbserial-110
arduino-cli monitor -p /dev/cu.usbserial-110
