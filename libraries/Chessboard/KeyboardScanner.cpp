#include <KeyboardScanner.h>

#include <Arduino.h>

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

Key KeyboardScanner::scan() {
	int value = 0;

	if (digitalRead(DOWN) == LOW && !downPressed) {
		downPressed = true;
		return Key::KEY_DOWN;
	}

	if (digitalRead(DOWN) == HIGH && downPressed) {
		downPressed = false;
	}

	if (digitalRead(UP) == LOW && !upPressed) {
		upPressed = true;
		return Key::KEY_UP;
	}

	if (digitalRead(UP) == HIGH && upPressed) {
		upPressed = false;
	}

	if (digitalRead(MENU) == LOW && !menuPressed) {
		menuPressed = true;
		return Key::KEY_MENU;
	}

	if (digitalRead(MENU) == HIGH && menuPressed) {
		menuPressed = false;
	}

	if (digitalRead(OK) == LOW && !okPressed) {
		okPressed = true;
		return Key::KEY_OK;
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