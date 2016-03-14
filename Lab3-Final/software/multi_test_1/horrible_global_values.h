/*
 * horrible_global_values.h
 *
 *  Created on: 2015-03-26
 *      Author: c73chan
 */

#ifndef HORRIBLE_GLOBAL_VALUES_H_
#define HORRIBLE_GLOBAL_VALUES_H_
#include "altera_avalon_mailbox.h"
#include "altera_avalon_pio_regs.h"
#include "sd_card.h"
#include "common/mjpeg423_types.h"

volatile int edge_capture;
volatile int playing;
volatile int getNextFile;
volatile int fastForward,backward;
volatile int sendMsg;
int flip;

alt_u32 sendmessage;
alt_u32 receiveMessage;
alt_mailbox_dev* send_dev;
alt_mailbox_dev* recv_dev;

dct_block_t** YDCAC;
dct_block_t** CbDCAC;
dct_block_t** CrDCAC;

color_block_t** Yblock;
color_block_t** Cbblock;
color_block_t** Crblock;
int playedOnceFlag;

#endif /* HORRIBLE_GLOBAL_VALUES_H_ */
