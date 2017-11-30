#include <KeyboardScanner.h>

#include <Arduino.h>

#include <MessageController.h>

KeyboardScanner* KeyboardScanner::instance = NULL;

#define MENU_PIN 2
#define UP_PIN 14
#define DOWN_PIN 15
#define OK_PIN 3

KeyboardScanner* KeyboardScanner::getInstance() {
	if (!instance) {
		instance = new KeyboardScanner();
		instance->init();
	}

	return instance;
}

void KeyboardScanner::scan() {
	int value = 0;

	if (digitalRead(DOWN_PIN) == LOW && !downPressed) {
		downPressed = true;
		MessageController::getInstance()->send(DOWN_PRESSED);
	}

	if (digitalRead(DOWN_PIN) == HIGH && downPressed) {
		downPressed = false;
	}

	if (digitalRead(UP_PIN) == LOW && !upPressed) {
		upPressed = true;
		MessageController::getInstance()->send(UP_PRESSED);
	}

	if (digitalRead(UP_PIN) == HIGH && upPressed) {
		upPressed = false;
	}

	if (digitalRead(MENU_PIN) == LOW && !menuPressed) {
		menuPressed = true;
		MessageController::getInstance()->send(MENU_PRESSED);
	}

	if (digitalRead(MENU_PIN) == HIGH && menuPressed) {
		menuPressed = false;
	}

	if (digitalRead(OK_PIN) == LOW && !okPressed) {
		okPressed = true;
		MessageController::getInstance()->send(OK_PRESSED);
	}

	if (digitalRead(OK_PIN) == HIGH && okPressed) {
		okPressed = false;
	}
}

void KeyboardScanner::init() {
	downPressed = false;
	upPressed = false;
	menuPressed = false;
	okPressed = false;
}