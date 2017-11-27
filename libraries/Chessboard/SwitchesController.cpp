#include <SwitchesController.h>

#include <Arduino.h>

#include <MessageController.h>

const int arrRow[] = {29,37,28,36,27,35,26,34};
const int arrCol[] = {30,22,31,23,32,24,33,25};

SwitchesController* SwitchesController::instance = NULL;

SwitchesController* SwitchesController::getInstance() {
	if (!instance) {
		instance = new SwitchesController();
		instance->init();
	}

	return instance;
}

void SwitchesController::scan() {
	bool changed = false;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			digitalWrite(arrRow[j], HIGH);
			delay(1);
		}
		digitalWrite(arrRow[i], LOW);
		delay(1);

		for (int j = 0; j < 8; j++) {
			if (digitalRead(arrCol[j]) == LOW) {
				delay(10);
				if (digitalRead(arrCol[j]) == LOW) {//double check
					bitboard.set(i * 8 + j, 1);
					changed = true;
				}
				delay(10);
			}
		}

		digitalWrite(arrRow[i], HIGH);
		delay(10);
	}

	if (changed) {
		MessageController::getInstance()->send(EventType::BOARD_CHANGED, bitboard.toString());	
	}
}

const Bitboard& SwitchesController::getCurrentState() {
	return bitboard;
}

void SwitchesController::onScanRequest() {
	MessageController::getInstance()->reply(ServiceResponseType::SCAN_DONE, bitboard.toString());
}

void SwitchesController::init() {
	for (int i = 0; i < 8; i++) {
		pinMode(arrRow[i], OUTPUT);
		pinMode(arrCol[i], INPUT_PULLUP);
	}
}