#include <sys/io.h>
#include <stdio.h>
#include <errno.h>

#define INDEXREG 0xcf8
#define RESULTREG 0xcfc

int main() {
	//1 bit of port access bits, gives you access to 1 byte of the port
	//we need 32bits --> 4 access bits
	//not in the man pages, found out by testing
	int b = ioperm(INDEXREG, 4, 1); //set port access bits for pci-system
	if(b < 0) {
		perror("ioperm failed for pci-system");
		return -1;
	}
	int c = ioperm(RESULTREG, 4, 1); //set port access bits for result
	if(c < 0) {
		perror("ioperm failed for result");
		return -1;
	}

	printf("all privileges are set!\n");
	//privileges are set!

	//variable to write to pci index register
	unsigned int value = 0x80000000; //bit 31 needs to be 1
	//loop through all bus numbers
	int bus = 0, device = 0, function = 0;
	for(bus = 0; bus < 256; bus++) {
		//set bus bits
		value &= 0xFF00FFFF; //clear previous bits
		int busmask = bus << 16;
		value |= busmask;//set bus bits
		//loop through all device numbers
		for(device = 0; device < 33; device++) {
			//set device bits
			value &= 0xFFFF07FF; // clear 5 MSB of 3rd byte
			int devicemask = device << 11;
			value |= devicemask; //set device bits
			//loop through all function numbers
			for(function = 0; function < 8; function++) {
				//make new value
				//set function bits
				value &= 0xFFFFF800; //clear 3 LSB of 3rd byte
				int functionmask = function << 8;
				value |= functionmask;
				//test print
				//write 32bit number to register at 0xcf8
				outl(value, INDEXREG);
				//read 32 bit number from result register
				int result = inl(RESULTREG);
				//check if not 0xFFFFF
				if(result != 0xFFFFFFFF) {
					printf("bus: %X, device: %X, function: %X, value: %X\n", bus, device, function, value);
					//device = 16 MSB
					int deviceID = (0xFFFF0000 & result) >> 16;
					//vendor = 16 LSB
					int vendorID = (0x0000FFFF & result);
					printf("vendorID: %X, deviceID: %X\n", vendorID, deviceID);
				}
			}
		}
	}
	return 0;
}