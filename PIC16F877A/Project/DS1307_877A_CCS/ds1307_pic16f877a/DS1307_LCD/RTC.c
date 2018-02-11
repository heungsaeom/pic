#include <16f877A.h>
#fuses XT
#use delay(clock=4M)
#include <DS1307.c>
#include  <lcd.c>

int sec;
int min;
int hrs;
int  day;
int  month;
int  yr;
int dow;
char fecha[4];

void main(){
   lcd_init();
   ds1307_set_date_time(18,2,16,0,21,42,0); // set time and day
while(true){
      ds1307_get_date (day, month, yr, dow);  /// se obtiene la fecha
      ds1307_get_time (hrs, min, sec);   /// se obtiene la hora
      ds1307_get_day_of_week(fecha);
      lcd_gotoxy(1,1);
      printf(lcd_putc,"%s %02u/%02u/20%02u",fecha,day,month,yr);
      lcd_gotoxy(2,2);
      printf(lcd_putc, "TIME: %02u:%02u:%02u",hrs, min, sec);
      delay_ms(200); 
  }
}


