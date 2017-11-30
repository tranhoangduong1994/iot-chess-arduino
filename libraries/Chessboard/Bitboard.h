#ifndef __Bitboard__
#define __Bitboard__

struct Bitboard {
	unsigned long first32;
	unsigned long last32;

	Bitboard() {
		first32 = 0;
		last32 = 0;
	}

	void set(int index, unsigned long value) {
		if (index > 63 || index < 0) {
			return;
		}

		if (index > 31) {
			last32 &= ~(value << (index - 32));
			last32 |= (value << (index - 32));
			return;
		}

		first32 &= ~(value << index);
		first32 |= (value << index);
	}

	const char* toString() {
		static char string[65] = {0};
		unsigned long f32 = first32, l32 = last32;

	    for (int i = 0; i < 32; i++) {
	        string[i] = (f32 % 2 ? '1' : '0');
	        f32 = f32 >> 1;

	        string[i + 32] = (l32 % 2 ? '1' : '0');
	        l32 = l32 >> 1;
	    }

	    return string;
	}

	Bitboard getChangedBoard(const Bitboard& other) {
		Bitboard result;
		result.first32 = this->first32 ^ other.first32;
		result.last32 = this->last32 ^ other.last32;
		return result;
	}

	bool operator== (const Bitboard& other) {
		return this->first32 == other.first32 && this->last32 == other.last32;
	}

	bool operator!= (const Bitboard& other) {
		return this->first32 != other.first32 || this->last32 != other.last32;
	}

	bool getBitByIndex(int index) {
		if (index > 31) {
			return last32 & (1 << (index - 1));
		}

		return first32 & (1 << (index - 1));
	}
};

#endif