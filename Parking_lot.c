#include <reg51.h>

// Actuatures
sbit Green = P1^0;
sbit Red = P1^1;
sbit Yellow = P1^2;
sbit Barr = P1^3;

// Sensors
sbit Bot_1 = P3^2;
sbit Bot_2 = P3^3;
sbit Sensor = P1^4;

// Display
sbit A = P2^0;
sbit B = P2^1;
sbit C = P2^2;
sbit D = P2^3;

// Sensor and LEDs on the parking spots
sbit S1_L1 = P0^0;
sbit S2_L2 = P0^1;
sbit S3_L3 = P0^2;
sbit S4_L4 = P0^3;
sbit S5_L5 = P0^4;
sbit S6_L6 = P0^5;
sbit S7_L7 = P0^6;
sbit S8_L8 = P0^7;

int void main() {
	// _init_ the pins
	P0 = 0xff;
	P1 = 0xff;
	P2 = 0xff;
	P3 = 0xff;
	
	unsigned char park_states;	// unsingned char 8 bits, meaning P0.0 .. P0.7
	unsigned char occupied;
	unsigned char available _park;
	
	while(1) {		// main loop
		park_states = P0; // assign P0 to a readable var
		
		// Verify the available parking spots and sends it o the Display
		occupied = 0;
		
		for(unsigned char i = 0; i < 8; i++) {
			// if park_states and bit[i], then a spot is occupied and 
			// i is the occupied slot.
			if(park_states & (1 << i)) {
				occupied++;
			}
		}

		available_park = 8 - occupied;	// results in the display code for the decoder
		available_park &= 0x0f;	// keeps at 4 bits since the full 8 are not used
		P2 = available_park; // This sends only the 4 bits necessary for the Display A-D
		
		// if there are any parking slots available Green else Red
		if(available_park > 0) {
			Green = 1;
			Red = 0;
		} else {
			Green = 0;
			Red = 1;
		}
	
		if(Sensor & !Red)
		
	}
	
}