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
	// Bitboard oldBitboard = bitboard;
	bitboard = Bitboard();
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			digitalWrite(arrRow[j], HIGH);
			delay(1);
		}
		digitalWrite(arrRow[i], LOW);
		delay(1);

		for (int j = 0; j < 8; j++) {
			if (digitalRead(arrCol[j]) == LOW) {
				delay(1);
				if (digitalRead(arrCol[j]) == LOW) {//double check
					bitboard.set(i * 8 + j, 1);
				}
				delay(1);
			}
		}

		digitalWrite(arrRow[i], HIGH);
		delay(1);
	}
	// if (oldBitboard != bitboard) {
	// 	changed = true;
	// 	lastChangedTime = millis();
	// } else {
	// 	if (changed && millis() - lastChangedTime > 1000) {
	// 		changed = false;
	// 		if (oldBitboard != bitboard) {
	// 			MessageController::getInstance()->send(BOARD_CHANGED, bitboard.toString());
	// 		}
	// 	}
	// }
}

Bitboard SwitchesController::getCurrentState() {
	return bitboard;
}

void SwitchesController::onScanRequest() {
	scan();
	MessageController::getInstance()->reply(SCAN_DONE, bitboard.toString());
}

void SwitchesController::onResetRequest() {
	bitboard = Bitboard();
}

void SwitchesController::init() {
	for (int i = 0; i < 8; i++) {
		pinMode(arrRow[i], OUTPUT);
		pinMode(arrCol[i], INPUT_PULLUP);
	}

	changed = false;

	MessageController::getInstance()->setSwitchesControllerMessageDelegate(this);
}
