#include <16C74.h>
#device adc=8

#FUSES NOWDT                 	//No Watch Dog Timer
#FUSES LP                    	//Low power osc < 200 khz
#FUSES NOPUT                 	//No Power Up Timer
#FUSES PROTECT               	//Code protected from reads

#use delay(clock=20000000)

