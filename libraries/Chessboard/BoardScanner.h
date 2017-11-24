#ifndef __BoardScanner__
#define __BoardScanner__

struct Bitboard {
	unsigned long first32;
	unsigned long last32;

	Bitboard() {
		first32 = 0;
		last32 = 0;
	}

	void set(int bitNumber, unsigned long value) {
		if (bitNumber > 63 || bitNumber < 0) {
			return;
		}

		if (bitNumber > 31) {
			last32 &= ~(value << (bitNumber - 32));
			last32 |= (value << (bitNumber - 32));
			return;
		}

		first32 &= ~(value << bitNumber);
		first32 |= (value << bitNumber);
	}

	const char* toString() {
		static char string[64] = {'0'};
		unsigned long f32 = first32, l32 = last32;

	    for (int i = 0; i < 32; i++) {
	        string[i] = (f32 % 2 ? '1' : '0');
	        f32 = f32 >> 1;

	        string[i + 32] = (l32 % 2 ? '1' : '0');
	        l32 = l32 >> 1;
	    }

	    return string;
	}

	Bitboard compare(const Bitboard& other) {
		Bitboard result;
		result.first32 = this->first32 ^ other.first32;
		result.last32 = this->last32 ^ other.last32;
		return result;
	}
};

class BoardScanner {
public:
	Bitboard scan();

	static BoardScanner* getInstance();
private:
	void init();

	BoardScanner() {}
	static BoardScanner* instance;
};

#endif