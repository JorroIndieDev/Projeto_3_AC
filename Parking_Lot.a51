
; Atuadores
Verde       EQU P1.0
Vermelho    EQU P1.1
Aamerlo     EQU P1.2
Barreira	EQU P1.3

; Sensores
Botao_1		EQU P3.2
Botao_2		EQU P3.3
Sensor		EQU P1.4

; Display
Display_A	EQU	P2.0
Display_B	EQU	P2.1
Display_C	EQU	P2.2
Display_D	EQU	P2.3
	
; Sensores de presenca e sinalizacao
S1			EQU P0.0
S2			EQU P0.1
S3			EQU P0.2
S4			EQU P0.3
S5			EQU P0.4
S6			EQU P0.5
S7			EQU P0.6
S8			EQU P0.7


org	0000h
Inicializacao:
	; Display 0 at the begining for a clean start
	mov Display_A, 00h
	mov Display_B, 00h
	mov Display_C, 00h
	mov Display_D, 00h
	
	mov S1, 0ffh
	mov S2, 0ffh
	mov S3, 0ffh
	mov S4, 0ffh
	mov S5, 0ffh
	mov S6, 0ffh
	mov S7, 0ffh
	mov S8, 0ffh
	