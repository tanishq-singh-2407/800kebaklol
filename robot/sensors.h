/*
	Sensor Position (x)

			| x |
	| x | x |   | x | x |
*/


class Sensors {
private:
	int *pins;

public:
	Sensors(int *pins) {
		this->pins = pins;
	};

	void initialize() {
		for (int i=0; i<5; i++)
			pinMode(pins[i], INPUT);
	};

	float getError() {
		switch (getLinePosition()) {
			case 0b11011: return 0.00;
			case 0b11101: return 0.50;
			case 0b11110: return 1.00;
			case 0b11001: return 0.25;
			case 0b11100: return 0.75;
			case 0b10111: return -0.50;
			case 0b01111: return -1.00;
			case 0b10011: return -0.25;
			case 0b00111: return -0.75;	
		};
	};

	int getLinePosition() { // (0 -> black) and (1 -> white)
		int linePosition = 0;

		for (int i=0; i<5; i++)
			if (!digitalRead(pins[i]))
				linePosition |= (1 << i);

		return linePosition;
	};

	bool detectMiddleLine() {
		return digitalRead(pins[2]);
	};

	bool detectIntersection() {
		return (digitalRead(pins[0]) && digitalRead(pins[1]) && digitalRead(pins[2])) || (digitalRead(pins[2]) && digitalRead(pins[3]) && digitalRead(pins[4]));
	};
};
