int baseSpeed = 100;
int turnSpeed = 100;
int Kp = 40;
int IN1 = 4, IN2 = 2, ENA = 5;
int IN3 = 7, IN4 = 3, ENB = 6;

int sensors[5] = {A1, A2, A3, A0, A4};

void drive(int l, int r) {
    digitalWrite(IN1, l > 0 ? 1 : 0);
    digitalWrite(IN2, l > 0 ? 0 : 1);
    digitalWrite(IN3, r > 0 ? 1 : 0);
    digitalWrite(IN4, r > 0 ? 0 : 1);

    if (l == 0) {
        digitalWrite(IN1, 0);
        digitalWrite(IN2, 0);
    }

    if (r == 0) {
        digitalWrite(IN3, 0);
        digitalWrite(IN4, 0);
    }

    analogWrite(ENA, constrain(abs(l), 0, 255));
    analogWrite(ENB, constrain(abs(r), 0, 255));
};

String readLine() {
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

void turnRight() {
    drive(0, 0);
    String lp = readLine();

    delay(200);

    do { 
        drive(turnSpeed, turnSpeed); delay(200);
        drive(0, 0);

        lp = readLine();
    } while (lp[2] == '1' && lp[3] == '1' && lp[4] == '1');
  
    delay(100);

    do {
        drive(turnSpeed, -turnSpeed); delay(10);
        drive(0, 0);

        lp = readLine();
    } while (lp[2] == '1' || lp[3] == '1');

    delay(100);

    do {
        drive(turnSpeed, -turnSpeed); delay(10);
        drive(0, 0);

        lp = readLine();
    } while(lp[2] != '1' && lp[3] != '1');

    drive(0, 0);
};

void turnLeft() {
    drive(0, 0);
    String lp = readLine();

    delay(200);

    do { 
        drive(turnSpeed, turnSpeed); delay(200);
        drive(0, 0);

        lp = readLine();
    } while (lp[0] == '1' && lp[1] == '1' && lp[2] == '1');
  
    delay(100);

    do {
        drive(-turnSpeed, turnSpeed); delay(10);
        drive(0, 0);

        lp = readLine();
    } while (lp[1] == '1' || lp[2] == '1');

    delay(100);

    do {
        drive(-turnSpeed, turnSpeed); delay(10);
        drive(0, 0);

        lp = readLine();
    } while(lp[1] != '1' && lp[2] != '1');

    drive(0, 0);
};

void overshoot() {
    String lp = readLine();
};

float tuning(String linePosition) {
    float error;

    if (linePosition == "00100") error = 0.00;
    else if (linePosition == "01100") error = -0.25;
    else if (linePosition == "01000") error = -0.50;
    else if (linePosition == "11000") error = -0.75;
    else if (linePosition == "10000") error = -1.00;
    else if (linePosition == "00110") error =  0.25;
    else if (linePosition == "00010") error =  0.50;
    else if (linePosition == "00011") error =  0.75;
    else if (linePosition == "00001") error =  1.00;

    return error * Kp; 
}

void straight() {
    String linePosition = readLine();

    if (linePosition == "11100") turnLeft();
    else if (linePosition == "00111") turnRight();
    else if (linePosition == "11111") turnLeft();
    else if (linePosition == "00000") drive(0, 0);
    else {
        drive(baseSpeed + tuning(linePosition), baseSpeed - tuning(linePosition));
        delay(20);
        drive(0, 0);
    };

}

void loop() {
    straight();
};
