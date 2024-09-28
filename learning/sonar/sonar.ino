const int echo = 9, trig = 10;

void setup() {
  pinMode(echo, INPUT);
  pinMode(trig, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  Serial.println();

  digitalWrite(trig, HIGH);
  digitalWrite(trig, LOW);

  float duration = pulseIn(echo, HIGH);
  float distance = (duration * .0343) / 2;

  Serial.print("Distance: ");
  Serial.print(distance);
} 
