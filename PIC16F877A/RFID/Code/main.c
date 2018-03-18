#include <main.h>
//khai bao bien toan cuc
//*****************************************************************************

//khai bao cac ten chuong trinh con
//*****************************************************************************
void _CaiDatLCD(void);
//su dung ngat
//*****************************************************************************

//chuong trinh chinh
//*****************************************************************************
//*****************************************************************************
void main()
{
   char UID[6],i;
   unsigned int TagType;  
  // char writeData[] = "Kevifa.com";

   _CaiDatLCD();
   MFRC522_Init ();
   
   lcd_gotoxy(1,1);//dinh vi toa do muon hien thi ki tu len LCD
   printf(lcd_putc,"RFID RC522");//hien thi chuoi ki tu len LCD
   
   while(TRUE)
   {
      //TODO: User Code
       if (MFRC522_isCard (&TagType)) //Check any card
       {             
           if(MFRC522_ReadCardSerial (&UID)) 
           {
               lcd_gotoxy(1,2);
               printf(lcd_putc,"ID:");
               for(i=0;i<5;i++)
               {
                  printf(lcd_putc,"%x",UID[i]);//hien thi tung ki tu nhan duoc
                  //o dang ma so hexa
               }                           
           }        
            //Ngu dong
            MFRC522_Halt() ;           
       }             
   }

}
//viet noi dung cho cac chuong trinh con da duoc dinh nghia truoc do
//*****************************************************************************
//*****************************************************************************
void _CaiDatLCD(void)
{
   lcd_init();//khoi dong module LCD
   lcd_putc('\f');//xoa trang man hinh LCD
}

