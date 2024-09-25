void setup() {
    pinMode(A0, INPUT);
    pinMode(13, OUTPUT);
};

void loop() {
    pinMode(13, digitalRead(A0) ? HIGH : LOW);
};
