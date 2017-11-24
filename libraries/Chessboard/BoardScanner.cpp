#include <BoardScanner.h>

#include <Arduino.h>

const int arrRow[] = {29,37,28,36,27,35,26,34};
const int arrCol[] = {30,22,31,23,32,24,33,25};

BoardScanner* BoardScanner::instance = NULL;

BoardScanner* BoardScanner::getInstance() {
	if (!instance) {
		instance = new BoardScanner();
		instance->init();
	}

	return instance;
}

Bitboard BoardScanner::scan() {
	Bitboard value;
	bool checkedSwitch = false;

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			digitalWrite(arrRow[j], HIGH);
			delay(1);
		}
		digitalWrite(arrRow[i], LOW);
		delay(1);

		for (int j = 0; j < 8; j++) {
			checkedSwitch = false;
			if (digitalRead(arrCol[j]) == LOW) {
				checkedSwitch = true;
			}
			delay(10);
			if (digitalRead(arrCol[j]) == LOW && checkedSwitch) {
				value.set(i * 8 + j, 1);
			}
			delay(10);
		}

		digitalWrite(arrRow[i], HIGH);
		delay(10);
	}

	return value;
}

void BoardScanner::init() {
	for (int i = 0; i < 8; i++) {
		pinMode(arrRow[i], OUTPUT);
		pinMode(arrCol[i], INPUT_PULLUP);
	}
}