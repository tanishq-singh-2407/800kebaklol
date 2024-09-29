// bang bang se mast chal raha h, turns bhe smooth le raha h, and sonar bhe mast chal raha h, but node acche se detect kar raha h
// aur acche se chalana h

// Motors
const int baseSpeed = 100;
const int turnSpeed = 100;
const int IN1 = 4, IN2 = 2, ENA = 5;
const int IN3 = 7, IN4 = 3, ENB = 6;

// IR Sensor
const int sensors[5] = {A1, A2, A3, A0, A4};
String lp = "00000";

void drive(int l, int r) {
    digitalWrite(IN1, l > 0 ? 1 : 0);
    digitalWrite(IN2, l > 0 ? 0 : 1);
    digitalWrite(IN3, r > 0 ? 1 : 0);
    digitalWrite(IN4, r > 0 ? 0 : 1);

    if (l == 0) {
        digitalWrite(IN1, 0);
        digitalWrite(IN2, 0);
    };

    if (r == 0) {
        digitalWrite(IN3, 0);
        digitalWrite(IN4, 0);
    };

    analogWrite(ENA, constrain(abs(l), 0, 255));
    analogWrite(ENB, constrain(abs(r), 0, 255));
};

String readLine(bool readDistance = true) {
    String linePosition = "";
   
    for (int i=0; i<5; i++)
        linePosition += digitalRead(sensors[i]) ? "1" : "0";

    return linePosition;
};

void setup() {
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    pinMode(ENA, OUTPUT);
    pinMode(ENB, OUTPUT);

    for (int i=0; i<5; i++)
        pinMode(sensors[i], INPUT);

    drive(0, 0);

    Serial.begin(9600);
};

void oneStep(){
    drive(baseSpeed, baseSpeed); delay(150);
    drive(0, 0);
}; 

void loop() {
    drive(255, 255);
    delay(2000);
    drive(-255, -255);
    delay(2000);
};
