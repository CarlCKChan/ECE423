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
#include "altera_avalon_mailbox.h"
#include "sys/alt_irq.h"
#include "system.h"
#include "io.h"
#include "common/mjpeg423_types.h"
#include "common/util.h"
#include "altera_up_avalon_video_pixel_buffer_dma.h"
#include "altera_avalon_timer.h"
#include "altera_avalon_timer_regs.h"

volatile int *playing;
volatile int *getNextFile;
volatile int *fastForward;
volatile int *backward;

volatile int flag = 0;
int counter =0;
alt_up_pixel_buffer_dma_dev * pixel_buf_dev;

void ycbcr_to_rgb(int h, int w, uint32_t w_size, pcolor_block_t Y, pcolor_block_t Cb, pcolor_block_t Cr, rgb_pixel_t* rgbblock);

static void timer0_ISR(void* context, alt_u32 id) {
	// acknowledge the interrupt by clearing the TO bit in the status register
	IOWR(TIMER_0_BASE, 0, 0x0);
	if(pixel_buf_dev != NULL && *playing && flag){
		flag = 0;
		while(alt_up_pixel_buffer_dma_check_swap_buffers_status(pixel_buf_dev));
		alt_up_pixel_buffer_dma_swap_buffers(pixel_buf_dev);
	}

}

//Timer initalization function
void initTimer() {

	//set the timer period to 1400/50 million seconds
	int timerPeriod = 2900000;

	// initialize timer interrupt vector
	alt_irq_register(TIMER_0_IRQ, (void*) 0, timer0_ISR);

	// initialize timer period
	IOWR(TIMER_0_BASE, 2, (alt_u16)timerPeriod);
	IOWR(TIMER_0_BASE, 3, (alt_u16)(timerPeriod >> 16));

	// clear timer interrupt bit in status register
	IOWR(TIMER_0_BASE, 0, 0x0);

	// initialize timer control - start timer, run continuously, enable interrupts
	IOWR(TIMER_0_BASE, 1, 0x7);
}

int main()
{
	  alt_mailbox_dev* send_dev;
	  alt_mailbox_dev* recv_dev;
	  dct_block_t** YDCAC;
	  dct_block_t** CbDCAC;
	  dct_block_t** CrDCAC;


	  color_block_t** Yblock;
	  color_block_t** Cbblock;
	  color_block_t** Crblock;
	  alt_u32 receiveMessage = 0;
	  recv_dev = altera_avalon_mailbox_open("/dev/mailbox_0");
	  send_dev = altera_avalon_mailbox_open("/dev/mailbox_1");

	  while( altera_avalon_mailbox_get(recv_dev, errno) );
	  //file streams
	  FILE* file_in;
	  initTimer();
	  pixel_buf_dev = alt_up_pixel_buffer_dma_open_dev(VIDEO_PIXEL_BUFFER_DMA_0_NAME);
	  if ( pixel_buf_dev == NULL){
		  error_and_exit ("Error: could not open pixel buffer device \n");
	  }


	  YDCAC = altera_avalon_mailbox_pend(recv_dev);
	  CbDCAC = altera_avalon_mailbox_pend(recv_dev);
	  CrDCAC = altera_avalon_mailbox_pend(recv_dev);

	  Yblock = altera_avalon_mailbox_pend(recv_dev);
	  Cbblock = altera_avalon_mailbox_pend(recv_dev);
	  Crblock = altera_avalon_mailbox_pend(recv_dev);

	  playing = altera_avalon_mailbox_pend(recv_dev);
	  getNextFile = altera_avalon_mailbox_pend(recv_dev);
	  fastForward = altera_avalon_mailbox_pend(recv_dev);
	  backward = altera_avalon_mailbox_pend(recv_dev);

	  int h_size = altera_avalon_mailbox_pend(recv_dev);
	  int w_size = altera_avalon_mailbox_pend(recv_dev);


	  printf("YDCAC[%d] = %x\n", 0, YDCAC);

	  int hCb_size = h_size/8;           //number of chrominance blocks
	  int wCb_size = w_size/8;
	  int hYb_size = h_size/8;           //number of luminance blocks. Same as chrominance in the sample app
	  int wYb_size = w_size/8;

	  while(1){

		  receiveMessage = altera_avalon_mailbox_pend(recv_dev);
		  /*while(1){
			  if( (receiveMessage >= 0 && receiveMessage < 15 ) || receiveMessage == 100){
				  break;
			  }
			  receiveMessage = altera_avalon_mailbox_pend(recv_dev);
		  }*/
		  counter = receiveMessage;
		  for(int i = 0; i< 10; i++){
			  for(int b = 0; b < hYb_size*wYb_size; b++){
				  idct(((dct_block_t*)(YDCAC[0]))[b], ((color_block_t*)(Yblock[0]))[b]);
				  idct(((dct_block_t*)(CbDCAC[0]))[b], ((color_block_t*)(Cbblock[0]))[b]);
				  idct(((dct_block_t*)(CrDCAC[0]))[b], ((color_block_t*)(Crblock[0]))[b]);
				  ycbcr_to_rgb(b/wCb_size*8, b%wCb_size*8, w_size, ((color_block_t*)(Yblock[0]))[b], ((color_block_t*)(Cbblock[0]))[b], ((color_block_t*)(Crblock[0]))[b], (rgb_pixel_t*)(pixel_buf_dev->back_buffer_start_address));

			  }
			  /*for(int b = 0; b < hCb_size*wCb_size; b++)
				  ycbcr_to_rgb(b/wCb_size*8, b%wCb_size*8, w_size, ((color_block_t*)(Yblock[0]))[b], ((color_block_t*)(Cbblock[0]))[b], ((color_block_t*)(Crblock[0]))[b], (rgb_pixel_t*)(pixel_buf_dev->back_buffer_start_address));*/
			  flag = 1;
			  while(flag);
			  counter = (counter +1 ) % 10;
			  altera_avalon_mailbox_post(send_dev, 1);
			  //frame_counter = alt_timestamp();
			  //DEBUG_PRINT_ARG("TIMER YCBCR->RGB/DRAW FRAME: %u\n", frame_counter);
			  //counter = (counter + 1) % 10;
		  }
		  //

	  }
	  return 0;
}
