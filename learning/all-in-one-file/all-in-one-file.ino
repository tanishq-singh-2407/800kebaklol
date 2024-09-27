int baseSpeed = 100;
int turnSpeed = 100;
int Kp = 65;
int IN1 = 4, IN2 = 2, ENA = 5;
int IN3 = 7, IN4 = 3, ENB = 6;

int sensors[5] = {A1, A2, A3, A0, A4};
String lp = "00000";
String path = "";

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
    lp = readLine();

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
    lp = readLine();

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
    lp = readLine();
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

void straightPID() {
    String linePosition = readLine();

    Serial.println(linePosition);

    if (linePosition == "11100") turnLeft();
    else if (linePosition == "00111") turnRight();
    else if (linePosition == "11111") turnLeft();
    else if (linePosition == "00000") drive(0, 0);
    else {
        drive(baseSpeed + tuning(linePosition), baseSpeed - tuning(linePosition));
    };

}

void straight() {
    lp = readLine();
    int s = 90; // speed

    while (lp != "11111" && lp != "11100" && lp != "00111") {
        if (lp[1] == '1' || lp[0] == '1') drive(0, s);
        else if (lp[3] == '1' || lp[4] == '1') drive(s, 0);
        else if (lp[2] == '1') drive(s, s);
        else {drive(0, 0); break;};

        delay(1); drive(0, 0);
        lp = readLine();
    }

    delay(500);
}

void oneStep(){
    drive(baseSpeed, baseSpeed); delay(150);
    drive(0, 0);
}; 

int s = 80;

void turn(char t) {
    switch (t) {
        case 'L':
            while(lp[1] != '1') {
                drive(-turnSpeed, turnSpeed); delay(1);
                drive(0, 0);

                lp = readLine();
            }
  
            break;

        case 'R':
        case 'U':
            while(lp[3] != '1') {
                drive(turnSpeed, -turnSpeed); delay(1);
                drive(0, 0);

                lp = readLine();
            }
  
            break;
    };
};

void loop() {
    lp = readLine();

    if (lp == "11111" || lp == "01111" || lp == "11110" || lp == "01110") { // (1. T), (2. +), (3. maze end)
        oneStep();
        lp = readLine();

        if (lp == "11111") {} // maze end
        else if (lp.indexOf('1') > 0) {turn('L');} // +
        else {turn('L');} // T
    }

    else if (lp == "11100") { // (1. left T), (2. left)
        oneStep();
        lp = readLine();

        if (lp.indexOf('1') > 0) {turn('L');} // Left T
        else {turn('L');} // Left
    }

    else if (lp == "00111") { // (1. right T), (2. right)
        oneStep();
        lp = readLine();

        if(lp.indexOf('1') > 0) {} // Right T
        else {turn('R');} // Right
    }

    else if (lp[1] == '1' || lp[0] == '1') drive(0, s);
    else if (lp[3] == '1' || lp[4] == '1') drive(s, 0);
    else if (lp[2] == '1') drive(s, s);
    else {turn('U');} // u-turn
   
    delay(1);
    drive(0, 0); 
};
