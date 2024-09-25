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
		switch ((getLinePosition() >> 1) & (~(1 << 3))) {
			case 0b101: return 0.00; // center line

			case 0b110: return 1.00; // leaning towards left
			case 0b100: return 0.50;

			case 0b011: return -1.00; // leaning towards right
			case 0b001: return -0.50;
		};
	};

	int getLinePosition() { // (1 -> black) and (0 -> white)
		int linePosition = 0;

		for (int i=0; i<5; i++)
			if (!digitalRead(pins[i]))
				linePosition |= (1 << i);

		return linePosition;
	};

	bool detectMiddleLine() {
		return digitalRead(pins[2]);
	};

	bool detectLine() {
		return digitalRead(pins[1]) || digitalRead(pins[2]) || digitalRead(pins[3]);
	};

	bool detectIntersection() {
		return digitalRead(pins[0]) || digitalRead(pins[4]);
	};
};
