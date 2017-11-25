#ifndef __MessageControllerProtocols__
#define __MessageControllerProtocols__

#include <DataTypes.h>
#include <Arduino.h>

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

#endif