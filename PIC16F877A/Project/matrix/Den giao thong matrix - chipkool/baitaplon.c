#include<at89x52.h> 

//tre 1ms
void delayms (unsigned int  ms)
{


while(ms--)
{
	//chuong trinh tao re 1ms
	TMOD=0x01;
	TH0=0xFC;
	TL0=0x17;
	TR0=1;
	while(!TF0);

TF0=0;
TR0=0;
}
}

//chuong trinh chinh
void main()
{
 int n,i,k,j;
unsigned char mahang[8]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};//mahang logic 0
unsigned char macot[248]={
   //ma led ma tri 8x8 2 so 1 bong   tu 00=>30

 0x7C,0x44,0x7C,0x00,0x7C,0x44,0x7C,0x00,  	//00
   0x00,0x7C,0x44,0x7C,0x00,0x7C,0x00,0x00,  	//01
   0x00,0x7C,0x44,0x7C,0x00,0x5C,0x54,0x74,  	//02
   0x00,0x7C,0x44,0x7C,0x00,0x54,0x54,0x7C,  
   0x00,0x7C,0x44,0x7C,0x00,0x70,0x10,0x7C,  
   0x00,0x7C,0x44,0x7C,0x00,0x74,0x54,0x5C,  
   0x00,0x7C,0x44,0x7C,0x00,0x7C,0x54,0x5C,  
   0x00,0x7C,0x44,0x7C,0x00,0x40,0x40,0x7C,  
   0x00,0x7C,0x44,0x7C,0x00,0x7C,0x54,0x7C,  
   0x00,0x7C,0x44,0x7C,0x00,0x74,0x54,0x7C, 
 
   0x00,0x00,0x7C,0x00,0x00,0x74,0x54,0x7C,  
  0x00,0x00,0x7C,0x00,0x00,0x7C,0x00,0x00,  
    0x00,0x00,0x7C,0x00,0x5C,0x54,0x74,0x00,  
    0x00,0x00,0x7C,0x00,0x54,0x54,0x7C,0x00,
    0x00,0x00,0x7C,0x00,0x70,0x10,0x7C,0x00,  
    0x00,0x00,0x7C,0x00,0x74,0x54,0x5C,0x00,  
    0x00,0x00,0x7C,0x00,0x7C,0x54,0x5C,0x00,  
    0x00,0x00,0x7C,0x00,0x40,0x40,0x7C,0x00,  
   0x00,0x00,0x7C,0x00,0x7C,0x54,0x7C,0x00,  
   0x00,0x00,0x7C,0x00,0x74,0x54,0x7C,0x00,  

   0x5C,0x54,0x74,0x00,0x7C,0x44,0x7C,0x00, 
  0x5C,0x54,0x74,0x00,0x00,0x7C,0x00,0x00,  
     0x5C,0x54,0x74,0x00,0x5C,0x54,0x74,0x00,  
   0x5C,0x54,0x74,0x00,0x54,0x54,0x7C,0x00,  
    0x5C,0x54,0x74,0x00,0x70,0x10,0x7C,0x00,  
     0x5C,0x54,0x74,0x00,0x74,0x54,0x5C,0x00,  
     0x5C,0x54,0x74,0x00,0x7C,0x54,0x5C,0x00,  
     0x5C,0x54,0x74,0x00,0x40,0x40,0x7C,0x00,
   0x5C,0x54,0x74,0x00,0x7C,0x54,0x7C,0x00,  
   0x5C,0x54,0x74,0x00,0x74,0x54,0x7C,0x00,  
   0x54,0x54,0x7C,0x0,0x7C,0x44,0x7C,0x00  		//30   
							};//ma cot tich cuc muc 1			//khai bao trang thai ban dau cua cac den bao P0 dieu khien

 P0=0x3E; //trang thai den bao
	
while(1)	
{	
   
	for(k=45;k>=0;k--)					  //vong quet led truoc
  {
	for(i=50;i>=0;i--)				 //vong quet led sau
	{
		for(j=0;j<20;j++)		   //tao thoi gian tre hien thi	nhay 100lan
		{	 
		  	for(n=0;n<8;n++)	 	//vong quet tin hieu
		{
		P3=mahang[n];  			//quet hang
		P1=macot[i*8+n]; 			//quet cot	led sau
		 	P2=macot[k*8+n];		//quet cot led truoc
	 delayms(1);  				//thoigian hien thi 1 cot
		 
		 //dieu khien led bao 1 chu ki 	dau
		 
		 if(k==0&&i==6)
		 {	 //bat den vang
			P0_1=0;
			P0_5=0;
			//xoa het den xanh do
			  		P0_2=1;
				   P0_4=1;
				   P0_0=1;
				   P0_6=1;
			}
			
			}
		}
		if(k==0&&i==0)	
				   {
				   	P0=0xE3; //CHO PHEp DDI THANG VA CAM RE PHAI RE PHAI DOC	,CAM DI THANG NGANG,VA CHO PHEP RE PHAI NGANG
				  	}
	}
	
		 
	}
	 //nua chu ki sau
	for(k=4;k>=0;k--)					  //vong quet led truoc
  {
	for(i=9;i>=0;i--)				 //vong quet led sau
	{
		for(j=0;j<20;j++)		   //tao thoi gian tre hien thi	nhay 60lan
		{	 
		  	for(n=0;n<8;n++)	 	//vong quet tin hieu
		{
		P3=mahang[n];  			//quet hang
		P1=macot[i*8+n]; 			//quet cot	led sau
		 	P2=macot[k*8+n];		//quet cot led truoc
		 delayms(1);  				//thoigian hien thi 1 cot
		 //dieu khien led bao 1 chu ki 
		 if(k==0&&i==6)
		 {	 //bat den vang
			P0_1=0;
			P0_5=0;
			//xoa het den xanh do
			  		P0_2=1;
				   P0_4=1;
				   P0_0=1;
				   P0_6=1;
			}
			
			}
		}
		if(k==0&&i==0)					
				   {
							//lat tin hieu xanh do
				   	 P0=0x3E; //CAM DI THANG VA CHO PHEP RE PAHI DOC,CHO PHEP DI NGANG VA CAM RE PAHI NGANG
				 }
	}
	
		 
	}

}
}