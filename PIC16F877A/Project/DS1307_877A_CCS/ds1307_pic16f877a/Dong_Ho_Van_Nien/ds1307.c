
int8 bin2bcd(int8 binary_value)
{
  int8 temp;
  char retval;

  temp = binary_value;
  retval = 0;

  while(1)
  {
    // Get the tens digit by doing multiple subtraction
    // of 10 from the binary value.
    if(temp >= 10)
    {
      temp -= 10;
      retval += 0x10;
    }
    else // Get the ones digit by adding the remainder.
    {
      retval += temp;
      break;
    }
  }

  return(retval);
}

// Input range - 00 to 99.
char bcd2bin(char bcd_value)
{
  char temp;

  temp = bcd_value;
  // Shifting upper digit right by 1 is same as multiplying by 8.
  temp >>= 1;
  // Isolate the bits for the upper digit.
  temp &= 0x78;

  // Now return: (Tens * 8) + (Tens * 2) + Ones

  return(temp + (temp >> 2) + (bcd_value & 0x0f));
}

void ds1307_init(unsigned char rs,unsigned char sqwe,unsigned char out)
{
   rs&=3;
   if (sqwe) rs|=0x10;
   if (out) rs|=0x80;
   i2c_start();
   i2c_write(0xd0);
   i2c_write(7);
   i2c_write(rs);
   i2c_stop();
}

void ds1307_set_date_time(BYTE day, int8 mth, BYTE year, BYTE dow, BYTE hr, BYTE min, BYTE sec)
{
  sec &= 0x7F;
  hr &= 0x3F;

  i2c_start();
  i2c_write(0xD0);            // I2C write address
  i2c_write(0x00);            // Start at REG 0 - Seconds
  i2c_write(bin2bcd(sec));      // REG 0
  i2c_write(bin2bcd(min));      // REG 1
  i2c_write(bin2bcd(hr));      // REG 2
  i2c_write(bin2bcd(dow));      // REG 3
  i2c_write(bin2bcd(day));      // REG 4
  i2c_write(bin2bcd(mth));      // REG 5
  i2c_write(bin2bcd(year));      // REG 6
  //i2c_write(0x80);            // REG 7 - Disable squarewave output pin
  i2c_stop();
}

void ds1307_set_time(byte hr, byte min, byte sec)
{
  sec &= 0x7F;
  hr &= 0x3F;
  i2c_start();
  i2c_write(0xD0);            // I2C write address
  i2c_write(0x00);            // Start at REG 0 - Seconds
  i2c_write(bin2bcd(sec));      // REG 0
  i2c_write(bin2bcd(min));      // REG 1
  i2c_write(bin2bcd(hr));      // REG 2
  //i2c_write(0x80);            // REG 7 - Disable squarewave output pin
  i2c_stop();
}

void ds1307_set_date(byte day, byte mth, byte year, byte dow)
{
 
  i2c_start();
  i2c_write(0xD0);            // I2C write address
  i2c_write(0x03);            // Start at REG 0 - Seconds
  i2c_write(bin2bcd(dow));
  i2c_write(bin2bcd(day));
  i2c_write(bin2bcd(mth));
  i2c_write(bin2bcd(year));
  //i2c_write(0x80);            // REG 7 - Disable squarewave output pin
  i2c_stop();
}

void ds1307_get_date(BYTE &day, BYTE &mth, BYTE &year, BYTE &dow)
{
  i2c_start();
  i2c_write(0xD0);
  i2c_write(0x03);            // Start at REG 3 - Day of week
  i2c_start();
  i2c_write(0xD1);
  dow  = bcd2bin(i2c_read() & 0x7f);   // REG 3
  day  = bcd2bin(i2c_read() & 0x3f);   // REG 4
  mth  = bcd2bin(i2c_read() & 0x1f);   // REG 5
  year = bcd2bin(i2c_read(0));            // REG 6
  i2c_stop();
}

void ds1307_get_time(BYTE &hr, BYTE &min, BYTE &sec)
{
  i2c_start();
  i2c_write(0xD0);
  i2c_write(0x00);            // Start at REG 0 - Seconds
  i2c_start();
  i2c_write(0xD1);
  sec = bcd2bin(i2c_read() & 0x7f);
  min = bcd2bin(i2c_read() & 0x7f);
  hr  = bcd2bin(i2c_read(0) & 0x3f);
  i2c_stop();

} 
