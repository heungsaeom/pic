#include <REGX52.H>

#define SEC    	0x00
#define MIN    	0x01
#define HOUR   	0x02
#define DAY    	0x03
#define DATE   	0x04
#define MONTH  	0x05
#define YEAR   	0x06
#define CONTROL 0x07
#define A_MIN  	0x08
#define A_HOUR 	0x09
#define A_MIN1  0x0A
#define A_HOUR1 0x0B
#define ON_OFF  0x0C
#define ON_OFF1 0x0D

char DATEAL,MONTHAL,YEARAL,sec,hour,min,day,date,month,year,year_20,yearal_20,a_min,a_hour,a_min1,a_hour1;

sbit SDA   = P1^0;
sbit SCL   = P1^1;
sbit Set   = P1^2;
sbit Up    = P1^3;
sbit Down  = P1^4;
sbit Ok    = P1^5;
sbit Alarm = P1^6;
sbit Loa   = P1^7;
sbit E11   = P3^0;
sbit E12   = P3^1;
sbit AM    = P3^2;
sbit PM    = P3^3;
//******************************************************************************
// KHAI BAO BIEN, MANG... 
unsigned char led7_data[11] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x39}; //Decimal to Led7seg
unsigned char Data,Display,time,index,set,bat_nhay,read_time;
unsigned char AM_PM,alarm,alarm1,on_off,on_off1;
//,,,,,,,,,,,,,,,,,,,,KHAI BAO MANG AM DUONG LICH,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
unsigned char code DL2010[48]={17,11,15,12,18,12,14,1,16,1,16,2,17,2,14,3,18,3,14,4,19,4,12,5,
                               20,5,12,6,21,6,10,7,23,7,8,8,24,8,8,9,25,9,6,10,26,10,6,11};
unsigned char code DL2011[48]={27,11,4,12,19,12,3,1,27,1,5,2,28,2,3,3,29,3,3,4,30,4,2,5,
                               1,6,31,7,2,7,29,8,4,8,27,9,5,9,27,10,6,10,25,11,7,11,25,12};
unsigned char code DL2012[48]={8,12,23,1,10,1,22,2,9,2,22,3,11,3,21,4,11,4,21,4,12,4,19,5,	 
					           13,5,19,6,14,6,17,7,16,7,16,8,16,8,15,9,18,9,14,10,18,10,13,11};   
unsigned char code DL2013[48]={20,11,12,12,21,12,10,1,20,1,12,2,21,2,10,3,22,3,10,4,23,4,8,5,
							   24,5,8,6,25,6,7,7,26,7,5,8,27,8,5,9,28,9,3,10,29,10,3,11};
unsigned char code DL2014[48]={1,12,31,1,2,1,30,2,1,2,31,3,2,3,29,4,3,4,29,5,4,5,27,6,
                               5,6,27,7,6,7,25,8,8,8,24,9,8,9,24,9,9,9,22,10,10,10,22,11};
unsigned char code DL2015[48]={11,11,20,12,13,12,19,1,11,1,20,2,13,2,19,3,13,3,18,4,15,4,16,5,
                               16,5,16,6,17,6,14,7,19,7,13,8,19,8,13,9,20,9,12,10,20,10,11,11};
unsigned char code DL2016[48]={22,11,10,12,23,12,8,1,23,1,9,2,24,2,7,3,25,3,7,4,26,4,5,5,
                               27,5,4,6,29,6,3,7,1,8,31,9,3,11,29,12,1,9,31,10,2,10,29,11};
unsigned char code DL2017[48]={4,12,28,1,5,1,26,2,4,2,28,3,5,3,26,4,6,4,26,5,7,5,24,6,
                               8,6,23,6,10,6,22,7,11,7,20,8,12,8,20,9,13,9,18,10,14,10,18,11};
unsigned char code DL2018[48]={15,11,17,12,16,12,16,1,14,1,17,2,16,2,16,3,16,3,15,4,18,4,14,5,
                               18,5,13,6,20,6,11,7,22,7,10,8,22,8,9,9,24,9,7,10,25,10,7,11};
unsigned char code DL2019[48]={26,11,6,12,27,12,5,1,25,1,6,2,27,2,5,3,27,3,5,4,28,4,3,5,
                               29,5,3,6,1,7,30,8,3,8,29,9,3,9,28,10,5,10,26,11,6,11,26,12};
unsigned char code DL2020[48]={7,12,25,1,8,1,23,2,8,2,24,3,9,3,23,4,9,4,23,4,10,4,21,5,
                               11,5,21,6,12,6,19,7,14,7,17,8,15,8,17,9,16,9,15,10,17,10,14,11};

