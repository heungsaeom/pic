/////////////////////////////////////////////////////////////////////////
////                                                                 ////
////  FAT16 file system library for CCS PIC C compiler.              ////
////                                                                 ////
////  This library for reading files only.                           ////
////  Long file name (LFN) is not supported (maximum file name is 12 ////
////  character)                                                     ////
////                        --User Functions--                       ////
////                                                                 ////
////  fat16_init();                                                  ////
////   Initializes the FAT library and the MMC/SD card. Returns 0 if ////
////   OK, 1 if error.                                               ////
////                                                                 ////
////  fat16_open_file(int8* fname);                                  ////
////   Opens file stream. fname is file name. Returns 0 if OK and 1  ////
////   if the opening process faced problem (file not found, long    ////
////   file name ...........)                                        ////
////                                                                 ////
////  fat16_read_byte(int8* data);                                   ////
////   Gets a character (byte) from a file stream opened with        ////
////   fat16_open_file(int8* fname).                                 ////
////   Returns 0 if OK, 1 if error or end of file is reached.        ////
////                                                                 ////
////  fat16_read_data(int32 size, int8* data);                       ////
////   Reads an array (data) of size 'size' from the opened file.    ////
////   Returns 0 if OK, 1 if error or end of file is reached.        ////
////                                                                 ////
/////////////////////////////////////////////////////////////////////////
////                                                                 ////
////  http://ccspicc.blogspot.com/                                   ////
////  electronnote@gmail.com                                         ////
////                                                                 ////
/////////////////////////////////////////////////////////////////////////

#include <string.h>
#ignore_warnings    240

int16 Bytes_Per_Cluster, fat_Start; 
int32 fat_Length, Root_Dir, Data_Start, address_pointer, file_pointer,
      file_size = 0, mbr_offset = 0;

int1 fat16_init();
int1 fat16_open_file(int8* fname);
int1 fat16_read_byte(int8* data);
int1 fat16_read_data(int32 size, int8* data);
int1 get_file_name(int32 file_entry_addr, int8 sname[]);

