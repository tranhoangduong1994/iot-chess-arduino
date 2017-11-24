#ifndef __KeyboardScanner__
#define __KeyboardScanner__

enum Key {
	KEY_DOWN,
	KEY_UP,
	KEY_MENU,
	KEY_OK
};

class KeyboardScanner {
public:
	static KeyboardScanner* getInstance();
	Key scan();

private:
	bool downPressed;
	bool upPressed;
	bool menuPressed;
	bool okPressed;

	void init();

	KeyboardScanner() {}
	static KeyboardScanner* instance;
};

#endif