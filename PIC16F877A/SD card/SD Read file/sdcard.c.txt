/////////////////////////////////////////////////////////////////////////
////                                                                 ////
////    MMC/SD card driver for CCS PIC C compiler (August 2017)      ////
////                                                                 ////
////                        --User Functions--                       ////
////                                                                 ////
////  sdcard_init();                                                 ////
////   Initializes the media. Returns 0 if OK, non-zero if error.    ////
////                                                                 ////
////  sdcard_read_byte(int32 addr, int8* data);                      ////
////   Reads a byte from the MMC/SD card at address 'addr', saves to ////
////   pointer 'data'. Returns 0 if OK, non-zero if error.           ////
////                                                                 ////
////  sdcard_read_sector(int32 sector_number, int8* data);           ////
////   Reads an entire sector (block) from the SD/MMC. Returns 0 if  ////
////   OK, non-zero if error                                         ////
////                                                                 ////
////  sdcard_read_data(int32 addr, int16 size, int8* data);          ////
////   Reads data of size 'size' bytes from the MMC/SD card starting ////
////   at address a, saves result to pointer 'data'. Returns 0 if OK,////
////   non-zero if error.                                            ////
////                         ////////                                ////
////  The following three functions are for writing data to the SD   ////
////  card and to use them we must define 'SDCARD_WRITE' in the main ////
////  code page. This gives us more free in RAM and to use these     ////
////  functions the microcontroller must have RAM with more than     ////
////  512 bytes.                                                     ////
////                                                                 ////
////  sdcard_write_sector(int32 sector_number, int8* data);          ////
////   Writes an entire sector (block) from the SD/MMC. Returns 0 if ////
////   OK, non-zero if error.                                        ////
////                                                                 ////
////  sdcard_write_byte(int32 addr, int8 &data);                     ////
////   Writes byte 'data' to address 'addr'. Returns 0 if OK,        ////
////   non-zero if error.                                            ////
////                                                                 ////
////  sdcard_write_data(int32 addr, int16 size, int8 *data);         ////
////   Writes data of 'size' bytes from pointer 'data' starting at   ////
////   address 'addr'. Returns 0 if OK, non-zero if error.           ////
////                                                                 ////
/////////////////////////////////////////////////////////////////////////
////                                                                 ////
////  http://ccspicc.blogspot.com/                                   ////
////  electronnote@gmail.com                                         ////
////                                                                 ////
/////////////////////////////////////////////////////////////////////////

#include <string.h>

int16 timeout;
enum _card_type{MMC, SDSC, SDHC} g_card_type;
enum sdcard_err{
  sdcard_goodec = 0, sdcard_idle = 0x01, sdcard_timeout = 0x80,
  sdcard_illegal_cmd = 0x04 };

#define GO_IDLE_STATE 0
#define SEND_IF_COND 8
#define SET_BLOCKLEN 16
#define READ_SINGLE_BLOCK 17
#define WRITE_BLOCK 24
#define SEND_APP_OP_COND 41
#define APP_CMD 55
#define READ_OCR 58

void send_sdcard_command(int8 command, int32 sd_data, int8 sd_crc);
sdcard_err sdcard_init();
sdcard_err sdcard_read_byte(int32 addr, int8* data);
sdcard_err sdcard_read_sector(int32 sector_number, int8* data);
sdcard_err sdcard_read_data(int32 addr, int16 size, int8* data);
sdcard_err sdcard_write_sector(int32 sector_number, int8* data);
sdcard_err sdcard_write_byte(int32 addr, int8 &data);
sdcard_err sdcard_write_data(int32 addr, int16 size, int8 *data);
sdcard_err sdcard_get_r1();
sdcard_err sdcard_get_r7();
sdcard_err sdcard_go_idle_state();
sdcard_err sdcard_send_if_cond();
sdcard_err sdcard_send_app_cmd();
sdcard_err sdcard_sd_send_op_cond();
sdcard_err sdcard_read_ocr(int8* _ocr_byte_3);
sdcard_err sdcard_set_blocklen();
void sdcard_select();
void sdcard_deselect();

