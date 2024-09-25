arduino-cli compile -b arduino:avr:uno
arduino-cli upload -b arduino:avr:uno -p /dev/cu.usbserial-10
arduino-cli monitor -p /dev/cu.usbserial-10
