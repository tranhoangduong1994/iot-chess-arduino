#include <KeyboardScanner.h>

#include <Arduino.h>

#include <MessageController.h>

KeyboardScanner* KeyboardScanner::instance = NULL;

#define MENU 2
#define UP 14
#define DOWN 15
#define OK 3

KeyboardScanner* KeyboardScanner::getInstance() {
	if (!instance) {
		instance = new KeyboardScanner();
		instance->init();
	}

	return instance;
}

void KeyboardScanner::scan() {
	int value = 0;

	if (digitalRead(DOWN) == LOW && !downPressed) {
		downPressed = true;
		MessageController::getInstance()->send(SendingType::DOWN_PRESSED);
	}

	if (digitalRead(DOWN) == HIGH && downPressed) {
		downPressed = false;
	}

	if (digitalRead(UP) == LOW && !upPressed) {
		upPressed = true;
		MessageController::getInstance()->send(SendingType::UP_PRESSED);
	}

	if (digitalRead(UP) == HIGH && upPressed) {
		upPressed = false;
	}

	if (digitalRead(MENU) == LOW && !menuPressed) {
		menuPressed = true;
		MessageController::getInstance()->send(SendingType::MENU_PRESSED);
	}

	if (digitalRead(MENU) == HIGH && menuPressed) {
		menuPressed = false;
	}

	if (digitalRead(OK) == LOW && !okPressed) {
		okPressed = true;
		MessageController::getInstance()->send(SendingType::OK_PRESSED);
	}

	if (digitalRead(OK) == HIGH && okPressed) {
		okPressed = false;
	}
}

void KeyboardScanner::init() {
	downPressed = false;
	upPressed = false;
	menuPressed = false;
	okPressed = false;
}