#ifndef BIT_H_
#define BIT_H_

unsigned char GetBit(unsigned char bits, unsigned char numShifts) {
	unsigned char mask = 0x01 << numShifts;
	return ((bits & mask) != 0);
}

unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
	return (b ?  (x | (0x01 << k))  :  (x & ~(0x01 << k)));
}

#endif /* BIT_H_ */