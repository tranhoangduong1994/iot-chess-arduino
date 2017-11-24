#ifndef __DisplayController__
#define __DisplayController__

class DisplayController : public DisplayControllerMessageProtocol {
public:
	void clear();
	void print(int line, const char* content);

	void onPrintRequest(int line, String content) override;
	void onClearScreenRequest() override;

	static DisplayController* getInstance();

private:
	void init();

	DisplayController() {}
	static DisplayController* instance;
};

#endif