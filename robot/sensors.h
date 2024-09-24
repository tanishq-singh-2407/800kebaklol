/*
	Sensor Position (x)

		| x |
	| x |   | x |
*/


class Sensors {
private:
	int *pins;

public:
	Sensors(int *pins) {
		this->pins = pins;
	};

	void initialize() {
		for (int i=0; i<3; i++)
			pinMode(pins[i], INPUT);
	};

	float getError() {
		switch (getLinePosition()) {
			case 0b000: return 0.00; // center line

			case 0b001: return 1.00; // leaning towards right
			case 0b011: return 0.50;

			case 0b100: return -1.00; // leaning towards left
			case 0b110: return -0.50;
		};
	};

	int getLinePosition() { // (1 -> black) and (0 -> white)
		int linePosition = 0;

		for (int i=0; i<3; i++)  // only for 3 IR Sensors
			if (!digitalRead(pins[i]))
				linePosition |= (1 << i);

		return linePosition;
	};
};
