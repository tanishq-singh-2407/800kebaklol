#include <Servo.h>

// Motors
const int baseSpeed = 100;
const int turnSpeed = 100;
int bangSpeed = 100;
const int IN1 = 4, IN2 = 2, ENA = 5;
const int IN3 = 7, IN4 = 3, ENB = 6;

// Ultrasonic
const int echo = 9, trig = 10;

// IR Sensor
const int sensors[5] = {A1, A2, A3, A0, A4};
String lp = "00000";
String path = "";

// Servo
Servo s1;

bool oneTime = true; // pickup treasure
bool mazeSolve = true;
int retracePath = 0;

void drive(int l, int r) { // left motor speed, right motor speed
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

String readLine(bool readDistance = true) { // return position of black strip, or object in front
    String linePosition = "";

    if (readDistance) {
        digitalWrite(trig, LOW);
        delayMicroseconds(2);  digitalWrite(trig, HIGH);
        delayMicroseconds(10); digitalWrite(trig, LOW);

        float duration = pulseIn(echo, HIGH, 10000L);
        float distance = (duration * .0343) / 2;

		if (distance < 15.0 && distance) return "00000";
    }

	for (int i=0; i<5; i++)
        linePosition += digitalRead(sensors[i]) ? "1" : "0";

    return linePosition;
};

void oneStep(bool forward = true, unsigned long delayTime = 150){ // move one step forward
    drive(0, 0); delay(200);
    drive(forward ? baseSpeed : -baseSpeed, forward ? baseSpeed : -baseSpeed); delay(delayTime);
    drive(0, 0);
}; 

void turn(char t, bool move = true) {
    drive(0, 0);

    switch (t) {
        case 'B':
			drive(-turnSpeed, turnSpeed); delay(600); drive(0, 0); delay(300);

            while(lp[3] != '1' && lp[2] != '1' && lp[4] != '1') {
                drive(-turnSpeed, turnSpeed); delay(1);
                drive(0, 0);

                lp = readLine(false);
            }
  
            break;


        case 'L':
			if (move) oneStep();
			drive(-turnSpeed, turnSpeed); delay(400); drive(0, 0); delay(300);

            while(lp[3] != '1' && lp[2] != '1' && lp[4] != '1') {
                drive(-turnSpeed, turnSpeed); delay(1);
                drive(0, 0);

                lp = readLine(false);
            }
  
            break;
        
        case 'R':
			if (move) oneStep();
			drive(turnSpeed, -turnSpeed); delay(400);  drive(0, 0); delay(300);

            while(lp[3] != '1' && lp[2] != '1' && lp[4] != '1') {
                drive(turnSpeed, -turnSpeed); delay(1);
                drive(0, 0);

                lp = readLine(false);
            }
  
            break;
    };
};

String shortPathBackwards(String path) {
    path.replace("LBL", "S");
    path.replace("LBS", "R");
    path.replace("RBL", "B");
    path.replace("SBS", "B");
    path.replace("SBL", "R");
    path.replace("LBR", "B");

    return path;
};

String reverseString(String str) {
    String reversed = "";
    int length = str.length();
    
    for (int i = length - 1; i >= 0; i--)
        reversed += str[i];
    
    return reversed;
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

    pinMode(13, OUTPUT);
    digitalWrite(13, 0);

    s1.attach(11);
    s1.write(90);

    for (int i=0; i<5; i++)
        pinMode(sensors[i], INPUT);

    drive(0, 0);

    Serial.begin(9600);
};

void loop() {
    lp = readLine(mazeSolve);

	if (mazeSolve) { // maze solving
		if (lp[0] == '1' && lp[4] == '1') { // T, +, maze end
			oneStep();
			lp = readLine(false);

			if (lp == "11111") { mazeSolve = false; }
			else { drive(0, 0); delay(1000); turn('L', false); path += 'L'; } // T or +
		}
		else if (lp == "11100" || lp == "11110") { // left, left-T, maze end
			oneStep();
			lp = readLine(false);

			if (lp == "11111") { mazeSolve = false; }
			else { turn('L', false); path += 'L';}
		}
		else if (lp == "00111" || lp == "01111") { // right, right-T, maze end
			oneStep();
			lp = readLine();

			if (lp == "11111") { mazeSolve = false; }
			else if(lp.indexOf('1') > 0) { path += 'S'; }
			else {turn('R', false); path += 'R';}
		}
		else if (lp[0] == '1') {drive(0, 1.4 * baseSpeed);              drive(0, bangSpeed);            }
		else if (lp[4] == '1') {drive(1.4 * baseSpeed, 0);              drive(bangSpeed, 0);            }
		else if (lp[1] == '1') {drive(.5 * baseSpeed, 1.4 * baseSpeed); drive(bangSpeed / 3, bangSpeed);}
		else if (lp[3] == '1') {drive(1.4 * baseSpeed, .5 * baseSpeed); drive(bangSpeed, bangSpeed / 3);}
		else if (lp[2] == '1') {drive(baseSpeed, baseSpeed);}
		else { turn('B'); path += 'B'; }; // u-turn
	} else { // maze solved, retracing path
		if (oneTime) {
			digitalWrite(13, 1); delay(2000);
			s1.write(0); delay(2000);

			oneStep(false, 200);
			turn('B');
			
			while(path.indexOf('B') > 0) {
        		path = shortPathBackwards(path);
			};
			
			for (int i=0; i<path.length(); i++)
				path[i] = path[i] == 'L' ? 'R' : path[i] == 'R' ? 'L' : path[i];

    		path = reverseString(path);
			
			oneTime = false;

			drive(baseSpeed, baseSpeed);
			lp = readLine(false);
		}


		if (lp == "11100" || lp == "00111" || lp == "11110" || lp == "01111" || lp == "11111") { // any type of turn detected
            switch(path[retracePath++]) {
                case 'L':
					turn('L');
                    break;

                case 'R':
					turn('R');
                    break;

                case 'S':
                    oneStep(true, 250);
                    break;
            }; 
        }

		else if (lp[0] == '1') {drive(0, 1.4 * baseSpeed);              drive(0, bangSpeed);            }
		else if (lp[4] == '1') {drive(1.4 * baseSpeed, 0);              drive(bangSpeed, 0);            }
		else if (lp[1] == '1') {drive(.5 * baseSpeed, 1.4 * baseSpeed); drive(bangSpeed / 3, bangSpeed);}
		else if (lp[3] == '1') {drive(1.4 * baseSpeed, .5 * baseSpeed); drive(bangSpeed, bangSpeed / 3);}
		else if (lp[2] == '1') {drive(baseSpeed, baseSpeed);}
		else { turn('B'); };
	};
};
