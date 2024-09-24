struct Motor {
	int speed;
	int dirn;
};

class Motors {
private:
	struct Motor *motorLeft;
	struct Motor *motorRight;
	int maxSpeed;

public:
	Motors (struct Motor *motorLeft, struct Motor *motorRight) {
		this->motorLeft = motorLeft;
		this->motorRight = motorRight;
		this->maxSpeed = 255;
	};

	void initialize() {
		pinMode(motorLeft->speed, OUTPUT);
		pinMode(motorLeft->dirn, OUTPUT);
		pinMode(motorRight->speed, OUTPUT);
		pinMode(motorRight->dirn, OUTPUT);

		stop();	
	};

	void driveLeft(int speed) {
		digitalWrite(motorLeft->dirn, speed ? HIGH : LOW);
		analogWrite(motorLeft->speed, speed ? min(speed, maxSpeed): 0);
	};

	void driveRight(int speed) {
		digitalWrite(motorRight->dirn, speed ? HIGH : LOW);
		analogWrite(motorRight->speed, speed ? min(speed, maxSpeed) : 0);
	};

	void drive(int speed) {
		driveLeft(speed);
		driveRight(speed);
	};

	void stop() {
		driveLeft(0);
		driveRight(0);
	};
};
