#ifndef __SwitchesController__
#define __SwitchesController__

#include <Bitboard.h>
#include <MessageControllerProtocols.h>

class SwitchesController : public SwitchesControllerMessageProtocol {
public:
	void scan();
	Bitboard getCurrentState();

	void onScanRequest() override;
	void onResetRequest() override;
	
	static SwitchesController* getInstance();

private:
	void init();

	Bitboard bitboard;

	unsigned long lastChangedTime;
	bool changed;

	SwitchesController() {}
	static SwitchesController* instance;
};

#endif