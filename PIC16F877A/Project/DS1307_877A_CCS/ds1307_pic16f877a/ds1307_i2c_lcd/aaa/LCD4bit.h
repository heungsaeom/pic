   
#define lcd_RS        PIN_D1
#define lcd_RW        PIN_D2
#define lcd_E         PIN_D3
#define lcd_B1        PIN_D4
#define lcd_B2        PIN_D5
#define lcd_B3        PIN_D6
#define lcd_B4        PIN_D7

#define dong_1          0x80
#define dong_2          0xC0
#define Xoa_lcd         0x01

//====================Cac ham dung ngoai thu vien==============
#separate void LCD_Init();
#separate void LCD_Position(int x);
#separate void LCD_Char(int x);
#separate void LCD_Cmd(int x);
#separate void LCD_Pulse();
#separate void LCD_CGRAM();

//==========================Chuong trinh========================
#separate void LCD_Data (int x)
{
   output_bit(lcd_B1, x & 0x01);
   output_bit(lcd_B2, x & 0x02);
   output_bit(lcd_B3, x & 0x04);
   output_bit(lcd_B4, x & 0x08);
}

#separate void LCD_Pulse() 
{
   output_high(lcd_E);
   delay_us ( 3); // was 10
   output_low(lcd_E);
   delay_us ( 150); // was 5
}

#separate void LCD_Position(int x)
{
   LCD_Data(Swap(x) | 0x08);
   LCD_Pulse();
   LCD_Data(Swap(x));
   LCD_Pulse();
}

#separate void LCD_Cmd (int x)
{
   LCD_Data(Swap(x));
   LCD_Pulse();
   LCD_Data(Swap(x));
   LCD_Pulse();
   if (x == 0x01) delay_us(2000);
}

#separate void LCD_Char (int x) 
{
   output_high(lcd_RS);
   LCD_Data(Swap(x));
   LCD_Pulse();
   LCD_Data(Swap(x));
   LCD_Pulse();
   output_low(lcd_RS);
}

#separate void LCD_Init ()
{
//   output_high(lcd_open);
   LCD_Data(0x00);
   delay_ms(20);         //cho cho VDD len >> 15ms
   output_low (lcd_RS);  //mac dinh la viet lenh
   output_low (lcd_RW);  //mac dinh la LCD nhan lenh 
   LCD_Data(0x03);       //che do 4 bit
   LCD_Pulse();
   LCD_Pulse();
   LCD_Pulse();
   LCD_Data(0x02);       //xoa man hinh
   LCD_Pulse();
   LCD_Cmd(0x2c);
   LCD_Cmd(0x0c);        //bat hien thi tat con tro
   LCD_Cmd(0x06);        //tang con tro
   LCD_Cmd(0x01);        //xoa man hinh hien thi  
}

