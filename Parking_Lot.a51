
; Atuadores
Green       BIT P1.0
Red    		BIT P1.1
Yellow     	BIT P1.2
Barr		BIT P1.3

; Sensores
Bot_1		BIT P3.2
Bot_2		BIT P3.3
Sensor		BIT P1.4

; Display
Display_A	BIT	P2.0
Display_B	BIT	P2.1
Display_C	BIT	P2.2
Display_D	BIT	P2.3
	
; Sensores de presenca e sinalizacao
S1			BIT P0.0
S2			BIT P0.1
S3			BIT P0.2
S4			BIT P0.3
S5			BIT P0.4
S6			BIT P0.5
S7			BIT P0.6
S8			BIT P0.7


org	0000h
InitVals:
	mov 	P0, 0ffh			;Set the pins of the parking spots to 0
	mov 	P1, 0ffh			;Actuators set to 0
	mov 	P2, 0ffh 			;Display 0 at the begining
	mov 	P3, 0ffh			;Interaction Peripherals set to 0

MainLoop:
	mov 	A, P0			;Park_states(accumulator)
	mov 	r0, 0			;Occupied
	
countOccupied:
	mov 	r1, 8			;Number of slots available
countLoop:
	rl 		A				;Rotate left to verify each bit L to R
	jnc 	nextBit			;Skip if the bit is 0
	inc 	r0				;Increment r0 occupied
nextBit:
	djnz 	r1, countLoop	;Decrement r1 and jump to loop if not 0
	
	mov 	A, 8			;8 slots to park
	clr 	C				;Clear the carry bit from RL
	subb	A, r0			;Subtract r0 occupied from A 8 slots to get available_park
	mov 	P2, A			;Present Display
	
	jz 		FullPark	;If there are no slots its full jump to routine to handle

FreePark:
	setb 	Green			;Green light on
	clr 	Red				;Red light off
	jmp 	testButtons		;Verify if the buttons are pressed
FullPark:
	clr 	Green			;Green light off
	setb 	Red				;Red light on
	jmp 	testButtons		;Verify if the buttons are pressed
		
testButtons:
testBot1:
	jnb		Bot_1, testBot2	;Test button2 if button1 is off
	jnb		Green, LoopBack	;Ignore the rest if the Green Light is not one (meaning Red light is on, no space)
	jmp		awaitCar		;Await the car to enter the sensor range
testBot2:
	jnb		Bot_2, LoopBack	;Ignore if button2 is not pressed
	jmp		awaitCar		;Await the car to enter the sensor range
LoopBack:
	jmp 	MainLoop		;Loop back to continue reading the signals

awaitCar:					;Routine to detect if the car is in the sensor range to pass the barrier
	setb	Barr			;Set barrier on
	setb	Yellow			;Yellow light on
	jnb		Sensor, $		;Await sensor
	; Delay 10s
	lcall	delay10s
	clr		Barr			;Set barrier off
	clr		Yellow			;Yellow light off
	jmp		LoopBack
	
delay10s:
    push 	ACC         	;Save accumulator
    push 	PSW         	;Save program status word
    mov 	R2, 200     	;200 intervals of 50 ms
delayLoop:
    mov 	TMOD, 01h   	;Timer 0, Mode 1
    mov 	TH0, 0x3C   	;Load 50 ms delay
    mov 	TL0, 0xB0
    setb 	TR0         	;Start Timer 0
    jnb 	TF0, $       	;Wait for overflow
    clr 	TF0          	;Clear overflow flag
    clr 	TR0          	;Stop Timer 0
    djnz 	R2, delayLoop  	;Decrement and loop if not zero
    pop 	PSW          	;Restore PSW
    pop 	ACC          	;Restore accumulator
    RET

END