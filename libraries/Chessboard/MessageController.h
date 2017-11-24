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
	CAPTURE,
	SCAN_BOARD,
	RESET_BOARD
};

class MotorsControllerMessageProtocol {
public:
	virtual void onMoveRequest(Position from, Position to) = 0;
	virtual void onResetRequest() = 0;
};

class BoardScannerMessageProtocol {
public:
	virtual void onScanRequest() = 0;
};

class DisplayControllerMessageProtocol {
public:	
	virtual void onPrintRequest(int line, const String& content) = 0;
	virtual void onClearScreenRequest() = 0;
	virtual void onResetRequest() = 0;
};


class MessageController {
public:
	void checkMessage();
	void reply(MessageType type, const String& content);

	void setMotorsControllerMessageDelegate(MotorsControllerMessageProtocol* delegate);
	void setBoardScannnerMessageDelegate(BoardScannerMessageProtocol* delegate);
	void setDisolayControllerMessageDelegate(DisplayControllerMessageProtocol* delegate);

	static MessageController* getInstance();

private:	
	void init();
	void translateMessage();

	String commandBuffer;

	MotorsControllerMessageProtocol* motorsDelegate;
	BoardScannerMessageProtocol* scannerDelegate;
	DisplayControllerMessageProtocol* displayerDelegate;

	MessageController() {}
	static MessageController* instance;
};

#endif