// KHAI BAO HAM, CHUONG TRINH CON
void set_main(void); 			// Chuong trinh khoi tao main
void setup_timer(void); 		// Chuong trinh cai dat timer 1 va 0
void delay(unsigned int time); 	// Delay
void Set_Clock(void); 			// Kiem tra va hieu chinh gia tri cua gio,phut,giay
void Keypad(void); 				// Kiem tra xu ly phim nhan.
void T1_ISR(void); 				//ngat timer 1 phuc vu nhay led
void T0_ISR(void); 				// Dung ngat timer 0 de quet led
unsigned char DATE_AL(unsigned char date, unsigned char month, unsigned char year);
unsigned char MONTH_AL(unsigned char date, unsigned char month, unsigned char year);
unsigned char YEAR_AL(unsigned char MONTHAL, unsigned char month);
//******************************************************************************
//*************************CAC CHUONG TRINH TINH AM LICH**************************
unsigned char DATE_AL(unsigned char date, unsigned char month, unsigned char year)
{  
   unsigned char n,i;
   i=month-1;
   if (year==10)
     {  if (date<DL2010[2+(4*i)]) n=(DL2010[4*i]+date)-1;
       else if (date>DL2010[2+(4*i)]) n=(date-DL2010[2+(4*i)])+1;
       else n=1; }
   else if (year==11)
     {  if (date<DL2011[2+(4*i)]) n=(DL2011[4*i]+date)-1;
       else if (date>DL2011[2+(4*i)]) n=(date-DL2011[2+(4*i)])+1;
       else n=1; }
   else if (year==11)
     {  if (date<DL2011[2+(4*i)]) n=(DL2011[4*i]+date)-1;
       else if (date>DL2011[2+(4*i)]) n=(date-DL2011[2+(4*i)])+1;
       else n=1; }
   else if (year==12)
     {  if (date<DL2012[2+(4*i)]) n=(DL2012[4*i]+date)-1;
       else if (date>DL2012[2+(4*i)]) n=(date-DL2012[2+(4*i)])+1;
       else n=1; }
   else if (year==13)
     {  if (date<DL2013[2+(4*i)]) n=(DL2013[4*i]+date)-1;
       else if (date>DL2013[2+(4*i)]) n=(date-DL2013[2+(4*i)])+1;
       else n=1; }
   else if (year==14)
     {  if (date<DL2014[2+(4*i)]) n=(DL2014[4*i]+date)-1;
       else if (date>DL2014[2+(4*i)]) n=(date-DL2014[2+(4*i)])+1;
       else n=1; }
   else if (year==15)
     {  if (date<DL2015[2+(4*i)]) n=(DL2015[4*i]+date)-1;
       else if (date>DL2015[2+(4*i)]) n=(date-DL2015[2+(4*i)])+1;
       else n=1; }
   else if (year==16)
     {  if (date<DL2016[2+(4*i)]) n=(DL2016[4*i]+date)-1;
       else if (date>DL2016[2+(4*i)]) n=(date-DL2016[2+(4*i)])+1;
       else n=1; }
   else if (year==17)
     {  if (date<DL2017[2+(4*i)]) n=(DL2017[4*i]+date)-1;
       else if (date>DL2017[2+(4*i)]) n=(date-DL2017[2+(4*i)])+1;
       else n=1; }
   else if (year==19)
     {  if (date<DL2019[2+(4*i)]) n=(DL2019[4*i]+date)-1;
       else if (date>DL2019[2+(4*i)]) n=(date-DL2019[2+(4*i)])+1;
       else n=1; }
   else if (year==20)
     {  if (date<DL2020[2+(4*i)]) n=(DL2020[4*i]+date)-1;
       else if (date>DL2020[2+(4*i)]) n=(date-DL2020[2+(4*i)])+1;
       else n=1; }
   return n;
   }
 unsigned char MONTH_AL(unsigned char date, unsigned char month, unsigned char year)
 {
   unsigned char n,i;
   i=month-1;
   if (year==10)
	  {    if (date<DL2010[2+(4*i)]) n=DL2010[1+(4*i)];
          else  n=DL2010[3+(4*i)]; }
   else if (year==11)
	  {    if (date<DL2011[2+(4*i)]) n=DL2011[1+(4*i)];
          else  n=DL2011[3+(4*i)]; }
   else if (year==12)
	  {    if (date<DL2012[2+(4*i)]) n=DL2012[1+(4*i)];
          else  n=DL2012[3+(4*i)]; }
   else if (year==13)
	  {    if (date<DL2013[2+(4*i)]) n=DL2013[1+(4*i)];
          else  n=DL2013[3+(4*i)]; }
   else if (year==14)
	  {    if (date<DL2014[2+(4*i)]) n=DL2014[1+(4*i)];
          else  n=DL2014[3+(4*i)]; }
   else if (year==15)
	  {    if (date<DL2015[2+(4*i)]) n=DL2015[1+(4*i)];
          else  n=DL2015[3+(4*i)]; }
   else if (year==16)
	  {    if (date<DL2016[2+(4*i)]) n=DL2016[1+(4*i)];
          else  n=DL2016[3+(4*i)]; }
   else if (year==17)
	  {    if (date<DL2017[2+(4*i)]) n=DL2017[1+(4*i)];
          else  n=DL2017[3+(4*i)]; }
   else if (year==18)
	  {    if (date<DL2018[2+(4*i)]) n=DL2018[1+(4*i)];
          else  n=DL2018[3+(4*i)]; }
   else if (year==19)
	  {    if (date<DL2019[2+(4*i)]) n=DL2019[1+(4*i)];
          else  n=DL2019[3+(4*i)]; }
   else if (year==20)
	  {    if (date<DL2020[2+(4*i)]) n=DL2020[1+(4*i)];
          else  n=DL2020[3+(4*i)]; }
	return n;
   }
