/////////////////////         www.mycircuits9.com            /////////////////
///////////////            THIS IS A 4-BIT LCD HEADER              //////////
//////////////                      CREATED BY                    //////////
/////////////                      MYCIRCUITS9                   //////////
//////////////////////////////////////////////////////////////////////////




void lcd_init();
void lcd_cmd(unsigned char b);
void lcd_str(unsigned char a);
void gotoxy(int x,int y);
void lcd_clear();
void lcd_array(char *a);
void lcd_init()
{
lcd_cmd(0x02);
lcd_cmd(0x28);
lcd_cmd(0x0c);
lcd_cmd(0x06);
lcd_cmd(0x80);
}

void lcd_cmd(unsigned char b)
{
int b1;
b1 = (b & 0xF0);
output_b(b1);
 output_high(PIN_B2);
delay_us(10);
output_low(PIN_B2);

b1 = ((b<<4) & 0xF0);
output_b(b1);
 output_high(PIN_B2);
delay_us(10);
output_low(PIN_B2);
}

void lcd_str(unsigned char a)
{
int a1;
a1=(a&0xF0);
output_b(a1 | 0x01);
output_high(PIN_B2);
delay_us(10);
output_low(PIN_B2);

a1=((a<<4)&0xF0);
output_b(a1 | 0x01);
output_high(PIN_B2);
delay_us(10);
output_low(PIN_B2);
}

void gotoxy(int x,int y)
{
if(y==0)
{
lcd_cmd(0x80+x);
}
else if(y==1)
{
lcd_cmd(0xc0+x);
}
}
void lcd_clear()
{
lcd_cmd(0x01);
lcd_cmd(0x80);
}
void lcd_array(char *a)
{
while(*a)
{
lcd_str(*a++);
}
}
