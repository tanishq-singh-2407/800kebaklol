#include "sensors.h"
#include "motors.h"
#include "pid.h"

int maxSpeed = 255;
int minSpeed = 150;
int baseSpeed = 190;

const int sensors_pin[3] = {A3, A4, A5};

struct Motor motorLeft {6, 7};
struct Motor motorRight {9, 8};

Motors motors(&motorLeft, &motorRight);
Sensors sensors(sensors_pin);
PID pid(65, 0, 0);

void setup() {
    Serial.begin(9600);

    motors.initialize();
    sensors.initialize();
};

void loop() {};
