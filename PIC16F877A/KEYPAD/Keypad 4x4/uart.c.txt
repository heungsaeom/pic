#ifndef __UART_
#define __UART_
void UART_config()
{
#use rs232(baud=9600,parity=N,xmit=PIN_C6,rcv=PIN_C7,bits=8,stream=PORT1)
//#use rs232(baud=9600,parity=E,xmit=PIN_C6,rcv=PIN_C7,bits=8,stream=PORT1)
//#use rs232(baud=9600,parity=O,xmit=PIN_C6,rcv=PIN_C7,bits=8,stream=PORT1)

}
#endif