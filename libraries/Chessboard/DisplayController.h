#ifndef __DisplayController__
#define __DisplayController__

class DisplayController {
public:
	void clear();
	void print(int line, const char* content);

	static DisplayController* getInstance();

private:
	void init();

	DisplayController() {}
	static DisplayController* instance;
};

#endif