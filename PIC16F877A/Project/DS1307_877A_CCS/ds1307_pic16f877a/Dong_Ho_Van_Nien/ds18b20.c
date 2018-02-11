int8 ds1820_read()
{
 int8 busy=0, temp1, temp2;
 int16 temp3;
 int8 result;

 onewire_reset();
 onewire_write(0xCC);
 onewire_write(0x44);

 while (busy == 0)
 busy = onewire_read();

 onewire_reset();
 onewire_write(0xCC);
 onewire_write(0xBE);
 temp1 = onewire_read();
 temp2 = onewire_read();
 temp3 = make16(temp2, temp1);
 
 //result = (float) temp3 / 2.0;   //Calculation for DS18S20 with 0.5 deg C resolution
 result = temp3 / 16.0;  //Calculation for DS18B20 with 0.1 deg C resolution
 delay_ms(2);
 return(result);
} 
void ds18b20_configure(int8 TH, int8 TL, int8 config) {
    onewire_reset();
    onewire_write(0xCC);            //Skip ROM, address all devices
    onewire_write(0x4E);            //Write to scratchpad
    onewire_write(TH);
    onewire_write(TL);
    onewire_write(config);
}