unsigned char YEAR_AL(unsigned char MONTHAL, unsigned char month)
{
unsigned n;
if (MONTHAL<=month) n=year;
else if (MONTHAL>month) n=year-1;
return n;
}
void bip(void)
{
	unsigned char n;
    for(n=0;n<100;n++)
    {
		Loa=1; delay(50);
        Loa=0; delay(50);
    }    
}
//******************************************************************************
// CAC CHUONG TRINH CON CHO I2C
//******************************************************************************
void I2C_start(void)
{
	SCL = 1; SCL = 0;
	SDA = 1; SCL = 1;
	delay(2);
	SDA = 0; delay(2);
	SCL = 0; delay(2);
}
void I2C_stop(void)
{
	SCL = 1; SCL = 0;
	SDA = 0; delay(2);
	SCL = 1; delay(2);
	SDA = 1;
}
void I2C_write(unsigned char dat)
{
	unsigned char i;
	for (i=0;i<8;i++)
	{
		SDA = (dat & 0x80) ? 1:0;
		SCL=1;SCL=0;
		dat<<=1;
	}
	SCL = 1; delay(2);
	SCL = 0;
}
unsigned char I2C_read(void)
{
	bit rd_bit;
	unsigned char i, dat;
	dat = 0x00;
	for(i=0;i<8;i++) 		/* For loop read data 1 byte */
	{
		delay(2);
		SCL = 1; delay(2); 	/* Set SCL */
		rd_bit = SDA; 		/* Keep for check acknowledge */
		dat = dat<<1;
		dat = dat | rd_bit; /* Keep bit data in dat */
		SCL = 0; 			/* Clear SCL */
	}
	return dat;
}
//******************************************************************************
// CAC CHUONG TRINH CON CHO DS1307
//******************************************************************************
unsigned char DS1307_read(unsigned char addr)
{
	unsigned int temp,ret;
	I2C_start(); 		/* Start i2c bus */
	I2C_write(0xD0); 	/* Connect to DS1307 */
	I2C_write(addr); 	/* Request RAM address on DS1307 */
	I2C_start(); 		/* Start i2c bus */
	I2C_write(0XD1); 	/* Connect to DS1307 for Read */
	ret = I2C_read(); 	/* Receive data */
	I2C_stop();
	//**********************************************************
	temp = ret; 							/*BCD to HEX*/
	ret = (((ret/16)*10)+ (temp & 0x0f)); 	/*for Led 7seg*/
	//**********************************************************
	return ret;
}

void DS1307_Write(unsigned char addr,unsigned char dat)
{
	unsigned int temp;
	//********************************************** 
	temp = dat ; 						/*HEX to BCD*/
	dat = (((dat/10)*16)|(temp %10)); 	/*for Led 7seg*/
	//**********************************************
	I2C_start(); 		/* Start i2c bus */
	I2C_write(0XD0); 	/* Connect to DS1307 */
	I2C_write(addr); 	/* Request RAM address on DS1307 */
	I2C_write(dat); 	/* Connect to DS1307 for Read */
	I2C_stop();
}

void Out_1Hz()
{
	I2C_start(); /* Start i2c bus */
	I2C_write(0xD0); /* Connect to DS1307 */
	I2C_write(CONTROL);   //vi tri con tro RTC
	I2C_write(0x10);
	I2C_stop();
}

void Read_DS1307()
{
	sec   = DS1307_read(SEC);
	min   = DS1307_read(MIN);
	hour  = DS1307_read(HOUR);
	day   = DS1307_read(DAY);
	date  = DS1307_read(DATE);
	month = DS1307_read(MONTH);
	year  = DS1307_read(YEAR);
}

void Write_DS1307()
{
	DS1307_Write(SEC,sec);
	DS1307_Write(MIN,min);
	DS1307_Write(HOUR,hour);
	DS1307_Write(DAY,day);
	DS1307_Write(DATE,date);
	DS1307_Write(MONTH,month);
	DS1307_Write(YEAR,year); 
}

