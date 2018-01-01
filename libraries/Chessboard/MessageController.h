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
	MOVE,
	CASTLING,
	SCAN_BOARD,
	RESET_BOARD,
	PLAY_SOUND
};

enum ServiceResponseType {
	MOVE_FAILED,
	MOVE_DONE,
	SCAN_DONE,
	RESET_DONE
};

enum EventType {
	SYSTEM_READY,
	BOARD_CHANGED
};

class MessageController {
public:
	void checkMessage();
	void reply(ServiceResponseType type, String content = "");
	void send(EventType type, String content = "");

	void setMotorsControllerMessageDelegate(MotorsControllerMessageProtocol* delegate);
	void setSwitchesControllerMessageDelegate(SwitchesControllerMessageProtocol* delegate);
	void setSpeakerControllerMessageDelegate(SpeakerControllerMessageProtocol* delegate);

	static MessageController* getInstance();

private:	
	void init();
	void translateMessage();

	String commandBuffer;

	MotorsControllerMessageProtocol* motorsDelegate;
	SwitchesControllerMessageProtocol* switchesDelegate;
	SpeakerControllerMessageProtocol* speakerDelegate;

	MessageController() {}
	static MessageController* instance;
};

#endif
