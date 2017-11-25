#ifndef __DisplayController__
#define __DisplayController__

#include <MessageControllerProtocols.h>
#include <Arduino.h>

class DisplayController : public DisplayControllerMessageProtocol {
public:
	void clear();
	void print(int line, String content);

	void onPrintRequest(int line, String content) override;
	void onClearScreenRequest() override;

	static DisplayController* getInstance();

private:
	void init();

	DisplayController() {}
	static DisplayController* instance;
};

#endif