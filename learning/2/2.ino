// bas ultrasonic test kar raha hu
// Ultrasonic
const int echo = 9, trig = 10;

void setup() {
    pinMode(echo, INPUT);
    pinMode(trig, OUTPUT);

	Serial.begin(9600);
}

void loop() {
	digitalWrite(trig, LOW);
    delayMicroseconds(2);  digitalWrite(trig, HIGH);
    delayMicroseconds(10); digitalWrite(trig, LOW);

    float duration = pulseIn(echo, HIGH);
    float distance = (duration * .0343) / 2;

	Serial.println(distance);
}