int1 sdcard_mbr_offset(int16 address){
  int8 _byte;
  if(sdcard_read_byte(address, &_byte) != 0)             // Read active byte of the partition
    return 1;                                            // Error occured while reading byte
  if(_byte != 0x80)
    return 0;
  sdcard_read_byte(address + 11, &mbr_offset);
  mbr_offset <<= 8;
  sdcard_read_byte(address + 10, &mbr_offset);
  mbr_offset <<= 8;
  sdcard_read_byte(address + 9, &mbr_offset);
  mbr_offset <<= 8;
  sdcard_read_byte(address +  8, &mbr_offset);
  mbr_offset *= 512;
  return 0;
}
int1 fat16_init(){
  const int8 *fat_sign = {"FAT"};
  int1 ec = 0;
  int8 FATs, Sectors_Per_Cluster, *file_system[3];
  int16  Bytes_Per_Sector, Reserved_Sectors, Root_Entries, Small_Sectors, Sectors_Per_FAT;
  int32  Hidden_Sectors, Total_Sectors;
  if(sdcard_init() != 0)
    return 1;                                      // Error initializing the SD card
  // Check the first sector of the SD card (MBR, FAT16 or FAT32)
  sdcard_read_data(0x52, 3, file_system);
  if(strcmp(file_system, fat_sign) == 0)
    return 1;                                      // SD Card with FAT32 system and without MBR (FAT32 not supported)
  sdcard_read_data(0x36, 3, file_system);
  if(strcmp(file_system, fat_sign) != 0){
  // First sector contains MBR (not FAT16 volume boot sector) ==> search for the first active partition offset
    if(sdcard_mbr_offset(0x1EE) != 0)  return 1;
    if(sdcard_mbr_offset(0x1DE) != 0)  return 1;
    if(sdcard_mbr_offset(0x1CE) != 0)  return 1;
    if(sdcard_mbr_offset(0x1BE) != 0)  return 1;
  }
  ec |= sdcard_read_byte(mbr_offset + 0x0C, &Bytes_Per_Sector);
  Bytes_Per_Sector <<= 8;
  ec |= sdcard_read_byte(mbr_offset + 0x0B, &Bytes_Per_Sector);
  ec |= sdcard_read_byte(mbr_offset + 0x0D, &Sectors_Per_Cluster);
  ec |= sdcard_read_byte(mbr_offset + 0x0F, &Reserved_Sectors);
  Reserved_Sectors <<= 8;
  ec |= sdcard_read_byte(mbr_offset + 0x0E, &Reserved_Sectors);
  ec |= sdcard_read_byte(mbr_offset + 0x10, &FATs);
  ec |= sdcard_read_byte(mbr_offset + 0x12, &Root_Entries);
  Root_Entries <<= 8;
  ec |= sdcard_read_byte(mbr_offset + 0x11, &Root_Entries);
  ec |= sdcard_read_byte(mbr_offset + 0x14, &Small_Sectors);
  Small_Sectors <<= 8;
  ec |= sdcard_read_byte(mbr_offset + 0x13, &Small_Sectors);
  ec |= sdcard_read_byte(mbr_offset + 0x17, &Sectors_Per_FAT);
  Sectors_Per_FAT <<= 8;
  ec |= sdcard_read_byte(mbr_offset + 0x16, &Sectors_Per_FAT);
  ec |= sdcard_read_byte(mbr_offset + 0x1F, &Hidden_Sectors);
  Hidden_Sectors <<= 8;
  ec |= sdcard_read_byte(mbr_offset + 0x1E, &Hidden_Sectors);
  Hidden_Sectors <<= 8;
  ec |= sdcard_read_byte(mbr_offset + 0x1D, &Hidden_Sectors);
  Hidden_Sectors <<= 8;
  ec |= sdcard_read_byte(mbr_offset + 0x1C, &Hidden_Sectors);
  ec |= sdcard_read_byte(mbr_offset + 0x23, &Total_Sectors);
  Total_Sectors <<= 8;
  ec |= sdcard_read_byte(mbr_offset + 0x22, &Total_Sectors);
  Total_Sectors <<= 8;
  ec |= sdcard_read_byte(mbr_offset + 0x21, &Total_Sectors);
  Total_Sectors <<= 8;
  ec |= sdcard_read_byte(mbr_offset + 0x20, &Total_Sectors);
  if(ec != 0)
    return 1;
  Bytes_Per_Cluster = Sectors_Per_Cluster * Bytes_Per_Sector;
  fat_Length = Sectors_Per_FAT * (int32)Bytes_Per_Sector;
  fat_Start = mbr_offset + Reserved_Sectors * Bytes_Per_Sector;
  Root_Dir = fat_Start + (FATs * fat_Length);
  Data_Start = mbr_offset + (Root_Entries * 0x20) + (Bytes_Per_Sector - 1);
  Data_Start /= Bytes_Per_Sector;
  Data_Start += Reserved_Sectors + (FATs * Sectors_Per_FAT);
  Data_Start *= Bytes_Per_Sector;
  return 0;
}
int1 fat16_open_file(int8* fname){
  int1 ok = 0, ec = 0;
  int8 target_file[13], s_name[12];
  int8 fname_parse_pos = 0, name_size;
  int16 start_cluster;
  int32 addr;
  while(fname[fname_parse_pos] != '\0' && fname_parse_pos < 13){
    target_file[fname_parse_pos] = tolower(fname[fname_parse_pos]);
    fname_parse_pos++;
   }
   target_file[fname_parse_pos] = '\0';
   name_size = strlen(target_file);
   if(name_size > 12)
     return 1;                                   // Return error (file name not supported)
   addr = Root_Dir;
   while(addr < Data_Start && ok == 0){
     if(get_file_name(addr, s_name) != 0)
       return 1;                                 // Error reading file name from the SD card
     if(strcmp(s_name, target_file) == 0)
       ok = 1;
     if(ok == 0)
       addr += 32;
   }
   if(ok == 0)
     return 1;                                   // File not found ==> Error
  ec |= sdcard_read_byte(addr + 0x1F, &file_size);
  file_size <<= 8;
  ec |= sdcard_read_byte(addr + 0x1E, &file_size);
  file_size <<= 8;
  ec |= sdcard_read_byte(addr + 0x1D, &file_size);
  file_size <<= 8;
  ec |= sdcard_read_byte(addr + 0x1C, &file_size);
  ec |= sdcard_read_byte(addr + 0x1B, &start_cluster);
  start_cluster <<= 8;
  ec |= sdcard_read_byte(addr + 0x1A, &start_cluster);
  address_pointer = Data_Start + (int32)(start_cluster - 2) * (int32)Bytes_Per_Cluster;
  file_pointer = 0;
  return ec;
}
int1 fat16_read_byte(int8* data){
  if(file_pointer == file_size)
    return 1;
  if(sdcard_read_byte(address_pointer, &*data) != 0)
    return 1;                                    // Error reading data
  address_pointer++;
  file_pointer++;
  return 0;
}
int1 fat16_read_data(int32 fsize, int8* fdata){
  if(file_pointer == file_size)
    return 1;
  if(file_pointer + fsize > file_size){
    fsize = file_size - file_pointer;
    fdata[fsize] = '\0';
  }
  if(sdcard_read_data(address_pointer, fsize, fdata) != 0)
    return 1;                                    // Error reading data
  address_pointer += fsize;
  file_pointer += fsize;
  return 0;
}
int1 get_file_name(int32 file_entry_addr, int8 sname[]){
  int8 buf, i, j = 0;
  for(i = 0; i < 11; i++){
    if(sdcard_read_byte(i + file_entry_addr, &buf) != 0)
      return 1;
    if(buf != ' '){         
      sname[j] = tolower(buf);
      j += 1;
    }
  }
  sname[j] = '\0';
  if(strlen(sname) > 3){
    j = strlen(sname);
    for(i=j; i > j-3; --i)
      sname[i] = sname[i-1];
    sname[i] = '.';
  }
  sname[j+1] = '\0';
  return 0;
}
