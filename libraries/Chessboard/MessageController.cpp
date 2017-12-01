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
	displayerDelegate = NULL;
}

void MessageController::translateMessage() {
	if (commandBuffer[0] - 48 == CLEAR_SCREEN) {
		if (displayerDelegate) {
			displayerDelegate->onClearScreenRequest();
		}
	}

	if (commandBuffer[0] - 48 == PRINT) {
		int line = commandBuffer[1] - 48;
		String content = commandBuffer.substring(2);
		if (displayerDelegate) {
			displayerDelegate->onPrintRequest(line, content);
		}
	}

	if (commandBuffer[0] - 48 == MOVE) {
		char fromFile = commandBuffer[1];
		int fromRank = commandBuffer[2] - 48;
		char toFile = commandBuffer[3];
		int toRank = commandBuffer[4] - 48;
		if (motorsDelegate) {
			motorsDelegate->onMoveRequest(Position(fromFile, fromRank),	Position(toFile, toRank));
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
}

void MessageController::checkMessage() {
	if (Serial.available()) {
		while(Serial.available() > 0) {
			delay(100);

			char character = (char)(Serial.read());
			if (character == MESSAGE_ENDING_CHAR) {
				translateMessage();
				commandBuffer = "";
				return;
			}

			commandBuffer += character;
		}
	}
}

void MessageController::reply(ServiceResponseType type, String content) {
	content += MESSAGE_ENDING_CHAR;
	Serial.println(String(ServiceResponse) + String(type) + content);
	Serial.flush();
}

void MessageController::send(EventType type, String content) {
	content += MESSAGE_ENDING_CHAR;
	Serial.println(String(Event) + String(type) + content);
	Serial.flush();
}

void MessageController::setMotorsControllerMessageDelegate(MotorsControllerMessageProtocol* delegate) {
	motorsDelegate = delegate;
}

void MessageController::setSwitchesControllerMessageDelegate(SwitchesControllerMessageProtocol* delegate) {
	switchesDelegate = delegate;
}

void MessageController::setDisplayControllerMessageDelegate(DisplayControllerMessageProtocol* delegate) {
	displayerDelegate = delegate;
}