sdcard_err sdcard_init(){
  int8 i, resp, ocr_byte_3;
#if defined(SDCARD_SPI_HW)
  SETUP_SPI(SPI_MASTER | SPI_H_TO_L | SPI_CLK_DIV_64 | SPI_XMIT_L_TO_H);
  #define sdcard_xfer(x)    spi_read(x)
#else
  #if defined(SDCARD_PIN_SCL)
   output_drive(SDCARD_PIN_SCL);
  #endif
  #if defined(SDCARD_PIN_SDO)
   output_drive(SDCARD_PIN_SDO);
  #endif
  #if defined(SDCARD_PIN_SDI)
   output_float(SDCARD_PIN_SDI);
  #endif
    #use spi(MASTER, DI=SDCARD_PIN_SDI, DO=SDCARD_PIN_SDO, CLK=SDCARD_PIN_SCL, BITS=8, MSB_FIRST, MODE=3, baud=400000)
    #define sdcard_xfer(x)    spi_xfer(x)
#endif
  output_high(SDCARD_PIN_SELECT);
  output_drive(SDCARD_PIN_SELECT);
  delay_ms(250);
  for(i = 0; i < 10; i++)                        // Send 80 cycles
    sdcard_xfer(0xFF);
  sdcard_select();
  resp = sdcard_go_idle_state();                 // Send CMD0
  sdcard_deselect();
  if(resp != sdcard_idle)
    return resp;
  sdcard_select();
  resp = sdcard_send_if_cond();                  // Send CMD8
  sdcard_deselect();
  if(resp != sdcard_idle)
    return resp;
  i = 0;
  do{
    sdcard_select();
    resp = sdcard_send_app_cmd();                // Send CMD58
    if((resp != sdcard_idle) && (resp != sdcard_illegal_cmd) && (resp != 0)){
      sdcard_deselect(); return resp;}
    resp = sdcard_sd_send_op_cond();             // Send ACMD41
    sdcard_deselect();
    delay_ms(100);
    i++;
  } while((resp == 0x01) && (i < 254));
  sdcard_deselect();
  if((resp != 0 || i >= 254) && (resp != sdcard_illegal_cmd))
    return sdcard_timeout;
  if(resp == 0x04) g_card_type =  MMC;           // MMC type
  else             g_card_type = SDSC;           // SDSC or SDHC type
  if(g_card_type == SDSC){
    sdcard_select();
    resp = sdcard_read_ocr(&ocr_byte_3);
    sdcard_deselect();
    if(resp != sdcard_idle && resp != sdcard_illegal_cmd)
      return resp;
    if(resp != sdcard_illegal_cmd){
      if(bit_test(ocr_byte_3, 6))                  // If bit 30 of the OCR register is 1 (CCS is 1) ==> SDHC type
        g_card_type =  SDHC;
    }
  }
  sdcard_select();
  resp = sdcard_set_blocklen();
  sdcard_deselect();
  if(resp != 0)
    return sdcard_timeout;
#if defined(SDCARD_SPI_HW)
  // Speed up the SPI bus
  SETUP_SPI(SPI_MASTER | SPI_H_TO_L | SPI_CLK_DIV_4 | SPI_XMIT_L_TO_H);
#else
   #use spi(MASTER, DI=SDCARD_PIN_SDI, DO=SDCARD_PIN_SDO, CLK=SDCARD_PIN_SCL, BITS=8, MSB_FIRST, MODE=3)
#endif
  return sdcard_goodec;
}
sdcard_err sdcard_read_byte(int32 addr, int8* data){
  int8 response;
  int16 i, byte_addr;
  int32 sector_number;
  timeout = 0xFFFF;
  sector_number = addr/512;
  if(g_card_type != SDHC)
    sector_number = sector_number << 9;
  byte_addr = addr % 512;
  sdcard_select();
  send_sdcard_command(17, sector_number, 0xFF);
  while(timeout){
    response = sdcard_xfer(0xFF);
    if(response == 0xFE){
      for(i = 0; i < byte_addr; i++)
        sdcard_xfer(0xFF);
      *data = sdcard_xfer(0xFF);
      byte_addr++;
      for(i = byte_addr; i < 512; i++)
        sdcard_xfer(0xFF);
      for(i = 0; i < 2; i++)
        sdcard_xfer(0xFF);
      sdcard_deselect();
      return sdcard_goodec;
    }
    timeout--;
  }
  sdcard_deselect();
  return sdcard_timeout; 
}
sdcard_err sdcard_read_sector(int32 sector_number, int8* data){
  int8 response;
  int16 i;
  timeout = 0xFFFF;
  sdcard_select();
  for(i = 0; i < 10; i++)  sdcard_xfer(0xFF);
  sdcard_deselect();
  if(g_card_type != SDHC)
    sector_number = sector_number << 9;
  sdcard_select();
  send_sdcard_command(17, sector_number, 0xFF);
  while(timeout){
    response = sdcard_xfer(0xFF);
    if(response == 0xFE){
      for(i = 0; i < 512; i++)
        data[i] = sdcard_xfer(0xFF);
      for(i = 0; i < 2; i++)
        sdcard_xfer(0xFF);
      sdcard_deselect();
      return sdcard_goodec;
    }
    timeout--;
  }
  sdcard_deselect();
  return sdcard_timeout; 
}
sdcard_err sdcard_read_data(int32 addr, int16 size, int8* data){
  int8 response = 0;
  int16 i, byte_addr, byte_number = 0;
  int32 sector_number;
  timeout = 0xFFFF;
  sector_number = addr/512;
  byte_addr = addr % 512;
  next_sector:
  if(g_card_type != SDHC)
    sector_number = sector_number << 9;
  sdcard_select();
  send_sdcard_command(17, sector_number, 0xFF);
  while(timeout){
    response = sdcard_xfer(0xFF);
    if(response == 0xFE){
      for(i = 0; i < byte_addr; i++)
        sdcard_xfer(0xFF);
      if((byte_addr + size) < 512){
        size += byte_addr;
        for(i = byte_addr; i < size; i++, byte_number++)
          data[byte_number] = sdcard_xfer(0xFF);
        for(i = size; i < 512; i++)
          sdcard_xfer(0xFF);
      }
      else{
        for(i = byte_addr; i < 512; i++, byte_number++, size--)
          data[byte_number] = sdcard_xfer(0xFF);
        for(i = 0; i < 2; i++)
          sdcard_xfer(0xFF);
        sdcard_deselect();
        if(g_card_type != SDHC)
          sector_number = sector_number >> 9;
        sector_number++;
        byte_addr = 0;
        goto next_sector;
      }
      for(i = 0; i < 2; i++)
        sdcard_xfer(0xFF);
      sdcard_deselect();
      return sdcard_goodec;
    }
    timeout--;
  }
  sdcard_deselect();
  return sdcard_timeout; 
}
#if defined(SDCARD_WRITE)
sdcard_err sdcard_write_sector(int32 sector_number, int8 *data){
  int8 response;
  int16 i;
  timeout = 0xFFFF;
  if(g_card_type != SDHC)
    sector_number = sector_number << 9;
  sdcard_select();
  send_sdcard_command(24, sector_number, 0xFF);
  while(timeout){
    response = sdcard_xfer(0xFF);
    if(response != 0xFF){
      sdcard_xfer(0xFE);
      for(i = 0; i < 512; i++)                   // Send 512 data bytes
        sdcard_xfer(data[i]);
      for(i = 0; i < 2; i++)                     // Send 2 CRC bytes
        sdcard_xfer(0xFF);
      // get the error code back from the card; "data accepted" is 0bXXX00101
      response = sdcard_get_r1();
      if(response & 0x0A){
        sdcard_deselect();
        return response;
      }
      // wait for the line to go back high, this indicates that the write is complete
      while(sdcard_xfer(0xFF) == 0);
      sdcard_deselect();
      return sdcard_goodec;
    }
    timeout--;
  }
  sdcard_deselect();
  return sdcard_timeout;
}
sdcard_err sdcard_write_byte(int32 addr, int8 &data){
  int8  sector_data[512];
  int16 byte_addr;
  int32 sector_number;
  sector_number = addr/512;
  byte_addr = addr % 512;
  if(sdcard_read_sector(sector_number, sector_data) != 0)
    return 1;
  sector_data[byte_addr] = data;
  if(sdcard_write_sector(sector_number, sector_data) != 0)
    return 1;
  return sdcard_goodec;
}
sdcard_err sdcard_write_data(int32 addr, int16 size, int8 *data){
  int8  sector_data[512];
  int16 i, byte_addr;
  int32 sector_number;
  sector_number = addr/512;
  byte_addr = addr % 512;
  next_sector:
  if(sdcard_read_sector(sector_number, sector_data) != 0)
    return 1;
  if(byte_addr + size < 513){
    size += byte_addr;
    for(i = byte_addr; i < size; i++)  sector_data[i] = data[i - byte_addr];
    if(sdcard_write_sector(sector_number, sector_data) != 0)
      return 1;
  }
  else{
    for(i = byte_addr; i < 512; i++, size--)  sector_data[i] = data[i - byte_addr];
    if(sdcard_write_sector(sector_number, sector_data) != 0)
      return 1;
    byte_addr = 0;
    sector_number++;
    goto next_sector;
  }
  return sdcard_goodec;
}
#endif
void send_sdcard_command(int8 command, int32 sd_data, int8 sd_crc){
  int8 i;
  sdcard_xfer(0x40 | command);
  for(i = 0; i < 4; i++)
    sdcard_xfer(sd_data >> (3 - i) * 8);
  sdcard_xfer(sd_crc);
}
sdcard_err sdcard_go_idle_state(){
  send_sdcard_command(GO_IDLE_STATE, 0, 0x95);
  return sdcard_get_r1();
}
sdcard_err sdcard_send_if_cond(){
  send_sdcard_command(SEND_IF_COND, 0x1AA, 0x87);
  return sdcard_get_r7(); 
}
sdcard_err sdcard_send_app_cmd(){
  send_sdcard_command(APP_CMD, 0, 0xFF);
  return sdcard_get_r1();
}
sdcard_err sdcard_sd_send_op_cond(){
  send_sdcard_command(SEND_APP_OP_COND, 0x40000000, 0xFF);
  return sdcard_get_r1();
}
sdcard_err sdcard_read_ocr(int8* _ocr_byte_3){
  unsigned int8 i, response;
  timeout = 0xFFFF;
  send_sdcard_command(READ_OCR, 0, 0xFF);
  while(timeout){
    response = sdcard_xfer(0xFF);
    if(response != 0xFF){
      if(response == 0x04) return response;
      *_ocr_byte_3 = sdcard_xfer(0xFF);
      for(i = 0; i < 3; i++)
        sdcard_xfer(0xFF);
      return sdcard_idle;
    timeout--;
    }
  }
  return sdcard_timeout;
}
sdcard_err sdcard_set_blocklen(){
  send_sdcard_command(SET_BLOCKLEN, 512, 0xFF);
  return sdcard_get_r1();
}
sdcard_err sdcard_get_r1(){
  int8 response = 0;
  timeout = 0xFFFF;
  while(timeout){
    response = sdcard_xfer(0xFF);
    if(response != 0xFF){
      return response;
    }
    timeout--;
  }
  return sdcard_timeout;
}
sdcard_err sdcard_get_r7(){
  int8 i, response = 0;
  timeout = 0xFFFF;
  while(timeout){
    response = sdcard_xfer(0xFF);
    if(response != 0xFF){
      for(i = 0; i < 4; i++)
        sdcard_xfer(0xFF);
      return sdcard_idle;
    }
    timeout--;
  }
  return sdcard_timeout;
}
void sdcard_select(){
  output_low(SDCARD_PIN_SELECT);
}
void sdcard_deselect(){
  output_high(SDCARD_PIN_SELECT);
  sdcard_xfer(0xFF);
}
