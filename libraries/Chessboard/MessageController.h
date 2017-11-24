#ifndef __MessageController__
#define __MessageController__

#include <Arduino.h>
#include <string.h>

#include <DataTypes.h>

enum MessageType {
	NONE,
	CLEAR_SCREEN,
	PRINT,
	MOVE,
	SCAN_BOARD,
	RESET_BOARD
};

enum ReplyingType {
	CLEAR_SCREEN_DONE,
	PRINT_DONE,
	MOVE_FAILED,
	MOVE_DONE,
	SCAN_DONE,
	RESET_DONE
};

enum SendingType {
	BOARD_CHANGED,
	UP_PRESSED,
	DOWN_PRESSED,
	MENU_PRESSED,
	OK_PRESSED
}

class MotorsControllerMessageProtocol {
public:
	virtual void onMoveRequest(Position from, Position to) = 0;
	virtual void onResetRequest() = 0;
};

class SwitchesControllerMessageProtocol {
public:
	virtual void onScanRequest() = 0;
};

class DisplayControllerMessageProtocol {
public:	
	virtual void onPrintRequest(int line, String content) = 0;
	virtual void onClearScreenRequest() = 0;
};


class MessageController {
public:
	void checkMessage();
	void reply(ReplyingType type, String content = "");
	void send(SendingType type, String content = "");

	void setMotorsControllerMessageDelegate(MotorsControllerMessageProtocol* delegate);
	void setSwitchesControllerMessageDelegate(SwitchesControllerMessageProtocol* delegate);
	void setDisplayControllerMessageDelegate(DisplayControllerMessageProtocol* delegate);

	static MessageController* getInstance();

private:	
	void init();
	void translateMessage();

	String commandBuffer;

	MotorsControllerMessageProtocol* motorsDelegate;
	SwitchesControllerMessageProtocol* switchesDelegate;
	DisplayControllerMessageProtocol* displayerDelegate;

	MessageController() {}
	static MessageController* instance;
};

#endif