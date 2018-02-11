/***********************1Wire Class***********************/
/*Description: This class handles all communication */
/* between the processor and the 1wire */
/* sensors.
/*********************************************************/

/*-------1-wire definitions-------*/
//#define ONE_WIRE_PIN PIN_B0

/*******************1-wire communication functions********************/


void onewire_reset()  // OK if just using a single permanently connected device
{
output_low(ONE_WIRE_PIN);
delay_us( 500 ); // pull 1-wire low for reset pulse
output_float(ONE_WIRE_PIN); // float 1-wire high
delay_us( 500 ); // wait-out remaining initialisation window.
output_float(ONE_WIRE_PIN);
}

/*********************** onewire_write() ********************************/
/*This function writes a byte to the sensor.*/
/* */
/*Parameters: byte - the byte to be written to the 1-wire */
/*Returns: */
/*********************************************************************/

void onewire_write(int data)
{
int count;

for (count=0; count<8; ++count)
{
  output_low(ONE_WIRE_PIN);
  delay_us( 2 ); // pull 1-wire low to initiate write time-slot.
  output_bit(ONE_WIRE_PIN, shift_right(&data,1,0)); // set output bit on 1-wire
  delay_us( 60 ); // wait until end of write slot.
  output_float(ONE_WIRE_PIN); // set 1-wire high again,
  delay_us( 2 ); // for more than 1us minimum.
}
}

/*********************** read1wire() *********************************/
/*This function reads the 8 -bit data via the 1-wire sensor. */
/* */
/*Parameters: */
/*Returns: 8-bit (1-byte) data from sensor */
/*********************************************************************/

int onewire_read()
{
int count, data;

for (count=0; count<8; ++count)
{
  output_low(ONE_WIRE_PIN);
  delay_us( 2 ); // pull 1-wire low to initiate read time-slot.
  output_float(ONE_WIRE_PIN); // now let 1-wire float high,
  delay_us( 8 ); // let device state stabilise,
  shift_right(&data,1,input(ONE_WIRE_PIN)); // and load result.
  delay_us( 120 ); // wait until end of read slot.
}

return( data );
}
