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
	if (commandBuffer[0] == MessageType::CLEAR_SCREEN) {
		if (displayerDelegate) {
			displayerDelegate->onClearScreenRequest();
		}
	}

	if (commandBuffer[0] == MessageType::PRINT) {
		int line = commandBuffer[1] - 48;
		String content = commandBuffer.substring(2);
		if (displayerDelegate) {
			displayerDelegate->onPrintRequest(line, content);
		}
	}

	if (commandBuffer[0] == MessageType::MOVE) {
		char fromFile = commandBuffer[1];
		int fromRank = commandBuffer[2] - 48;
		char toFile = commandBuffer[3];
		int toRank = commandBuffer[4] - 48;
		if (motorsDelegate) {
			motorsDelegate->onMoveRequest(Position(fromFile, fromRank),	Position(toFile, toRank));
		}
	}	

	if (commandBuffer[0] == MessageType::CAPTURE) {
		char fromFile = commandBuffer[1];
		int fromRank = commandBuffer[2] - 48;
		char toFile = commandBuffer[3];
		int toRank = commandBuffer[4] - 48;
		if (motorsDelegate) {
			motorsDelegate->onMoveRequest(Position(fromFile, fromRank), Position(toFile, toRank));
		}
	}

	if (commandBuffer[0] == MessageType::SCAN_BOARD) {
		if (switchesDelegate) {
			switchesDelegate->onScanRequest();
		}
	}

	if (commandBuffer[0] == MessageType::RESET_BOARD) {
		if (motorsDelegate) {
			motorsDelegate->onResetRequest();
		}

		if(displayerDelegate) {
			displayerDelegate->onResetRequest();
		} 
	}
}

void MessageController::checkMessage() {
	if (Serial.available()) {
		while(Serial.available() > 0) {
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

void MessageController::reply(ReplyingType type, String content) {
	content += MESSAGE_ENDING_CHAR;
	Serial.print(String(type) + content);
	Serial.flush();
}

void MessageController::send(SendingType type, String content) {
	content += MESSAGE_ENDING_CHAR;
	Serial.print(String(type) + content);
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