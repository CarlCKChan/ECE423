//
//  sd_card.h
//
//  Created by Rodolfo Pellizzoni on 01/02/15.
//  Copyright (c) 2015 __MyCompanyName__. All rights reserved.
//

#ifndef mjpeg423_sd_card_h
#define mjpeg423_sd_card_h

#include "altera_UP_SD_Card_Avalon_Interface.h"

//create list of sectors for a file
//Arguments: file_handle = handle to the file
//Return value: true if the sectors list is created; false otherwise
//Notes: the function must be called immediately after opening a file
//with a call to alt_up_sd_card_fopen
bool sd_card_create_sectors_list(short int file_handle);

//get file size
//Arguments: file_handle = handle to the file
//Return value: size of the file in bytes
unsigned int sd_card_file_size(short int file_handle);

//starts reading a sector
//Arguments: index = sector index to read
//Note: indexes go from 0 (first sector) to ceil(file_size/512)-1
//The function returns immediately; use the next function
//to determine when the read completes
void sd_card_start_read_sector(int index);

//wait until a read sector operation completes
//Return value: true if the sectors has been read correctly; false otherwise
//Note: the function polls the SD Card Interface until the operation completes
bool sd_card_wait_read_sector();

//SD Card Interface memory buffer
//Note: after a sector read is complete, the content of the sector
//can be read from this 512-bytes buffer in the SD Card Interface
extern volatile char		*buffer_memory;

#endif
