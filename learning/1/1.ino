// bang bang se mast chal raha h, turns bhe smooth le raha h, and sonar bhe mast chal raha h, but node acche se detect kar raha h

// Motors
const int baseSpeed = 100;
const int turnSpeed = 100;
const int IN1 = 4, IN2 = 2, ENA = 5;
const int IN3 = 7, IN4 = 3, ENB = 6;

// Ultrasonic
const int echo = 9, trig = 10;

// IR Sensor
const int sensors[5] = {A1, A2, A3, A0, A4};
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

	if (readDistance) {
		digitalWrite(trig, LOW);
		delayMicroseconds(2);  digitalWrite(trig, HIGH);
		delayMicroseconds(10); digitalWrite(trig, LOW);

		float duration = pulseIn(echo, HIGH, 100000L);
		float distance = (duration * .0343) / 2;

		if (distance < 15.0 && distance) return "00000";
	}
	
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

    pinMode(echo, INPUT);
    pinMode(trig, OUTPUT);

    for (int i=0; i<5; i++)
        pinMode(sensors[i], INPUT);

    drive(0, 0);

    Serial.begin(9600);
};

void oneStep(){
    drive(baseSpeed, baseSpeed); delay(150);
    drive(0, 0);
}; 

void turn(char t) {
	drive(0, 0);

    switch (t) {
        case 'L':
            while(lp[1] != '1') {
                drive(-turnSpeed, turnSpeed); delay(1);
                drive(0, 0);

                lp = readLine(false);
            }
  
            break;

		case 'U':
			drive(turnSpeed, -turnSpeed); delay(250);
			drive(0, 0);

        case 'R':
            while(lp[3] != '1') {
                drive(turnSpeed, -turnSpeed); delay(1);
                drive(0, 0);

                lp = readLine(false);
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

    else if (lp[0] == '1') {drive(0, 1.3 * baseSpeed); drive(0, 105);}
    else if (lp[4] == '1') {drive(1.3 * baseSpeed, 0); drive(105, 0);}
    else if (lp[1] == '1') {drive(0, 1.3 * baseSpeed); drive(70 / 2, 87);}
    else if (lp[3] == '1') {drive(1.3 * baseSpeed, 0); drive(87, 70 / 2);}
    else if (lp[2] == '1') {drive(baseSpeed, baseSpeed);}
    else {turn('U');} // u-turn
};