void Write_Alarm()
{
	DS1307_Write(A_MIN,a_min);
	DS1307_Write(A_HOUR,a_hour);
	DS1307_Write(ON_OFF,on_off);
}
void Write_Alarm1()
{
	DS1307_Write(A_MIN1,a_min1);
	DS1307_Write(A_HOUR1,a_hour1);
	DS1307_Write(ON_OFF1,on_off1);
}
void Read_Alarm()
{
    a_min   = DS1307_read(A_MIN);
	a_hour  = DS1307_read(A_HOUR);
	a_min1  = DS1307_read(A_MIN1);
	a_hour1 = DS1307_read(A_HOUR1);
	on_off  = DS1307_read(ON_OFF);
	on_off1 = DS1307_read(ON_OFF1);
}
//******************************************************************************
void delay(unsigned int time)
{ 
	while(time--); 
}
void set_main(void) // Chuong trinh khoi tao main
{ 
	P2=P1=0xFF;P1_7=0;
	P0=P3=0x00;
}
void setup_timer(void) // Setup timer 0 va timer 1
{
	TMOD=0x11; // timer0 & timer1 set che do 1
	ET1=1; ET0=1; EA=1;
	TF0=0; TF1=0; 
	TR0=1; TR1=1;
}
void Set_Clock(void) // Kiem tra va hieu chinh gia tri cua gio,phut,giay
{
	unsigned char y;
	y=(year%4);
//Tang
	if(min>59)  { min=0; }
	if(hour>23) { hour=0; }
	if(day>7) day=1;
	if((month==2)&&(date>29)&&(y==0)) { date=1; }
	else	if((month==2)&&(date>28)&&(y!=0)) { date=1; }
			else 	if(((month==4)||(month==6)||(month==9)||(month==11))&&(date>30)&&(y!=0)){ date=1; }
					else 	if(date>31) { date=1; }
	if(month>12)  { month=1; }
	if(year>99)   { year_20++; year=0; if(year_20>99){year_20=0;}  }
	if(a_min>59)  { a_min=0; }
	if(a_hour>23) { a_hour=0; }
	if(a_min1>59) { a_min1=0; }
	if(a_hour1>23){ a_hour1=0; }
//Giam
	if(min<0)  { min=59; }
	if(hour<0) { hour=23; }
	if(day<1) day=7;
	if((month==2)&&(date<1)&&(y==0)) { date=29; }
	else	if((month==2)&&(date<1)&&(y!=0)) { date=28; }
			else 	if(((month==4)||(month==6)||(month==9)||(month==11))&&(date<1)&&(y!=0)){ date=30; }
					else 	if(date<1) { date=31; }
	if(month<1)   { month=12; }
	if(year<0)    { year_20--; year=99; if(year_20<0){year_20=99;} }
	if(a_min<0)   { a_min=59; }
	if(a_hour<0)  { a_hour=23; }
	if(a_min1<0)  { a_min1=59; }
	if(a_hour1<0) { a_hour1=23; }
}
void Keypad(void) // Kiem tra phim nhan.
{
 	if(Ok==0)
	{	delay(1000);
	    while(Ok==0);
        delay(1000);
		if((Display==0)&&(set==0)&&(alarm==0)&&(alarm1==0))
		{
			AM_PM++;
			if(AM_PM==2) AM_PM=0;
		}
		else AM_PM=0;
		bip(); 
		Display=set=alarm=alarm1=0;
		if(on_off==1) Write_Alarm();	
		else Write_DS1307();	
		if(on_off1==1) Write_Alarm1();	
		else Write_DS1307();
	}

//*********************************************************************
	if(Display==0)
	{
		if(Set==0)
		{ 	delay(1000);
            while(Set==0);
            delay(1000);
			bip(); set++; 
			if(set==7) set=1;
		}
		if((set==1)&&(Display==0))
		{ 
			if(!Up) { min++; bip();Set_Clock(); delay(50000); }
			if(!Down) { min--; bip();Set_Clock(); delay(50000); }
		}
		if(set==2)
		{ 
			if(!Up) 
			{
				hour=hour+1; bip();Set_Clock(); delay(50000);
			}
			if(!Down) 
			{	
				hour=hour-1; bip();Set_Clock(); delay(50000);
			}
		}
		if(set==3)
		{ 
			if(!Up) 
			{
				day=day+1; bip();Set_Clock(); delay(50000);
			}
			if(!Down) 
			{	
				day=day-1; bip();Set_Clock(); delay(50000);
			}
		}
		if(set==4)
		{ 
			if(!Up) 
			{
				date=date+1; bip();Set_Clock(); delay(50000);
			}
			if(!Down) 
			{	
				date=date-1; bip();Set_Clock(); delay(50000);
			}
		}
		if(set==5)
		{ 
			if(!Up) 
			{
				month=month+1; bip();Set_Clock(); delay(50000);
			}
			if(!Down) 
			{	
				month=month-1; bip();Set_Clock(); delay(50000);
			}
		}
		if(set==6)
		{ 
			if(!Up) 
			{
				year=year+1; bip();Set_Clock(); delay(50000);
			}
			if(!Down) 
			{	
				year=year-1; bip();Set_Clock(); delay(50000);
			}
		}
	}
	if(Alarm==0)
	{  delay(1000);
       while(Alarm==0);
       delay(1000);
		bip(); Display++;
		if(Display==3) Display=1;
	}
	if(Display==1)
	{
		if(!Set)
		{ 
			bip(); alarm++; 
			if(alarm==4) alarm=1;
		}
		if(alarm==1)
		{ 
			if(!Up) //min++; 
			{
				a_min=a_min+1; bip();Set_Clock(); delay(50000);
			}
			if(!Down) //min--;
			{
				a_min=a_min-1; bip();Set_Clock(); delay(50000);
			}
		}
		if(alarm==2)
		{ 
			if(!Up) //hour++;
			{
				a_hour=a_hour+1; bip();Set_Clock(); delay(50000);
			}
			if(!Down) //hour--;
			{	
				a_hour=a_hour-1; bip();Set_Clock(); delay(50000);
			}
		}
		if(alarm==3)
		{ 
			if(!Up) //hour++;
			{
				on_off=1; bip();Set_Clock(); delay(50000); 
			}
			if(!Down) //hour--;
			{	
				on_off=0; bip();Set_Clock(); delay(50000);
			}
		}
	}
	if(Display==2)
	{
		if(!Set)
		{ 
			bip(); alarm1++; 
			if(alarm1==4) alarm1=1;
		}
		if(alarm1==1)
		{ 
			if(!Up) 
			{
				a_min1=a_min1+1; bip();Set_Clock(); delay(50000);
			}
			if(!Down) 
			{
				a_min1=a_min1-1; bip();Set_Clock(); delay(50000);
			}
		}
		if(alarm1==2)
		{ 
			if(!Up) 
			{
				a_hour1=a_hour1+1; bip();Set_Clock(); delay(50000);
			}
			if(!Down) 
			{	
				a_hour1=a_hour1-1; bip();Set_Clock(); delay(50000);
			}
		}
		if(alarm1==3)
		{ 
			if(!Up) 
			{
				on_off1=1; bip();Set_Clock(); delay(50000); 
			}
			if(!Down) 
			{	
				on_off1=0; bip();Set_Clock(); delay(50000);
			}
		}
}


//*********************************************************************
	Set_Clock(); 
	delay(1000);
}
void T0_ISR(void) interrupt 1 // Dung ngat Timer0 de doc DS1307
{
	TR0=0;
	TF0=0;
	TH0=-20000/256; // Gia tri cho Timer0: 50000(us)x10=0.5(s)
	TL0=-20000%256;
	time++;
	if(time==10)
	{
		time=0; bat_nhay++; read_time=1;
		if(bat_nhay==4) bat_nhay=0;	 // chop tat khi dieu chinh Clock
	}
	TR0=1;
}
/*************************************/
void T1_ISR(void) interrupt 3 // Dung ngat Timer1 de hien thi 15 Led_7D
{
	TR1=0;
	TF1=0;
	TH1=-1000/256; // Gia tri cho Timer1: 1000(us) x 15 = 15(ms)
	TL1=-1000%256;
	if(Display==0)
	{
		index++;
		if(index==1)
		{
			Data=sec;
			P0=0x00; E11=1; E12=1;
			P2=0x00;
			P0=led7_data[Data%10];//xuat gia tri hang don vi
			E11=0;E12=1;
		}
		if(index==2)
		{
			P0=0x00; E11=1; E12=1;
			P2=0x01;
			P0=led7_data[Data/10]; //xuat gia tri hang truc
			E11=0;E12=1;
		}
	//******************************************************************************
		if(index==3)
		{
			Data=min;
			P0=0x00; E11=1; E12=1;
			if(set==1 && bat_nhay==1){ index++; index++; }
			else
			{
				P2=0x02;
				P0=led7_data[Data%10];
				E11=0;E12=1;
			}
		}
		if(index==4)
		{
			P0=0x00; E11=1; E12=1;
			P2=0x03;
			P0=led7_data[Data/10];
			E11=0;E12=1; 
		}
	//******************************************************************************
		if(index==5)
		{
			if(AM_PM==1) 
			{
				if(hour>12) Data=hour-12;
				else 
				{
					if(hour==0) Data=12;
					else Data=hour;
				}
			}
			else Data=hour;
			P0=0x00; E11=1; E12=1;
			if(set==2 && bat_nhay==1){ index++; index++; }
			else
			{ 
				P2=0x04;
				P0=led7_data[Data%10];
				E11=0;E12=1;
			}
		}
		if(index==6)
		{ 	
			P0=0x00; E11=1; E12=1;
			P2=0x05;
			P0=led7_data[Data/10]; 
			E11=0;E12=1;
		} 
	//******************************************************************************
		if(index==7) 
		{
			if ((day<8)&&(day>1)) Data=day;
			else if (day==1) Data=10;
			P0=0x00; E11=1; E12=1;
			if(set==3 && bat_nhay==1){ index++; }
			else
			{ 	
					P2=0x06;
					P0=led7_data[Data];
					E11=0;E12=1;
			}
		}
	//******************************************************************************
		if(index==8)
		{
			Data=date;
			P0=0x00; E11=1; E12=1;
			if(set==4 && bat_nhay==1){ index++; index++; }
			else
			{
			    P2=0x07;
				P0=led7_data[Data/10];
				E11=0; E12=1;
			}
		}
		if(index==9)
		{
			P0=0x00; E11=1; E12=1;
			P2=0x08;
			P0=led7_data[Data%10];
			E11=0; E12=1;
		}
	//******************************************************************************
		if(index==10)
		{
			Data=month;
			P0=0x00; E11=1; E12=1;
			if(set==5 && bat_nhay==1){ index++; index++; }
			else
			{
				P2=0x09;
				P0=led7_data[Data/10];
				E11=0; E12=1;
			}
		}
		if(index==11)
		{
			P0=0x00; E11=1; E12=1;
			P2=0x0a;
			P0=led7_data[Data%10];
			E11=0; E12=1;
		}
	//******************************************************************************
		if(index==12)
		{
			Data=year_20;
			P0=0x00; E11=1; E12=1;
			if(set==6 && bat_nhay==3){ index++; index++; index++; index++; }
			else
			{
				P2=0x0b;
				P0=led7_data[Data/10];
				E11=0;E12=1;
			}
		}
		if(index==13)
		{
			P0=0x00; E11=1; E12=1;
			P2=0x0c;
			P0=led7_data[Data%10];
			E11=0; E12=1;
		}
		if(index==14)
		{
			Data=year;
			P0=0x00; E11=1; E12=1;
			P2=0x0d;
			P0=led7_data[Data/10];
			E11=0; E12=1;
		}
		if(index==15)
		{
			P0=0x00; E11=1; E12=1;
			P2=0x0e;
			P0=led7_data[Data%10];
			E11=0; E12=1;
		}
		if(index==16)
		{
			Data=DATEAL;
			P0=0x00; E11=1; E12=1;
			P2=0x00;
			P0=led7_data[Data/10];//xuat gia tri hang don vi
			E11=1;E12=0;
		}
		if(index==17)
		{
			P0=0x00; E11=1; E12=1;
			P2=0x10;
			P0=led7_data[Data%10]; //xuat gia tri hang truc
			E11=1;E12=0;
		}
	//******************************************************************************
		if(index==18)
		{
			Data=MONTHAL;
			P0=0x00; E11=1; E12=1;
			P2=0x20;
			P0=led7_data[Data/10];
			E11=1;E12=0;
			
		}
		if(index==19)
		{
			P0=0x00; E11=1; E12=1;
			P2=0x30;
			P0=led7_data[Data%10];
			E11=1;E12=0; 
		}
	//******************************************************************************
		if(index==20)
		{
		    Data=yearal_20;
			P0=0x00; E11=1; E12=1;
			P2=0x40;
			P0=led7_data[Data/10];
			E11=1;E12=0;
		}
		if(index==21)
		{ 	
			P0=0x00; E11=1; E12=1;
			P2=0x50;
			P0=led7_data[Data%10]; 
			E11=1;E12=0;
		} 
	//******************************************************************************
		if(index==22) 
		{
			Data=YEARAL;
			P0=0x00; E11=1; E12=1;
			P2=0x60;
			P0=led7_data[Data/10];
			E11=1;E12=0;
		}
	//******************************************************************************
		if(index==23)
		{
			P0=0x00; E11=1; E12=1;
			P2=0x70;
			P0=led7_data[Data%10];
			E11=1;E12=0;
		}
		if(index==24) index=0;
	}
	
	if(Display==1)
	{
		index++;
		if(index==1)
		{
			Data=sec;
			P0=0x00; E11=1; E12=1;
			P2=0x00;
			P0=led7_data[Data%10];//xuat gia tri hang don vi
			E11=0; E12=1;
		}
		if(index==2)
		{
			P0=0x00; E11=1; E12=1;
			P2=0x01;
			P0=led7_data[Data/10]; //xuat gia tri hang truc
			E11=0; E12=1;
		}
	//******************************************************************************
		if(index==3)
		{
			Data=min;
			P0=0x00; E11=1; E12=1;
			if(set==1 && bat_nhay==1){ index++; index++; }
			else
			{
				P2=0x02;
				P0=led7_data[Data%10];
				E11=0; E12=1;
			}
		}
		if(index==4)
		{
			P0=0x00; E11=1; E12=1;
			P2=0x03;
			P0=led7_data[Data/10]; 
			E11=0; E12=1;
		}
	//******************************************************************************
		if(index==5)
		{
			if(AM_PM==1) 
			{
				if(hour>12) Data=hour-12;
				else 
				{
					if(hour==0) Data=12;
					else Data=hour;
				}
			}
			else Data=hour;
			P0=0x00; E11=1; E12=1;
			if(set==2 && bat_nhay==1){ index++; index++; }
			else
			{ 
				P2=0x04;
				P0=led7_data[Data%10];
				E11=0; E12=1;
			}
		}
		if(index==6)
		{ 	
			P0=0x00; E11=1; E12=1;
			P2=0x05;
			P0=led7_data[Data/10];
			E11=0; E12=1; 
		} 
		if(index==7)
		{
			P0=0x00; E11=1; E12=1;
			P2=0x06;
			P0=led7_data[1];
			E11=0; E12=1;
		}
		if(index==8)
		{
			Data=a_min;
			if(alarm==1 && bat_nhay==1){ index++; index++; }
			else
			{
				P0=0x00; E11=1; E12=1;
				P2=0x0a;
				P0=led7_data[Data%10];
				E11=0; E12=1;
			}
		}
		if(index==9)
		{
			P0=0x00; E11=1; E12=1;
			P2=0x09;
			P0=led7_data[Data/10];
			E11=0;E12=1;
		}
		if(index==10)
		{
			if(AM_PM==1) 
			{
				if(a_hour>12) Data=a_hour-12;
				else 
				{
					if(a_hour==0) Data=12;
					else Data=a_hour;
				}
			}
			else Data=a_hour;
			if(alarm==2 && bat_nhay==1){ index++; index++; }
			else
			{
				P0=0x00; E11=1; E12=1;
				P2=0x08;;
				P0=led7_data[Data%10];
				E11=0;E12=1;
			}
		}
		if(index==11)
		{
			P0=0x00; E11=1; E12=1;
			P2=0x07;
			P0=led7_data[Data/10];
			E11=0;E12=1;
		}
		if(index==12)
		{
			P0=0x00; E11=1; E12=1;
			if(alarm==3 && bat_nhay==2){ index++; index++; index++; index++; }
			else
			{
				P2=0x0c;
				P0=led7_data[0];
				E11=0;E12=1;
			}
		}
		if(index==13)
		{
			P0=0x00; E11=1; E12=1;
			if(on_off==1)
			{
				P2=0x0d;
				P0=0x37;
				E11=0;E12=1;	
			}
			else 
			{
				P2=0x0d;
				P0=0x71;
				E11=0;E12=1;
			}
		}
		if(index==14)
		{
			P0=0x00; E11=1; E12=1;
			if(on_off==1)
			{
				P2=0x0e;
				P0=0x00;
				E11=0;E12=1;	
			}
			else 
			{
				P2=0x0e;
				P0=0x71;
				E11=0;E12=1;
			}
		}
		if(index==15)
		{
			P0=0x00; E11=1; E12=1;
			if(a_hour>12)
			{
				P2=0x0b;
				P0=0x40;
				E11=0;E12=1;	
			}
			else 
			{
				P2=0x0b;
				P0=0x00;
				E11=0;E12=1;
			}
		}
		if(index==16)
		{
			Data=DATEAL;
			P0=0x00; E11=1; E12=1;
			P2=0x00;
			P0=led7_data[Data/10];//xuat gia tri hang don vi
			E11=1;E12=0;
		}
		if(index==17)
		{
			P0=0x00; E11=1; E12=1;
			P2=0x10;
			P0=led7_data[Data%10]; //xuat gia tri hang truc
			E11=1;E12=0;
		}
	//******************************************************************************
		if(index==18)
		{
			Data=MONTHAL;
			P0=0x00; E11=1; E12=1;
			P2=0x20;
			P0=led7_data[Data/10];
			E11=1;E12=0;
			
		}
		if(index==19)
		{
			P0=0x00; E11=1; E12=1;
			P2=0x30;
			P0=led7_data[Data%10]; 
			E11=1;E12=0;
		}
	//******************************************************************************
		if(index==20)
		{
		    Data=yearal_20;
			P0=0x00; E11=1; E12=1;
			P2=0x40;
			P0=led7_data[Data/10];
			E11=1;E12=0;
		}
		if(index==21)
		{ 	
			P0=0x00; E11=1; E12=1;
			P2=0x50;
			P0=led7_data[Data%10]; 
			E11=1;E12=0;
		} 
	//******************************************************************************
		if(index==22) 
		{
			Data=YEARAL;
			P0=0x00; E11=1; E12=1;
			P2=0x60;
			P0=led7_data[Data/10];
			E11=1;E12=0;
		}
	//******************************************************************************
		if(index==23)
		{
			P0=0x00; E11=1; E12=1;
			P2=0x70;
			P0=led7_data[Data%10];
			E11=1;E12=0;
		}
		if(index==24) index=0;
	}
	if(Display==2)
	{
		index++;
		if(index==1)
		{
			Data=sec;
			P0=0x00; E11=1; E12=1;
			P2=0x00;;
			P0=led7_data[Data%10];//xuat gia tri hang don vi
			E11=0;E12=1;
		}
		if(index==2)
		{
			P0=0x00; E11=1; E12=1;
			P2=0x01;
			P0=led7_data[Data/10]; //xuat gia tri hang truc
			E11=0;E12=1;
		}
	//******************************************************************************
		if(index==3)
		{
			Data=min;
			P0=0x00; E11=1; E12=1;
			if(set==1 && bat_nhay==1){ index++; index++; }
			else
			{
				P2=0x02;
				P0=led7_data[Data%10];
				E11=0;E12=1;
			}
		}
		if(index==4)
		{
			P0=0x00; E11=1; E12=1;
			P2=0x03;
			P0=led7_data[Data/10];
			E11=0;E12=1; 
		}
	//******************************************************************************
		if(index==5)
		{
			if(AM_PM==1) 
			{
				if(hour>12) Data=hour-12;
				else 
				{
					if(hour==0) Data=12;
					else Data=hour;
				}
			}
			else Data=hour;
			P0=0x00; E11=1; E12=1;
			if(set==2 && bat_nhay==1){ index++; index++; }
			else
			{ 
				P2=0x04;
				P0=led7_data[Data%10];
				E11=0; E12=1;
			}
		}
		if(index==6)
		{ 	
			P0=0x00; E11=1; E12=1;
			P2=0x05;
			P0=led7_data[Data/10]; 
			E11=0; E12=1;
		} 
		if(index==7)
		{
			P0=0x00; E11=1; E12=1;
			P2=0x06;
			P0=led7_data[2];
			E11=0; E12=1;
		}
		if(index==8)
		{
			Data=a_min1;
			if(alarm1==1 && bat_nhay==1){ index++; index++; }
			else
			{
				P0=0x00; E11=1; E12=1;
			    P2=0x0a;
				P0=led7_data[Data%10];
				E11=0; E12=1;
			}
		}
		if(index==9)
		{
			P0=0x00; E11=1; E12=1;
			P2=0x09;
			P0=led7_data[Data/10];
			E11=0; E12=1;
		}
		if(index==10)
		{
			if(AM_PM==1) 
			{
				if(a_hour1>12) Data=a_hour1-12;
				else 
				{
					if(a_hour1==0) Data=12;
					else Data=a_hour1;
				}
			}
			else Data=a_hour1;
			if(alarm1==2 && bat_nhay==1){ index++; index++; }
			else
			{
				P0=0x00; E11=1; E12=1;
				P2=0x08;
				P0=led7_data[Data%10];
				E11=0; E12=1;
			}
		}
		if(index==11)
		{
			P0=0x00; E11=1; E12=1;
			P2=0x07;
			P0=led7_data[Data/10];
			E11=0; E12=1;
		}
		if(index==12)
		{
			P0=0x00; E11=1; E12=1;
			if(alarm1==3 && bat_nhay==2){ index++; index++; index++; index++; }
			else
			{
				P2=0x0c;
				P0=led7_data[0];
				E11=0; E12=1;
			}
		}
		if(index==13)
		{
			P0=0x00; E11=1; E12=1;
			if(on_off1==1)
			{
				P2=0x0d;
				P0=0x37;
				E11=0; E12=1;	
			}
			else 
			{
				P2=0x0d;
				P0=0x71;
				E11=0; E12=1;
			}
		}
		if(index==14)
		{
			P0=0x00; E11=1; E12=1;
			if(on_off1==1)
			{
				P2=0x0e;
				P0=0x00;
				E11=0; E12=1;	
			}
			else 
			{
				P2=0x0e;
				P0=0x71;
				E11=0; E12=1;
			}
		}
		if(index==15)
		{
			P0=0x00; E11=1; E12=1;
			if(a_hour1>12)
			{
				P2=0x0b;
				P0=0x40;
				E11=0; E12=1;	
			}
			else 
			{
				P2=0x0b;
				P0=0x00;
				E11=0; E12=1;
			}
		}
		if(index==16)
		{
			Data=DATEAL;
			P0=0x00; E11=1; E12=1;
			P2=0x00;
			P0=led7_data[Data/10];//xuat gia tri hang don vi
			E11=1;E12=0;
		}
		if(index==17)
		{
			P0=0x00; E11=1; E12=1;
			P2=0x10;
			P0=led7_data[Data%10]; //xuat gia tri hang truc
			E11=1;E12=0;
		}
	//******************************************************************************
		if(index==18)
		{
			Data=MONTHAL;
			P0=0x00; E11=1; E12=1;
			P2=0x20;
			P0=led7_data[Data/10];
			E11=1;E12=0;
			
		}
		if(index==19)
		{
			P0=0x00; E11=1; E12=1;
			P2=0x30;
			P0=led7_data[Data%10]; 
			E11=1;E12=0;
		}
	//******************************************************************************
		if(index==20)
		{
		    Data=yearal_20;
			P0=0x00; E11=1; E12=1;
			P2=0x40;
			P0=led7_data[Data/10];
			E11=1;E12=0;
		}
		if(index==21)
		{ 	
			P0=0x00; E11=1; E12=1;
			P2=0x50;
			P0=led7_data[Data%10]; 
			E11=1;E12=0;
		} 
	//******************************************************************************
		if(index==22) 
		{
			Data=YEARAL;
			P0=0x00; E11=1; E12=1;
			P2=0x60;
			P0=led7_data[Data/10];
			E11=1;E12=0;
		}
	//******************************************************************************
		if(index==23)
		{
			P0=0x00; E11=1; E12=1;
			P2=0x70;
			P0=led7_data[Data%10];
			E11=1;E12=0;
		}
		if(index==24) index=0;
	}
	
//******************************************************************************
	TR1=1;
}
//************ CHUONG TRINH CHINH *************
void main()
{  	Out_1Hz();
    time=index=set=read_time=0;
	bip(); 
	delay(100);
	set_main();
	year_20=20;
	yearal_20=20;
	setup_timer();
	Read_Alarm();
	Read_DS1307();
	DS1307_Write(SEC,0x00);
	while(1)
	{   
	    
		if ((year>9)&&(year<26)) 
		{
		year_20=yearal_20=20;
		DATEAL=DATE_AL(date,month,year);
        MONTHAL=MONTH_AL(date,month,year);
	    YEARAL=YEAR_AL(MONTHAL,month); }
		else
		{DATEAL=MONTHAL=YEARAL=yearal_20=0; }
		Keypad();
		if(hour>=12)  {AM=1; PM=0; delay(100);}
		else {AM=0; PM=1; delay(100); }
		if((set==0)&&(read_time==1))
      {
        if ((hour==12)&&(min==0)&&(sec==0))
	         {read_time = 0;
              Read_DS1307();
			  DS1307_Write(SEC,0x0a);}
		    else 
			{ read_time = 0;
              Read_DS1307();}
      }
		else sec = DS1307_read(SEC); 
		if(((a_min==min)&&(a_hour==hour)&&(on_off==1)))
		{
			if(sec<59) { bip(); delay(50000);}
			else on_off=0;
		}
		if((a_min1==min)&&(a_hour1==hour)&&((day>1)&&(day<7))&&(on_off1==1))
		{
			if(sec<59) { bip(); delay(50000);}
		}
	}
} 
