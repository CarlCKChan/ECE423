#include <stdlib.h>
#include <math.h>
#include "sd_card.h"
#include "io.h"
#include <stdint.h>
#include <stdio.h>

#define MAX_FILES_OPENED				20
#define CMD_READ_BLOCK					17

struct s_file_record {
	unsigned char name[8];
	unsigned char extension[3];
	unsigned char attributes;
	unsigned short int create_time;
	unsigned short int create_date;
	unsigned short int last_access_date;
	unsigned short int last_modified_time;
	unsigned short int last_modified_date;
	unsigned short int start_cluster_index;
	unsigned int file_size_in_bytes;
	/* The following fields are only used when a file has been created or opened. */
	unsigned int current_cluster_index;
    unsigned int current_sector_in_cluster;
	unsigned int current_byte_position;
    // Absolute location of the file record on the SD Card.
    unsigned int file_record_cluster;
    unsigned int file_record_sector_in_cluster;
    short int    file_record_offset;
    // Is this record in use and has the file been modified.
    unsigned int home_directory_cluster;
    bool         modified;
	bool		 in_use;
};

struct s_FAT_12_16_boot_sector {
	unsigned char jump_instruction[3];
	char OEM_name[8];
	unsigned short int sector_size_in_bytes;
	unsigned char sectors_per_cluster;
	unsigned short int reserved_sectors;
	unsigned char number_of_FATs;
	unsigned short int max_number_of_dir_entires;
	unsigned short int number_of_sectors_in_partition;
	unsigned char media_descriptor;
	unsigned short int number_of_sectors_per_table;
	unsigned short int number_of_sectors_per_track;
	unsigned short int number_of_heads;
	unsigned int number_of_hidden_sectors;
	unsigned int total_sector_count_if_above_32MB;
	unsigned char drive_number;
	unsigned char current_head;
	unsigned char boot_signature;
	unsigned char volume_id[4];
	char volume_label[11];
	unsigned char file_system_type[8];
	unsigned char bits_for_cluster_index;
	unsigned int first_fat_sector_offset;
	unsigned int second_fat_sector_offset;
	unsigned int root_directory_sector_offset;
	unsigned int data_sector_offset;
};

bool get_cluster_flag(unsigned int cluster_index, unsigned short int *flag);

extern struct s_file_record active_files[MAX_FILES_OPENED];
extern volatile short int	*aux_status_register;
extern volatile short int	*command_register;
extern volatile int			*command_argument_register;
extern struct s_FAT_12_16_boot_sector boot_sector_data;
extern int	fat_partition_offset_in_512_byte_sectors;
extern unsigned int current_sector_index;

unsigned int *sectors = NULL;

int sd_card_curr_read_sector = 0;
unsigned int sd_card_start_byte_in_sector = 0;
int sd_card_sectors_num = 0;

/*
 * Set indices into the SD card for a certain point in the file with the sector list.
 * @param start_byte_in_file : Byte in the current file (has sector list loaded) from which to start reading
 * @return:	Returns 0
 */
int sd_seek(uint32_t start_byte_in_file) {
	sd_card_curr_read_sector = start_byte_in_file / 512;
	sd_card_start_byte_in_sector = start_byte_in_file % 512;
	return 0;
}

/*
 * Read a stream of bytes for a file (in the position start_byte_in_file), to a length of read_length, on an SD card.
 * @param buf	: Pointer to buffer to start writing to.
 * @param count	: Number of bytes to read from SD card.
 * @return: On success, returns number of bytes read.
 * 			If at end of file, returns -1.
 * 			On error, returns -2.
 */
int sd_read(void* buf, int count) {
	int bytes_read = 0;

	if (count <= 0) { return 0; }
	if (sd_card_curr_read_sector >= sd_card_sectors_num) { return -1; }

	while (sd_card_curr_read_sector < sd_card_sectors_num){
		sd_card_start_read_sector(sd_card_curr_read_sector);
		if(!sd_card_wait_read_sector()){
			printf("Cannot read %d-th sector\n", sd_card_curr_read_sector);
			return -2;
		}

		//move sector to file buffer 32bits at a time
		for(int j = sd_card_start_byte_in_sector; j < 512; j+=4){
			*((uint32_t*)(buf+j-sd_card_start_byte_in_sector)) = IORD_32DIRECT(buffer_memory, j);
			bytes_read += 4;
			if (bytes_read >= count) {
				sd_card_start_byte_in_sector = j + 4;
				return bytes_read;
			}
		}
		buf+= (512 - sd_card_start_byte_in_sector);

		sd_card_curr_read_sector++;
		sd_card_start_byte_in_sector = 0;
	}
	return bytes_read;
}


bool sd_card_create_sectors_list(short int file_handle)
{
	if(sectors != NULL) free(sectors);

	//determine number of sectors and clusters
	int sectors_num = ceil(active_files[file_handle].file_size_in_bytes/512.0);
	sd_card_sectors_num = sectors_num;			// Store in global variables for when reading

	//allocate sector list
	sectors = malloc(4*sectors_num);

	// Reset the offsets into the sectors and bytes in the sector
	sd_card_curr_read_sector = 0;
	sd_card_start_byte_in_sector = 0;

	//set first sector
	int data_sector = boot_sector_data.data_sector_offset + (active_files[file_handle].current_cluster_index - 2)*boot_sector_data.sectors_per_cluster +
			active_files[file_handle].current_sector_in_cluster;
	sectors[0] = data_sector;

	//set other sectors
	for(int i = 1; i < sectors_num; i++){

		// Read in a new sector of data.
		if (active_files[file_handle].current_sector_in_cluster == boot_sector_data.sectors_per_cluster - 1)
		{
			// Go to the next cluster.
			unsigned short int next_cluster;
			if (get_cluster_flag(active_files[file_handle].current_cluster_index, &next_cluster))
			{
				if ((next_cluster & 0x0000fff8) == 0x0000fff8)
				{
					/* End of file */
					return false;
				}
				else
				{
					active_files[file_handle].current_cluster_index = next_cluster;
					active_files[file_handle].current_sector_in_cluster = 0;
					data_sector = boot_sector_data.data_sector_offset + (active_files[file_handle].current_cluster_index - 2)*boot_sector_data.sectors_per_cluster +
					  active_files[file_handle].current_sector_in_cluster;
				}
			}
			else
			{
				return false;
			}
		}
		else
		{
			active_files[file_handle].current_sector_in_cluster = active_files[file_handle].current_sector_in_cluster + 1;
			data_sector = data_sector + 1;
		}

		sectors[i] = data_sector;
	}
	return true;
}

unsigned int sd_card_file_size(short int file_handle)
{
	return active_files[file_handle].file_size_in_bytes;
}

void sd_card_start_read_sector(int index)
{

	/* Multiply sector offset by sector size to get the address. Sector size is 512. Also,
	 * the SD card reads data in 512 byte chunks, so the address must be a multiple of 512. */
	IOWR_32DIRECT(command_argument_register, 0, (sectors[index] + fat_partition_offset_in_512_byte_sectors)*512);
	IOWR_16DIRECT(command_register, 0, CMD_READ_BLOCK);
	current_sector_index = sectors[index]+fat_partition_offset_in_512_byte_sectors;
}

bool sd_card_wait_read_sector()
{
	short int reg_state;
	do {
		reg_state = (short int) IORD_16DIRECT(aux_status_register,0);
	} while ((reg_state & 0x04)!=0);
	// Make sure the request did not time out.
	if ((reg_state & 0x10) == 0)
		return true;
	else
		return false;
}
