#ifndef __DisplayController__
#define __DisplayController__

#include <MessageControllerProtocols.h>
#include <Arduino.h>

class LiquidCrystal_I2C;

class DisplayController : public DisplayControllerMessageProtocol {
public:
	void clear();
	void print(int line, String content);

	void onPrintRequest(int line, String content) override;
	void onClearScreenRequest() override;

	static DisplayController* getInstance();

private:
	LiquidCrystal_I2C* lcd;
	void init();

	DisplayController() {}
	static DisplayController* instance;
};

#endif