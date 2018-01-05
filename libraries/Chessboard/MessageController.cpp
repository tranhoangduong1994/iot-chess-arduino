#include <MessageController.h>

const char MESSAGE_ENDING_CHAR = '|';

MessageController* MessageController::instance = NULL;

MessageController* MessageController::getInstance() {
	if (!instance) {
		instance = new MessageController();
		instance->init();
	}

	return instance;
}

void MessageController::init() {
	commandBuffer = "";
	motorsDelegate = NULL;
	switchesDelegate = NULL;
	speakerDelegate = NULL;
	send(SYSTEM_READY);
}

void MessageController::translateMessage() {
	// Serial.print(String("[Arduino-MessageController] translateMessage: ") + commandBuffer + "|");
	if (commandBuffer[0] - 48 == MOVE) {
		char fromFile = commandBuffer[1];
		int fromRank = commandBuffer[2] - 48;
		char toFile = commandBuffer[3];
		int toRank = commandBuffer[4] - 48;
		if (motorsDelegate) {
			motorsDelegate->onMoveRequest(Position(fromFile, fromRank),	Position(toFile, toRank));
		}
	}	

	if (commandBuffer[0] - 48 == CASTLING) {
		int type = commandBuffer[1] - 48;
		if (motorsDelegate) {
			motorsDelegate->onCastlingRequest(CastlingType(type));
		}
	}	

	if (commandBuffer[0] - 48 == SCAN_BOARD) {
		if (switchesDelegate) {
			switchesDelegate->onScanRequest();
		}
	}

	if (commandBuffer[0] - 48 == RESET_BOARD) {
		if (motorsDelegate) {
			motorsDelegate->onResetRequest();
		}

		if (switchesDelegate) {
			switchesDelegate->onResetRequest();
		}
	}
	
	if (commandBuffer[0] - 48 == PLAY_SOUND) {
		int type = commandBuffer[1] - 48;
		if (speakerDelegate) {
			speakerDelegate->onSoundRequest(SoundType(type));
		}
	}

	if (commandBuffer[0] - 48 == MAGNET_ON) {
		motorsDelegate->onMagnetOnRequest();
	}
	
	if (commandBuffer[0] - 48 == MAGNET_OFF) {
		motorsDelegate->onMagnetOffRequest();
	}
}

void MessageController::checkMessage() {
	if (Serial.available()) {
		delay(100);
		while(Serial.available() > 0) {
			char character = (char)(Serial.read());
			if (character == MESSAGE_ENDING_CHAR) {
				// Serial.print("[Arduino-checkMessage] End message|");
				translateMessage();
				commandBuffer = "";
				// return;
			} else {
				// Serial.print(String("[Arduino-checkMessage] Received character:") + String(character) + "|");
				commandBuffer += character;
			}
		}
	}
}

void MessageController::reply(ServiceResponseType type, String content) {
	content += MESSAGE_ENDING_CHAR;
	Serial.print(String(MESSAGE_ENDING_CHAR) + String(ServiceResponse) + String(type) + content);
	Serial.flush();
}

void MessageController::send(EventType type, String content) { 
	content += MESSAGE_ENDING_CHAR;
	Serial.print(String(MESSAGE_ENDING_CHAR) + String(Event) + String(type) + content);
	Serial.flush();
}

void MessageController::setMotorsControllerMessageDelegate(MotorsControllerMessageProtocol* delegate) {
	motorsDelegate = delegate;
}

void MessageController::setSwitchesControllerMessageDelegate(SwitchesControllerMessageProtocol* delegate) {
	switchesDelegate = delegate;
}

void MessageController::setSpeakerControllerMessageDelegate(SpeakerControllerMessageProtocol* delegate) {
	speakerDelegate = delegate;
}
