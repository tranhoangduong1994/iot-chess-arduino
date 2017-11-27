#ifndef __MessageController__
#define __MessageController__

#include <Arduino.h>
#include <string.h>

#include <DataTypes.h>

#include <MessageControllerProtocols.h>

enum MessageType {
    ServiceRequest,
    ServiceResponse,
    Event
};

enum ServiceRequestType {
	CLEAR_SCREEN,
	PRINT,
	MOVE,
	SCAN_BOARD,
	RESET_BOARD
};

enum ServiceResponseType {
	CLEAR_SCREEN_DONE,
	PRINT_DONE,
	MOVE_FAILED,
	MOVE_DONE,
	SCAN_DONE,
	RESET_DONE
};

enum EventType {
	BOARD_CHANGED,
	UP_PRESSED,
	DOWN_PRESSED,
	MENU_PRESSED,
	OK_PRESSED
};

class MessageController {
public:
	void checkMessage();
	void reply(ServiceResponseType type, String content = "");
	void send(EventType type, String content = "");

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