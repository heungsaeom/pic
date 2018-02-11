#include <16F877.h>
#use delay(clock=4000000)
#include "lcd_4bit.c"
#define RTC_SDA  PIN_C4 
#define RTC_SCL  PIN_C3 
#use i2c(master, sda=RTC_SDA, scl=RTC_SCL)

void rtc_get_time() ;
int convert(int k);
int sec,min,hr,k[20],z,k1[20];
int day1,date,month,year;
void main()
{

SETUP_ADC(ADC_OFF);
lcd_init();

while(1)
{

rtc_get_time();



lcd_cmd(0x80);
sprintf(k,"%02d:%02d:%02d",hr,min,sec);
lcd_array(k);
lcd_cmd(0xc0);
sprintf(k1,"%02d:%02d:%02d ",date,month,year,);
lcd_array(k1);
lcd_cmd(0xc9);
switch(day1)
{
case 1: lcd_str("Sunday");
break;
case 2: lcd_str("Monday");
break;
case 3: lcd_str("Tuesday");
break;
case 4: lcd_str("Wednesday");
break;
case 5: lcd_str("Thursday");
break;
case 6: lcd_str("Friday");
break;
case 7: lcd_str("Saturday");
break;

}

}
}

void rtc_get_time() 
{ 
  i2c_start(); 
  i2c_write(0xD0); 
  i2c_write(0x00);       
  i2c_start(); 
  i2c_write(0xD1); 
  sec = i2c_read(); 
  min = i2c_read(); 
  hr  = i2c_read(); 
 day1=i2c_read();
 date=i2c_read();
 month=i2c_read();
 year=i2c_read(0);
  i2c_stop(); 
  
 sec= convert(sec);
 min= convert(min);
 hr= convert(hr);
 day1= convert(day1);
 date= convert(date);
 month= convert(month);
 year= convert(year);
 z='a';
if(hr>12)
{
hr=hr-12;
z='p';
}

} 

int convert(int k)
{
int a0,a1,a;
 a0=((k&0x0F));
 a1=k>>4;
  a=((a1*10)+a0);
  return a;
}
