#include <reg51.h>


#define second 5000	// 5000 ticks * 200us = 1s
#define servoZero 3		// servo zero, starting pos = 3 * 200us = 600us
#define downTime 10	// 10 ticks * 200us = 20ms
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
unsigned int Free;	// Free slots in the parking lot
unsigned int i;	// increment for counter in for loop (errors occur if the variable is declared inside the loop)


void awaitCar(void);
void delayTicks(unsigned int ticks);
void blinker();

void main(void){
	// _init_ the pins
	P0 = 0xff;
	P1 = 0xff;
	P2 = 0x00;
	P3 = 0xff;
	delayTicks(servoZero);
	Barr = 0;
	
	while(1){		// main loop
		park_states = P0; // assign P0 to a readable var
		
		// verify the available parking spots and sends it o the Display
		Free = 0;
		
		for(i = 0; i < 8; i++){	// loop through the 8 slots and increment occupied if any 0 is detected
			// if park_states and bit[i], then a spot is occupied and 
			// i is the occupied slot.
			if(park_states & (1 << i)){	// park_states AND 1 << [i] <- i.e (0000 0100) & (1 << [3]) = true (1 << [3]) = 100
				Free += 1;
			}
		}

		P2 = Free; // This sends only the 4 bits necessary for the Display A-D
		// at this point in the program runing the first time no alteratino to any input the display pins should be 0000 1000 = 8
		// if there are any parking slots available Green else Red
		if(Free != 0){
			Green = 0;
			Red = 1;
		} else {
			Green = 1;
			Red = 0;
		}

		// assuming from the assignment there is only one way to enter / exit
		// all buttons in the diagram activate on a low(0) so !Bot
		if((Bot_1 == 0 )& !Green){	// if button 1 -> entry Light must be green also
			awaitCar();
		} else if(Bot_2 == 0){	// if button 2 -> exit light does not matter 
			awaitCar();
		}
		
	}
	
}

void awaitCar(void){
	while(Sensor == 1);// await sensor response
	Barr = 1;
	while(Sensor == 0){blinker();}// if there is a car there wait
	for(i = 0;i < 10;i++){
		delayTicks(second);
	}
	Barr = 0;
	Yellow = 1;

}

void blinker(void){
	Yellow = 0;
	delayTicks(downTime);
	Yellow = 1;
}

void delayTicks(unsigned int ticks) {
    unsigned int i;
    
    TMOD = (TMOD & 0xF0) | 0x01;  // Use Timer0 in Mode 1 (16-bit)
	
    for (i = 0; i < ticks; i++) {
        // Each loop = 200us delay
        // Timer0 value = 65536 - 200 = 65336 ? 0xFF38
				TH0 = 0xFF;  // High byte
        TL0 = 0x38;  // Low byte (0xFF38 = 65528 = 8 ticks, but close enough for 200us with 12MHz)

        TF0 = 0;     // Clear overflow flag
        TR0 = 1;     // Start Timer0

        while (!TF0); // Wait for timer overflow

        TR0 = 0;     // Stop Timer0
        TF0 = 0;     // Clear flag again (optional)
    }
}
