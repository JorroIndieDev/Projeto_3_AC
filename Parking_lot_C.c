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
sbit Display_A = P2^0;
sbit Display_B = P2^1;
sbit Display_C = P2^2;
sbit Display_D = P2^3;

// Sensor and LEDs on the parking spots
sbit S1_L1 = P0^0;
sbit S2_L2 = P0^1;
sbit S3_L3 = P0^2;
sbit S4_L4 = P0^3;
sbit S5_L5 = P0^4;
sbit S6_L6 = P0^5;
sbit S7_L7 = P0^6;
sbit S8_L8 = P0^7;

unsigned int park_states;	// unsingned chr 8 bits, meaning P0.0 .. P0.7
unsigned int occupied;	// occupied slots in the parking lot
unsigned int available_park;	// how many slots are not occupied
unsigned int i;	// increment for counter in for loop (errors occur if the variable is declared inside the loop)


void awaitCar();
void delay_10s();

void main(){
	// _init_ the pins
	P0 = 0xff;
	P1 = 0xff;
	P2 = 0xff;
	P3 = 0xff;
	
	while(1){		// main loop
		park_states = P0; // assign P0 to a readable var
		
		// verify the available parking spots and sends it o the Display
		occupied = 0;
		
		for(i = 0; i < 8; i++){	// loop through the 8 slots and increment occupied if any 0 is detected
			// if park_states and bit[i], then a spot is occupied and 
			// i is the occupied slot.
			if(park_states & (1 << i)){	// park_states AND 1 << [i] <- i.e (0000 0100) & (1 << [3]) = true (1 << [3]) = 100
				occupied++;
			}
		}

		available_park = 8 - occupied;	// results in the display code for the decoder
		available_park &= 0x0f;	// keeps at 4 bits since the full 8 are not used (a sort of clamp)
		P2 = available_park; // This sends only the 4 bits necessary for the Display A-D
		// at this point in the program runing the first time no alteratino to any input the display pins should be 0000 1000 = 8
		// if there are any parking slots available Green else Red
		if(available_park > 0){
			Green = 1;
			Red = 0;
		} else {
			Green = 0;
			Red = 1;
		}
		// assuming from the assignment there is only one way to enter / exit
		// all buttons in the diagram activate on a low(0) so !Bot
		if(!Bot_1 & Green){	// if button 1 -> entry Light must be green also
			awaitCar();
		} else if(!Bot_2){	// if button 2 -> exit light does not matter 
			awaitCar();
		}
		
	}
	
}

void awaitCar(){
	Barr = 1;
	Yellow = 1;
	while(!Sensor);// await sensor response
	delay_10s();
	Barr = 0;
	Yellow = 0;

}

void delay_10s(){
	unsigned int intervals = 200;	// 200 & 50ms = 10s
	
	TMOD = (TMOD & 0xf0) | 0x01;	// configuration of the built in timer 0
	
	while(intervals > 0){
		// 0x3cb0
		TH0 = 0x3c;	// low byte
		TL0 = 0xb0;	// high byte
		
		TR0 = 1; // start the timer
		
		while(!TF0); // await overflow ?
		
		TF0 = 0;
		TR0 = 0;
		
		intervals--;
	}
}