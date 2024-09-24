class PID {
private:
	float Kp, Ki, Kd;
	unsigned long lastTime;
	float lastError, integral;

public:
	PID(float Kp, float Ki, float Kd) {
		this->Kp = Kp;
		this->Ki = Ki;
		this->Kd = Kd;

		reset();
	};

	void reset() {
		this->lastError = 0;
		this->lastTime = 0;
		this->integral = 0;
	};

	int getTuning(float error) {
		unsigned long time = millis();
		float de = (error - lastError);
		unsigned long dt = (time - lastTime);
		integral += (error * time);
	
		return ((Kp * error) + (Ki * integral) + (Kd * (de / dt)));
	};
};
