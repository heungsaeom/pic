#include <16F887.h>//chon chip lam viec
#device ADC=16//do phan giai adc

#FUSES NOWDT                    //No Watch Dog Timer
#FUSES NOBROWNOUT               //No brownout reset
#FUSES NOLVP                    //No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O

#use delay(crystal=4000000)//thach anh hoat dong la 4Mhz
#include <lcd.h>//chen thu vien lcd

#define MFRC522_CS  PIN_E2                 
#define MFRC522_SCK PIN_C3
#define MFRC522_SI  PIN_C4                           
#define MFRC522_SO  PIN_C5              
#define MFRC522_RST PIN_E0   
#include <Built_in.h> 

