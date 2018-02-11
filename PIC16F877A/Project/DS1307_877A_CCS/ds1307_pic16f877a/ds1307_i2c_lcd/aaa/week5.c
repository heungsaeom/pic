#include <16f877a.h>
#device *=16 adc=10
#FUSES NOWDT, HS, NOPUT, NOPROTECT, NODEBUG, NOBROWNOUT,NOLVP, NOCPD, NOWRT
#use delay(clock=4000000)
#include "LCD4bit.h"
#use i2c(Master,Slow,sda=PIN_B1,scl=PIN_B0)     
int set=0;
int mod=0;
byte giay,phut,gio, ngay,thang, nam;
byte BCD2DEC(byte bcd)// ham chuyen tu BCD sang he thap phan
{
  byte h;
  byte l;
  byte dec;
  h = bcd / 16;
  l = bcd % 16;
  dec = (h * 10) + l;
  return dec;
}
byte DEC2BCD(byte dec)//ham chuyen tu he thap phan sang BCD
{
   byte h;
   byte l;
   byte bcd;
   h = dec / 10;
   l = dec % 10;
   bcd = (h << 4) + l;
   return bcd;
}
void hienthi(int8 dec)// ham hien thi 1 so tren LCD
{
   LCD_Char(dec/10 + 48);
   LCD_Char(dec%10 + 48);
}
void ghidl(byte address, BYTE data)// Ham ghi du lieu vao DS1307
{
   i2c_start();
   i2c_write(0xd0);// gui dia chi
   i2c_write(address);// con tro chi den dia chi
   i2c_write(data);// ghi du lieu vao di chi
   i2c_stop();
}
BYTE docdl(byte address)
{
   BYTE data;
   i2c_start();
   i2c_write(0xd0);//gui dia chi
   i2c_write(address);// con tro dua den dia chi
   i2c_stop();
   i2c_start();
   i2c_write(0xd1);// gui lenh doc du lieu
   data=i2c_read(0);
   i2c_stop();
   return(data);
}
void update_Ds1307()
{
         ghidl(0, 0); // update giay
         delay_ms(5);
         ghidl(1, DEC2BCD(phut));// update phut
         delay_ms(5);
         ghidl(2, DEC2BCD(gio));// update gio
         delay_ms(5);
         ghidl(4, DEC2BCD(ngay));// update ngay
         delay_ms(5);
         ghidl(5, DEC2BCD(thang)); // update thang
         delay_ms(5);
         ghidl(6, DEC2BCD(nam));// update nam
         delay_ms(5);
} 
#int_RB
void chinh_gio()
{
if (input(PIN_B4)==0)// set chinh thoi gian
   {
      if (set==0)// bat dau chinh thoi gian
      {
      set=1;
      mod=1;
      }
      else // update DS1307
      {
      set=0;
      mod=0;
  
      }
   }
else if (input(PIN_B5)==0)// xet thoi gian can chinh
     {
        mod++;
        if (mod==7) mod=1;
       
     }
else if (input(PIN_B6)==0)// tang
   {
   if (set==1)
   switch(mod)
     {
       case 1:
          gio++;
          if (gio==24) gio=0;
          break;
        case 2:
          phut++;
          if (phut==60) phut=0;
          break;
        case 3:
           ngay++;
           if((ngay==30)&(thang==4)||(thang==6)||(thang==9)||(thang==11))
              ngay=1;
           else if((thang==2)&&(nam%4==0)&&(ngay==30)) ngay=1;
           else if ((thang==2)&&(nam%4!=0)&&(ngay==29)) ngay=1;
           else if (ngay==32) ngay=1;
           break;
       case 4:
          thang++;
          if (thang==13) thang=1;
          break;
       case 5:
          nam++;
          if (nam==100) nam=0;
          break;
     }

   }
else if (input(PIN_B7)==0)// giam
    {
      switch(mod)
         {
           case 1:
              if (gio==0) gio=24;
              gio--;
              break;
           case 2:
              if (phut==0) phut=60;
              phut--;
              break;
           case 3:
               ngay--;
               if((ngay==0)&(thang==4)||(thang==6)||(thang==9)||(thang==11))
               ngay=30;
               else if((thang==2)&&(nam%4==0)&&(ngay==0)) ngay=29;
               else if ((thang==2)&&(nam%4!=0)&&(ngay==0)) ngay=28;
               else if (ngay==0) ngay=31;
               break;
           case 4:
              thang--;
              if (thang==0) thang=12;
              break;
              case 5:
              nam--;
              if (nam==0) nam=99;
              break;
         }
        
    }
else return;    
}      
void main()
{
   enable_interrupts(INT_RB);
   enable_interrupts(global);
   LCD_init();// khoi tao LCD
   LCD_Position(0x84);
   LCD_Char("LE MINH HA");
   LCD_Position(0xC5);
   LCD_char("TC2052");
   delay_ms(200);
   LCD_cmd(0x01);// xoa man hinh
   LCD_Position(0x80);
   LCd_char("Time");
   LCD_Position(0xc0);
   LCD_char("Date");
   While (TRUE)
    {
      if (set == 0)// lay thoi gian
       {
          giay=BCD2DEC(docdl(0)); 
          delay_us(100);
          phut=BCD2DEC(docdl(1));
          delay_us(100);
          gio =BCD2DEC(docdl(2));
          delay_us(100);
          ngay=BCD2DEC(docdl(4));
          delay_us(100);
          thang=BCD2DEC(docdl(5));
          delay_us(100);
          nam=BCD2DEC(docdl(6));
          delay_us(100);
      }
      if (set==1)
      update_ds1307();
      // hien thi 
        LCD_Position(0x85);
        hienthi(gio);
        LCD_char(":");
        hienthi(phut);
        LCD_char(":");
        hienthi(giay);
        LCD_Position(0xc5);
        hienthi(ngay);
        LCD_char(":");
        hienthi(thang);
        LCD_char(":");
        hienthi(nam);
// hien thi nhap nhay khi chinh thoi gian 
      switch(mod)
        {
   
      case 1: // chinh gio
          LCD_Position(0x85);
          LCD_char("  ");
          delay_ms(30);
          break;
      case 2: // chinh phut
          LCD_Position(0x88);
          LCD_Char("  ");
          delay_ms(30);
          break;
      case 3: // chinh nam
          LCD_Position(0xc5);
          LCD_char("  ");
          delay_ms(30);
          break;
      case 4: // chinh thang
          LCD_Position(0xc8);
          LCD_char("  ");
          delay_ms(30);
          break;
      case 5: // chinh ngay
          LCD_Position(0xcb);
          LCD_char("  ");
          delay_ms(30);
          break;
      }    
    }
}


               
               
               
