#include <DisplayController.h>

#include <LiquidCrystal_I2C.h>

#include <MessageController.h>

DisplayController* DisplayController::instance = NULL;

const char* clearingString = "                    ";

DisplayController* DisplayController::getInstance() {
	if (!instance) {
		instance = new DisplayController();
		instance->init();
	}

	return instance;
}

void DisplayController::init() {
	// set the LCD address to 0x20 for a 20 chars 4 line display
	// Set the pins on the I2C chip used for LCD connections:
	//                    addr, en,rw,rs,d4,d5,d6,d7,bl,blpol
	lcd = new LiquidCrystal_I2C(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);// Set the LCD I2C address
	lcd->begin(20,4);

	MessageController::getInstance()->setDisplayControllerMessageDelegate(this);
}

void DisplayController::clear() {
	lcd->clear();
}

void DisplayController::print(int line, String content) {
	lcd->setCursor(0, line);
	lcd->print(clearingString);
	lcd->setCursor(0, line);
	lcd->print(content);
}

void DisplayController::onPrintRequest(int line, String content) {
	print(line, content);
	MessageController::getInstance()->reply(PRINT_DONE);
}

void DisplayController::onClearScreenRequest() {
	clear();
	MessageController::getInstance()->reply(CLEAR_SCREEN_DONE);
}
