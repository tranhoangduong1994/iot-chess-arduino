#ifndef __SwitchesController__
#define __SwitchesController__

#include <Bitboard.h>

class SwitchesController : public SwitchesControllerMessageProtocol {
public:
	void scan();
	const &Bitboard getCurrentState();

	void onScanRequest() override;
	
	static SwitchesController* getInstance();

private:
	void init();

	Bitboard bitboard;

	SwitchesController() {}
	static SwitchesController* instance;
};

#endif