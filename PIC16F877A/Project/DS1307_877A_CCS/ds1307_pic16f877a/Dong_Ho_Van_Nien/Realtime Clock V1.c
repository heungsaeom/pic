/******************************************************************************                                                   
Chip type           : PIC16F877A         
Program type        : Application
Clock frequency     : 20 MHz
Project             : Realtime Clock
Version             : V1.0
Date                : 28/06/2013
Author              : DAO XUAN TRUONG 
Gmail               : daotruongpro@gmail.com   
Mobile              : 0979.20.90.10
Company             :  

Comments : Chuong trinh dong ho thoi gian thuc
           Su dung RTC Ds1307
           Hien thi Led 7 doan
           Hien thi thoi gian, Gio, Phut
           Hien thi ngay, thang, nam duong lich
           Hien thi ngay, thang, nam am lich
           Hien thi nhiet do su dung Ds1307
*******************************************************************************/

#include <Realtime Clock V1.h>
#include "ds1307.c"
//#include <math.h>
//#include <stdio.h>
#include "PIC16F877A_registers.h"

/************************** Khai bao bien toan cuc*****************************/
                                                                              //
//int1  StEn=false;                                                            //
unsigned char StSet=0;   //bien tao nhap nhay vung cai dat                    //
unsigned char StNumber=0;//bien cho phep cai dat 0-11                         //
unsigned char Number=0;  //bien phuc vu quet led 0-22                         //
unsigned char Temp;     //Nhuet do                                            //
unsigned char sec,min,hrs,day_d,month_d,year_d,date; //thoi gian duong lich   //
unsigned char day_a,month_a,year_a; //thoi gian am lich                   //
unsigned char dsec=0; //bien phuc vu update time                              //
//unsigned char CharCode[10]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
/*                  Bang ma LED Anot chung                                   */
unsigned char CharCode[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
/*                  Bang ma LED CAtot chung                                    */
unsigned char ADC_Convert(); //chuong trinh chuyen doi ADC 10bit chanel 0     //
void VietNam_Calendar(unsigned char day_d,month_d,year_d);/*Chuyen doi am lich//
                                                          VietNam den nam 2030*/
void OutLed(unsigned char DATA);// Chuong trinh Xuat data ra PORTD            //
void Sys_init(); //Chuong trinh khoi tao chip                                 //
void Program_init(); // chuong trinh khoi tao                                 //
void Setup();//Chuong trinh cai dat thoi gian                                 // 
void Led7segment(unsigned char day_d,month_d,year_d,day_a,month_a,year_a,date,//
Temp,hrs,min,sec ); //chuong trinh quet LED dung ngat hoan toan tan so fV=25Hz//
                                                                              //
/******************************************************************************/

/*******************************************************************************
/////////////////Chuong trinh dinh thoi Timer0 1.8ms////////////////////////////
*******************************************************************************/
#int_RTCC
void  RTCC_isr(void) 
{
   Set_timer0(115); //tao tan so quet fV=25Hz
   Number++; 
   //if(Number>22){Number=1;en=1;clk=0;clk=1;en=0;}//xoa khi Scan du 22led
   if(Number>22){Number=1;en=0;clk=0;clk=1;en=1;}//xoa khi Scan du 22led
   clk=0;clk=1;  //tao xung clock
   OutLed(0xFF); //xoa bong ma
   Led7segment(day_d,month_d,year_d,day_a,month_a,year_a,date,Temp,hrs,min,sec);
      }
/*******************************************************************************
/////////////////Chuong trinh dinh thoi Timer1 2ms//////////////////////////////
*******************************************************************************/
#int_TIMER1                 
void  TIMER1_isr(void)                           
{                            
 Set_timer1(0xBDB);
 dsec++;
 if(dsec>=10) { //sau 1s cap ngat time gom gio, phut,giay
              dsec=0;
              ds1307_get_time(hrs,min,sec);
              if((hrs==00)&&(min==00)&&(sec<=10))//10s ngay moi update time.....
               {
                  ds1307_get_date(day_d,month_d,year_d,date);
                  VietNam_Calendar(day_d,month_d,year_d);
               }
              if(sec%10==0) Temp=ADC_convert(); //sau 10s update nhiet do
              }               
} 

/*******************************************************************************
/////////////////Chuong trinh quet LED7 dung timer khong dung delay/////////////
*******************************************************************************/
void Led7segment( unsigned char day_d,month_d,year_d,day_a,month_a,year_a,date,
Temp,hrs,min,sec )
{  
   StSet++;if(StSet>=200) StSet=0;
   Switch(Number)
   {
   case 1:{
            if(StNumber==1){if(StSet<=70) {OutLed(0);break;}}
            if(day_d<10) OutLed(0); //tat so 0
            else OutLed(CharCode[day_d/10]);
            break;
         };
   case 2:{
            if(StNumber==1){if(StSet<=70) {OutLed(0);break;}}
            OutLed(CharCode[day_d%10]);
            break;
           };
   case 3:{
            if(StNumber==2){if(StSet<=70) {OutLed(0);break;}}
            if(month_d<10) OutLed(0);//tat so 0
            else OutLed(CharCode[month_d/10]);
            break;
         };
   case 4:{
            if(StNumber==2){if(StSet<=70) {OutLed(0);break;}}
            OutLed(CharCode[month_d%10]);
            break;
           };
   case 5:{
            if(StNumber==3){if(StSet<=70) {OutLed(0);break;}}
            OutLed(CharCode[year_d/10]);
            break;
           };
   case 6:{
            if(StNumber==3){if(StSet<=70) {OutLed(0);break;}}
            OutLed(CharCode[year_d%10]);
            break;
         };
   case 7:{
            //if(StNumber==4){if(StSet<=200) {OutLed(0xff);break;}}
            if(day_a<10) OutLed(0);//tat so 0
            else OutLed(CharCode[day_a/10]);
            break;
           }; 
   case 8:{
            //if(StNumber==4){if(StSet<=200) {OutLed(0xff);break;}}
            OutLed(CharCode[day_a%10]);
            break;
           };
   case 9:{
            //if(StNumber==5){if(StSet<=200) {OutLed(0xff);break;}}
            if(month_a<10) OutLed(0);//tat so 0
            else OutLed(CharCode[month_a/10]);
            break;
         };
   case 10:{
            //if(StNumber==5){if(StSet<=200) {OutLed(0xff);break;}}
            OutLed(CharCode[month_a%10]);
            break;
           };
   case 11:{
            //if(StNumber==6){if(StSet<=200) {OutLed(0xff);break;}}
            OutLed(CharCode[year_a/10]);
            break;
           };
   case 12:{
            //if(StNumber==6){if(StSet<=200) {OutLed(0xff);break;}}
            OutLed(CharCode[year_a%10]);
            break;
         };
   case 13:{
            if(StNumber==4){if(StSet<=70) {OutLed(0);break;}}
            if(date==1) OutLed(0X39);//ngay CN hien thi chu C
            else OutLed(0x78);       //hien thi chu t
            break;
           }; 
   case 14:{
            if(StNumber==4){if(StSet<=70) {OutLed(0);break;}}
            if(date==1) OutLed(0x37);//ngay CN hien thi chu n
            else OutLed(CharCode[date%10]);
            break;
           };        
   case 15:{
            //if(StNumber==8){if(StSet<=200) {OutLed(0xff);break;}}
            if(Temp<10) OutLed(0);//tat so 0
            else OutLed(CharCode[Temp/10]);
            break;
         };
   case 16:{
            //if(StNumber==8){if(StSet<=200) {OutLed(0xff);break;}}
            OutLed(CharCode[Temp%10]);
            break;
           };
   case 17:{
            if(StNumber==5){if(StSet<=70) {OutLed(0);break;}}
            OutLed(CharCode[hrs/10]);
            break;
           };
   case 18:{
            if(StNumber==5){if(StSet<=70) {OutLed(0);break;}}
            OutLed(CharCode[hrs%10]);
            break;
         };
   case 19:{
            if(StNumber==6){if(StSet<=70) {OutLed(0);break;}}
            OutLed(CharCode[min/10]);
            break;
           }; 
   case 20:{
            if(StNumber==6){if(StSet<=70) {OutLed(0);break;}}
            OutLed(CharCode[min%10]);
            break;
           };
   case 21:{
            if(StNumber==7){if(StSet<=70) {OutLed(0);break;}} 
            OutLed(CharCode[sec/10]);
            break;
         };
   case 22:{
            if(StNumber==7){if(StSet<=70) {OutLed(0);break;}}
            OutLed(CharCode[sec%10]);
           };            
   }
}

/*******************************************************************************
/////////////////Chuong trinh chuyen doi ADC doc nhiet do tu LM35///////////////
*******************************************************************************/
unsigned char ADC_Convert()
   {       int16 Value;
           Value=read_adc();            
           Value=(FLOAT)((Value*500)/1023);
   return((char)Value); //tra ve gia tri do C
   }
/*******************************************************************************
///////////////////////Chuong trinh am lich Viet Nam////////////////////////////
*******************************************************************************/
void VietNam_Calendar(unsigned char day_1,mon_1,year_1)
{
   unsigned char luu,tam_1;
   unsigned char da,db;
   unsigned char lmon;
   if(year_1<=30) //cho phep den nam 2030
     {
         da = ALdauthangDL[year_1-10][mon_1-1];
         db = DLdauthangAL[year_1-10][mon_1-1];   
         luu= db-day_1;
         if(luu<=0)
            {
               day_a    = (day_1-db+1);
               month_a  = thangALdauthangAL[year_1-10][mon_1-1];
            }
         
         else 
            {
               if(luu>31){day_a = (day_1-db+1);
               month_a = thangALdauthangDL[year_1-10][mon_1];}
               else
                  {
                     day_a = (day_1+da-1);
                     month_a = thangALdauthangDL[year_1-10][mon_1-1];
                  }
            }
         lmon = month_a;
         tam_1=mon_1-lmon;         
         if(tam_1>100&&tam_1<255)
         year_a = (year_1-1);
         else year_a = year_1;
     }
}    
//!void VietNam_Calendar(unsigned char day,month,year)
//!{
//!   char da,db;
//!   if(year<=30) //cho phep den nam 2030
//!   {
//!      da = ALdauthangDL[year-10][month-1];
//!      db = DLdauthangAL[year-10][month-1];
//!      if(db <= day){
//!      day_a = day-db+1;
//!      month_a = thangALdauthangAL[year-10][month-1];
//!      } else {
//!      day_a = (day_d+da-1);
//!      month_a =thangALdauthangDL[year-10][month-1];
//!      }
//!      if(month < day_a)
//!      year_a = year-1;
//!      else
//!      year_a = year;
//!   }
//!}
/*******************************************************************************
///////////////////////Chuong trinh Xuat data ra PORTD//////////////////////////
*******************************************************************************/
void OutLed(unsigned char data)   
{                 
   PORTD = data;
}

char key(char data, char never)
{  
   //Cho phep tang ngay,thang,nam,thu
   if((!Up)&&(StNumber<=4)){
            delay_ms(300);
            data++;            
            if(data>never) data=1;
          }
   //Cho phep tang gio,phut,giay         
   if((!Up)&&(StNumber>4)){
            delay_ms(300);
            data++;            
            if(data>never) data=0;
            }
   //Cho phep giam ngay,thang,nam,thu        
   if((!Down)&&(StNumber<=4)){
             delay_ms(300); 
             if(data>1) data--;
            } 
   //Cho phep giam ngay,thang,nam,thu            
   if((!Down)&&(StNumber>4)){
             delay_ms(300); 
            (signed)data--;
             if(data<0) data=never;
            }
    //thoat ra menu cai dat        
    if(!ESC){
             while(!ESC){;};
             StNumber=0;
             break;
               }
   VietNam_Calendar(day_d,month_d,year_d);
   return(data);
}
void Set_time()
{
   switch(StNumber)
   {
   //restart_WDT(); //WDT
   case 1:{          
            day_d=key(day_d,31);
            if((!Enter))
               {
                  while(!Enter){;};
                  ds1307_set_date(day_d,month_d,year_d,date);
                  delay_ms(500);
                  StNumber++;
               }
            break;
          };
   case 2:{ 
           month_d=key(month_d,12);
           if((!Enter))
               {
                  while(!Enter){;};
                  ds1307_set_date(day_d,month_d,year_d,date);
                  delay_ms(500);
                  StNumber++;
               }
           break;
          };
   case 3:{ 
           year_d=key(year_d,30);
           if((!Enter))
               {
                  while(!Enter){;};
                  ds1307_set_date(day_d,month_d,year_d,date);
                  delay_ms(500);
                  StNumber++;
               }
           break;
          };
   case 4:{ 
            date=key(date,7);
            if((!Enter))
               {
                  while(!Enter){;};
                  ds1307_set_date(day_d,month_d,year_d,date);
                  delay_ms(500);
                  StNumber++;
               }
            break;
          };          
    case 5:{ disable_interrupts(INT_TIMER1);
            hrs=key(hrs,24);
            if((!Enter))
               {
                  while(!Enter){;};
                  ds1307_set_time(hrs,min,sec);
                  delay_ms(500);
                  StNumber++;
               }
            break;
          };
   case 6:{ disable_interrupts(INT_TIMER1);
            min=key(min,59);
            if((!Enter))
               {
                  while(!Enter){;};
                  ds1307_set_time(hrs,min,sec);
                  delay_ms(500);
                  StNumber++;
               }
            break;
          }; 
   case 7:{ disable_interrupts(INT_TIMER1);
            sec=key(sec,59);
            if((!Enter))
               {
                  while(!Enter){;};
                  ds1307_set_time(hrs,min,sec);
                  delay_ms(500);
                  StNumber=255;
               }
            break;
          };
   case 255:{
            enable_interrupts(INT_TIMER1);
            dsec=10; //update time
            StNumber=0;
            break;
            }           
   }
}

void Setup()
{
StNumber=1;
while(StNumber!=0) 
{
      //restart_WDT(); //WDT
      if(!Enter){
               while(!Enter){;}
               if(StNumber<10)StNumber++;
             }
      if(!MenuDown){
               while(!MenuDown){;}; 
               if(StNumber>1)StNumber--;
               }   
      if(!ESC){
             while(!ESC){;};
             StNumber=0;
             break;
               }
      Set_time();
} 
   ds1307_get_date(day_d,month_d,year_d,date);
   VietNam_Calendar(day_d,month_d,year_d);
}

/*******************************************************************************
/////////////////////////////Chuong trinh chinh ////////////////////////////////
*******************************************************************************/
#zero_ram
void main()
{ 
  Sys_init();
  Program_init();
  while(TRUE)
     {
      restart_WDT(); //WDT
      if(!Enter) {while(!Enter){;}; Setup();}
      //if(StEn) {Setup();};
     }
} 
void Sys_init()              
{
   TRISA=0X01;
   TRISB=0X3F;
   TRISD=0X00;
   TRISE=0X00;
   setup_adc_ports(AN0); //AN0 =ADC 10bit
   setup_adc(ADC_CLOCK_DIV_32);//
   setup_timer_0(RTCC_INTERNAL|RTCC_DIV_64|RTCC_8_bit);//3.2 ms overflow 
   setup_timer_1(T1_INTERNAL|T1_DIV_BY_8);      //104 ms overflow flow 
   //setup_wdt(WDT_ON);
   //setup_wdt(WDT_1152MS);   
   enable_interrupts(GLOBAL);
}
void Program_init()
{
  ds1307_init(0,0,0);  // initialize 1307 
  //ds1307_set_date_time(31,6,13,2,23,59,55);
  //ds1307_set_date(1,1,13,3);
  delay_ms(100);
  ds1307_get_date(day_d,month_d,year_d,date);
  ds1307_get_time(hrs,min,sec);
  VietNam_Calendar(day_d,month_d,year_d);
  Temp=ADC_convert();
  delay_ms(200);
  enable_interrupts(INT_RTCC);
  enable_interrupts(INT_TIMER1);
  enable_interrupts(GLOBAL);  
}

