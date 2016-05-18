#ifndef BIT_H_
#define BIT_H_

unsigned char GetBit(unsigned char bits, unsigned char numShifts) {
	unsigned char mask = 0x01 << numShifts;
	return ((bits & mask) != 0);
}

#endif /* BIT_H_ */