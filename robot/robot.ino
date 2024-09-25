#include "sensors.h"
#include "motors.h"
#include "pid.h"

int maxSpeed = 255;
int minSpeed = 150;
int baseSpeed = 110;

const int sensors_pin[5] = {A4, A0, A3, A2, A1};

struct Motor motorLeft {6, 7};
struct Motor motorRight {5, 4};

Motors motors(&motorLeft, &motorRight);
Sensors sensors(sensors_pin);
PID pid(30, 0, 0);

String intToBin(int n) {
    if (n == 0) return "0";
    
    String binary = "";
    while (n > 0) {
        binary = (n % 2 == 0 ? "0" : "1") + binary;
        n /= 2;
    }

    return binary;
}

void straight() {
    while(!sensors.detectIntersection() && sensors.detectLine()) {
        int error = sensors.getError();
        int tuning = pid.getTuning(error);

        motors.driveLeft(baseSpeed + tuning);
        motors.driveRight(baseSpeed - tuning);
    };

    motors.stop();
};

void setup() {
    Serial.begin(9600);

    motors.initialize();
    sensors.initialize();
};

void left() {
    while(!sensors.detectMiddleLine()) {
        motors.driveRight(150);
        delay(50);

        motors.stop();
    };

    motors.stop();
};

void loop() {
/*
    if (sensors.detectLine()) {
        int tuning = pid.getTuning(sensors.getError());

        motors.driveLeft(baseSpeed + tuning);
        motors.driveRight(baseSpeed - tuning);
    } else motors.stop();
*/
  left();
};

void loopu() {
    motors.driveRight(150);
    motors.driveLeft(170);
    delay(1000);

    motors.stop();
    delay(1000);
};
