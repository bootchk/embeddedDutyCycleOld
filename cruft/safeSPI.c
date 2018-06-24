
#ifdef NOTUSED
#include "safeSPI.h"

// A
#include <SPI.h>

void clearRegister(unsigned char registerAddress){
	// Write one byte of zero to registerAddress
	SPI::write(registerAddress, 1, 0);
}

bool SafeSPI::clearBit(){
	SPI::write();
	SPI::read();
	return (read == write);

}

#endif
