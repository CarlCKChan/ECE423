/*
 * "Hello World" example.
 *
 * This example prints 'Hello from Nios II' to the STDOUT stream. It runs on
 * the Nios II 'standard', 'full_featured', 'fast', and 'low_cost' example
 * designs. It runs with or without the MicroC/OS-II RTOS and requires a STDOUT
 * device in your system's hardware.
 * The memory footprint of this hosted application is ~69 kbytes by default
 * using the standard reference design.
 *
 * For a reduced footprint version of this template, and an explanation of how
 * to reduce the memory footprint for a given application, see the
 * "small_hello_world" template.
 *
 */

#include <stdio.h>
#include "system.h"
#include "horrible_global_values.h"


volatile int button_state,button_latch;
volatile int blah = 0;

volatile int edge_capture;
int last_tested;

char files[2][20] = { "vlong.mpg","vshort.mpg" };
char index = 0;

static void timer1_ISR(void* context, alt_u32 id) {
	// acknowledge the interrupt by clearing the TO bit in the status register
	IOWR(TIMER_1_BASE, 0, 0x0);
	sendMsg = 1;

}

//Timer initalization function
void initTimer() {

	//set the timer period to 1400/50 million seconds
	int timerPeriod = 29000000;

	// initialize timer interrupt vector
	alt_irq_register(TIMER_1_IRQ, (void*) 0, timer1_ISR);

	// initialize timer period
	IOWR(TIMER_1_BASE, 2, (alt_u16)timerPeriod);
	IOWR(TIMER_1_BASE, 3, (alt_u16)(timerPeriod >> 16));

	// clear timer interrupt bit in status register
	IOWR(TIMER_1_BASE, 0, 0x0);

	// initialize timer control - start timer, run continuously, enable interrupts
	IOWR(TIMER_1_BASE, 1, 0x7);
}

#ifdef ALT_ENHANCED_INTERRUPT_API_PRESENT
static void handle_button_interrupts(void* context)
#else
static void handle_button_interrupts(void* context, alt_u32 id)
#endif
{
	/* Cast context to edge_capture's type.
	   * It is important to keep this volatile,
	   * to avoid compiler optimization issues.
	   */
	  button_state = IORD(BUTTON_PIO_BASE,3);
	  button_latch = IORD(BUTTON_PIO_BASE,0);
	  if(button_latch != 15){
		  if(button_state & 1){
			  playing = playing > 0 ? 0 : 1;
		  }else if(button_state & 2){
			  getNextFile = 1;
		  }else if(button_state & 4){
			  fastForward = 1;
		  }else if(button_state & 8){
			  backward = 1;
		  }
	  }
	  IOWR(BUTTON_PIO_BASE,3,0x0);
}

/* Initialize the button_pio. */

static void init_button_pio()
{
  /* Recast the edge_capture pointer to match the alt_irq_register() function
  * prototype. */
  alt_irq_register(BUTTON_PIO_IRQ, (void*)0, handle_button_interrupts);
  IOWR(BUTTON_PIO_BASE, 3, 0x0);
  IOWR(BUTTON_PIO_BASE, 2 ,0xF);
}

int main()
{
	  char * fileName;

	  init_button_pio();
	  initTimer();
	  send_dev = altera_avalon_mailbox_open("/dev/mailbox_0");
	  recv_dev = altera_avalon_mailbox_open("/dev/mailbox_1");
	  while( altera_avalon_mailbox_get(recv_dev, errno) );

	  alt_up_sd_card_dev *device_reference = alt_up_sd_card_open_dev(ALTERA_UP_SD_CARD_AVALON_INTERFACE_0_NAME);
	  	if(device_reference == NULL){
	  	  printf("Cannot open SD Device\n");
	  	  return -1;
	  	}
	  	if(!alt_up_sd_card_is_Present()){
	  	  printf("There is no SD card in the slot\n");
	  	  return -1;
	  	}
	  	if(!alt_up_sd_card_is_FAT16()){
	  	  printf("SD card is not of FAT16 type\n");
	  	  return -1;
	  	}


		if((Yblock = malloc(sizeof(color_block_t*) * 5))==NULL) error_and_exit("malloc failed");
		if((Crblock = malloc(sizeof(color_block_t*) * 5))==NULL) error_and_exit("malloc failed");
		if((Cbblock = malloc(sizeof(color_block_t*) * 5))==NULL) error_and_exit("malloc failed");

		if((YDCAC = malloc(sizeof(color_block_t*) * 10))==NULL) error_and_exit("malloc failed");
		if((CbDCAC = malloc(sizeof(color_block_t*) * 10))==NULL) error_and_exit("malloc failed");
		if((CrDCAC = malloc(sizeof(color_block_t*) * 10))==NULL) error_and_exit("malloc failed");

		altera_avalon_mailbox_post(send_dev, YDCAC);
		altera_avalon_mailbox_post(send_dev, CbDCAC);
		altera_avalon_mailbox_post(send_dev, CrDCAC);
		altera_avalon_mailbox_post(send_dev, Yblock);
		altera_avalon_mailbox_post(send_dev, Cbblock);
		altera_avalon_mailbox_post(send_dev, Crblock);
		altera_avalon_mailbox_post(send_dev, playing);
		altera_avalon_mailbox_post(send_dev, getNextFile);
		altera_avalon_mailbox_post(send_dev, fastForward);
		altera_avalon_mailbox_post(send_dev, backward);

		fileName = files[index];
		//returnCode = mjpeg423_decode(fileName);
		while(1){
			while( mjpeg423_decode(fileName) <= 0 ){
				index = (index + 1) % 2;
				fileName = files[index];
			}
		}


	  return 0;

  return 0;
}
