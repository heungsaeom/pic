#include <16f877a.h>
#fuses HS,NOWDT
#use delay(clock=20000000)
#include <HDM64GS12.c>
#include <graphics.c>
#use standard_io(a)
void main (void)
{
CHAR A5 [ ] = "A5";
CHAR A4 [ ] = "A4";
CHAR A3 [ ] = "A3";
CHAR A2 [ ] = "A2";
CHAR A1 [ ] = "A1";
CHAR A0 [ ] = "A0";
CHAR IN [ ] = "SEC A";
glcd_init(ON);
glcd_text57(33,30,A5,1,1);
glcd_text57(49,30,A4,1,1);
glcd_text57(65,30,A3,1,1);
glcd_text57(81,30,A2,1,1);
glcd_text57(97,30,A1,1,1);
glcd_text57(113,30,A0,1,1);
glcd_text57(30,5,IN,2,1);
while(1)
{
if(input_state(PIN_A5)==0)
glcd_rect(32,40,46,60,1,1);
else
glcd_rect (30,40,46,60,1,0,);
glcd_rect(32,40,46,60,0,1);
//
if(input_state(PIN_A4)==0)
glcd_rect(48,40,62,60,1,1);
else
glcd_rect (48,40,62,60,1,0,);
glcd_rect(48,40,62,60,0,1);
//
if(input_state(PIN_A3)==0)
glcd_rect(64,40,78,60,1,1);
else
glcd_rect (64,40,78,60,1,0,);
glcd_rect(64,40,78,60,0,1);
//
if(input_state(PIN_A2)==0)
glcd_rect(80,40,94,60,1,1);
else
glcd_rect (80,40,94,60,1,0,);
glcd_rect(80,40,94,60,0,1);
//
if(input_state(PIN_A1)==0)
glcd_rect(96,40,110,60,1,1);
else
glcd_rect (96,40,110,60,1,0,);
glcd_rect(96,40,110,60,0,1);
//
if(input_state(PIN_A0)==0)
glcd_rect(112,40,126,60,1,1);
else
glcd_rect (112,40,126,60,1,0,);
glcd_rect(112,40,126,60,0,1);
delay_ms(400);
}
}
