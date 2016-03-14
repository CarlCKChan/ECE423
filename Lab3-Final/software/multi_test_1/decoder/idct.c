//
//  idct.c
//  mjpeg423app
//
//  Created by Rodolfo Pellizzoni on 12/28/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include <stdio.h>
#include "mjpeg423_decoder.h"
#include "../common/dct_math.h"
#include "../common/util.h"
#include "system.h"
#include "sys/alt_cache.h"
#ifndef NULL_DCT

/*
* This implementation is based on an algorithm described in
*   C. Loeffler, A. Ligtenberg and G. Moschytz, "Practical Fast 1-D DCT
*   Algorithms with 11 Multiplications", Proc. Int'l. Conf. on Acoustics,
*   Speech, and Signal Processing 1989 (ICASSP '89), pp. 988-991.
* The primary algorithm described there uses 11 multiplies and 29 adds.
* We use their alternate method with 12 multiplies and 32 adds.
* The advantage is that no data path contains more than one multiplication.
*/

/* normalize the result between 0 and 255 */
/* this is required to handle precision errors that might cause the decoded result to fall out of range */
#define NORMALIZE(x) (temp = (x), ( (temp < 0) ? 0 : ( (temp > 255) ? 255 : temp  ) ) )

void idct(pdct_block_t DCAC, pcolor_block_t block)
{
	 uint32_t* input = (uint32_t*)(DCAC);
	 int scratchpad_input1,scratchpad_input2;
	 int scratchpad_output;

	 scratchpad_input1 = input[0];
	 scratchpad_input2 = input[1];
	 scratchpad_output= ALT_CI_2D_INTERFACE25_INST(0,scratchpad_input1,scratchpad_input2);
	 //printf("1 %d\n",scratchpad_input1);
	 //printf("2 %d\n",scratchpad_input2);

	 scratchpad_input1 = input[2];
	 scratchpad_input2 = input[3];
	 scratchpad_output= ALT_CI_2D_INTERFACE25_INST( 0x10 ,scratchpad_input1,scratchpad_input2);
	 //printf("1 %d\n",scratchpad_input1);
	 //printf("2 %d\n",scratchpad_input2);

	 scratchpad_input1 = input[4];
	 scratchpad_input2 = input[5];
	 scratchpad_output= ALT_CI_2D_INTERFACE25_INST( 0x2 ,scratchpad_input1,scratchpad_input2);
	 //printf("1 %d\n",scratchpad_input1);
	 //printf("2 %d\n",scratchpad_input2);

	 scratchpad_input1 = input[6];
	 scratchpad_input2 = input[7];
	 scratchpad_output= ALT_CI_2D_INTERFACE25_INST(0x12,scratchpad_input1,scratchpad_input2);
	 //printf("1 %d\n",scratchpad_input1);
	 //printf("2 %d\n",scratchpad_input2);

	 scratchpad_input1 = input[8];
	 scratchpad_input2 = input[9];
	 scratchpad_output= ALT_CI_2D_INTERFACE25_INST(0x4,scratchpad_input1,scratchpad_input2);
	 //printf("1 %d\n",scratchpad_input1);
	 //printf("2 %d\n",scratchpad_input2);

	 scratchpad_input1 = input[10];
	 scratchpad_input2 = input[11];
	 scratchpad_output= ALT_CI_2D_INTERFACE25_INST(0x14,scratchpad_input1,scratchpad_input2);
	 //printf("1 %d\n",scratchpad_input1);
	 //printf("2 %d\n",scratchpad_input2);
	 ////printf("%d\n",scratchpad_output);

	 scratchpad_input1 = input[12];
	 scratchpad_input2 = input[13];
	 scratchpad_output= ALT_CI_2D_INTERFACE25_INST(0x6,scratchpad_input1,scratchpad_input2);
	 //printf("1 %d\n",scratchpad_input1);
	 //printf("2 %d\n",scratchpad_input2);
	 ////printf("%d\n",scratchpad_output);

	 scratchpad_input1 = input[14];
	 scratchpad_input2 = input[15];
	 scratchpad_output= ALT_CI_2D_INTERFACE25_INST(0x16,scratchpad_input1,scratchpad_input2);
	 //printf("1 %d\n",scratchpad_input1);
	 //printf("2 %d\n",scratchpad_input2);

	 scratchpad_input1 = input[16];
	 scratchpad_input2 = input[17];
	 scratchpad_output= ALT_CI_2D_INTERFACE25_INST(0x08,scratchpad_input1,scratchpad_input2);
	 //printf("1 %d\n",scratchpad_input1);
	 //printf("2 %d\n",scratchpad_input2);

	 scratchpad_input1 = input[18];
	 scratchpad_input2 = input[19];
	 scratchpad_output= ALT_CI_2D_INTERFACE25_INST(0x18,scratchpad_input1,scratchpad_input2);
	 //printf("1 %d\n",scratchpad_input1);
	 //printf("2 %d\n",scratchpad_input2);

	 scratchpad_input1 = input[20];
	 scratchpad_input2 = input[21];
	 scratchpad_output= ALT_CI_2D_INTERFACE25_INST(0x0a,scratchpad_input1,scratchpad_input2);
	 //printf("1 %d\n",scratchpad_input1);
	 //printf("2 %d\n",scratchpad_input2);

	 scratchpad_input1 = input[22];
	 scratchpad_input2 = input[23];
	 scratchpad_output= ALT_CI_2D_INTERFACE25_INST(0x1a,scratchpad_input1,scratchpad_input2);
	 //printf("1 %d\n",scratchpad_input1);
	 //printf("2 %d\n",scratchpad_input2);

	 scratchpad_input1 = input[24];
	 scratchpad_input2 = input[25];
	 scratchpad_output= ALT_CI_2D_INTERFACE25_INST(0x0c,scratchpad_input1,scratchpad_input2);
	 //printf("1 %d\n",scratchpad_input1);
	 //printf("2 %d\n",scratchpad_input2);

	 scratchpad_input1 = input[26];
	 scratchpad_input2 = input[27];
	 scratchpad_output= ALT_CI_2D_INTERFACE25_INST(0x1c,scratchpad_input1,scratchpad_input2);
	 //printf("1 %d\n",scratchpad_input1);
	 //printf("2 %d\n",scratchpad_input2);

	 scratchpad_input1 = input[28];
	 scratchpad_input2 = input[29];
	 scratchpad_output= ALT_CI_2D_INTERFACE25_INST(0x0e,scratchpad_input1,scratchpad_input2);
	 //printf("1 %d\n",scratchpad_input1);
	 //printf("2 %d\n",scratchpad_input2);

	 scratchpad_input1 = input[30];
	 scratchpad_input2 = input[31];
	 scratchpad_output= ALT_CI_2D_INTERFACE25_INST(0x1e,scratchpad_input1,scratchpad_input2);
	 //printf("1 %d\n",scratchpad_input1);
	 //printf("2 %d\n",scratchpad_input2);

	 ALT_CI_2D_INTERFACE25_INST(0x1e,scratchpad_input1,scratchpad_input2);
	 ////printf("1 %d\n",scratchpad_input1);
	 ////printf("2 %d\n",scratchpad_input2);

	 scratchpad_output = ALT_CI_2D_INTERFACE25_INST(0x01,0,0);
	 block[0][0] = scratchpad_output >> 16;
	 block[1][0] = scratchpad_output & 0xffff;
	 //printf("**** %d\n",scratchpad_output);

	 scratchpad_output = ALT_CI_2D_INTERFACE25_INST(0x11,0,0);
	 block[2][0] = scratchpad_output >> 16;
	 block[3][0] = scratchpad_output & 0xffff;
	 //printf("**** %d\n",scratchpad_output);

	 scratchpad_output = ALT_CI_2D_INTERFACE25_INST(0x21,0,0);
	 block[4][0] = scratchpad_output >> 16;
	 block[5][0] = scratchpad_output & 0xffff;
	 //printf("**** %d\n",scratchpad_output);

	 scratchpad_output = ALT_CI_2D_INTERFACE25_INST(0x31,0,0);
	 block[6][0] = scratchpad_output >> 16;
	 block[7][0] = scratchpad_output & 0xffff;
	 //printf("**** %d\n",scratchpad_output);

	 scratchpad_output = ALT_CI_2D_INTERFACE25_INST(0x03,0,0);
	 block[0][1] = scratchpad_output >> 16;
	 block[1][1] = scratchpad_output & 0xffff;
	 //printf("**** %d\n",scratchpad_output);

	 scratchpad_output = ALT_CI_2D_INTERFACE25_INST(0x13,0,0);
	 block[2][1] = scratchpad_output >> 16;
	 block[3][1] = scratchpad_output & 0xffff;
	 //printf("**** %d\n",scratchpad_output);

	 scratchpad_output = ALT_CI_2D_INTERFACE25_INST(0x23,0,0);
	 block[4][1] = scratchpad_output >> 16;
	 block[5][1] = scratchpad_output & 0xffff;
	 //printf("**** %d\n",scratchpad_output);

	 scratchpad_output = ALT_CI_2D_INTERFACE25_INST(0x33,0,0);
	 block[6][1] = scratchpad_output >> 16;
	 block[7][1] = scratchpad_output & 0xffff;
	 //printf("**** %d\n",scratchpad_output);

	 scratchpad_output = ALT_CI_2D_INTERFACE25_INST(0x05,0,0);
	 block[0][2] = scratchpad_output >> 16;
	 block[1][2] = scratchpad_output & 0xffff;
	 //printf("**** %d\n",scratchpad_output);

	 scratchpad_output = ALT_CI_2D_INTERFACE25_INST(0x15,0,0);
	 block[2][2] = scratchpad_output >> 16;
	 block[3][2] = scratchpad_output & 0xffff;
	 //printf("**** %d\n",scratchpad_output);

	 scratchpad_output = ALT_CI_2D_INTERFACE25_INST(0x25,0,0);
	 block[4][2] = scratchpad_output >> 16;
	 block[5][2] = scratchpad_output & 0xffff;
	 //printf("**** %d\n",scratchpad_output);

	 scratchpad_output = ALT_CI_2D_INTERFACE25_INST(0x35,0,0);
	 block[6][2] = scratchpad_output >> 16;
	 block[7][2] = scratchpad_output & 0xffff;
	 //printf("**** %d\n",scratchpad_output);

	 scratchpad_output = ALT_CI_2D_INTERFACE25_INST(0x07,0,0);
	 block[0][3] = scratchpad_output >> 16;
	 block[1][3] = scratchpad_output & 0xffff;
	 //printf("**** %d\n",scratchpad_output);

	 scratchpad_output = ALT_CI_2D_INTERFACE25_INST(0x17,0,0);
	 block[2][3] = scratchpad_output >> 16;
	 block[3][3] = scratchpad_output & 0xffff;
	 //printf("**** %d\n",scratchpad_output);

	 scratchpad_output = ALT_CI_2D_INTERFACE25_INST(0x27,0,0);
	 block[4][3] = scratchpad_output >> 16;
	 block[5][3] = scratchpad_output & 0xffff;
	 //printf("**** %d\n",scratchpad_output);

	 scratchpad_output = ALT_CI_2D_INTERFACE25_INST(0x37,0,0);
	 block[6][3] = scratchpad_output >> 16;
	 block[7][3] = scratchpad_output & 0xffff;
	 //printf("**** %d\n",scratchpad_output);

	 scratchpad_output = ALT_CI_2D_INTERFACE25_INST(0x09,0,0);
	 block[0][4] = scratchpad_output >> 16;
	 block[1][4] = scratchpad_output & 0xffff;
	 //printf("**** %d\n",scratchpad_output);

	 scratchpad_output = ALT_CI_2D_INTERFACE25_INST(0x19,0,0);
	 block[2][4] = scratchpad_output >> 16;
	 block[3][4] = scratchpad_output & 0xffff;
	 //printf("**** %d\n",scratchpad_output);

	 scratchpad_output = ALT_CI_2D_INTERFACE25_INST(0x29,0,0);
	 block[4][4] = scratchpad_output >> 16;
	 block[5][4] = scratchpad_output & 0xffff;
	 //printf("**** %d\n",scratchpad_output);

	 scratchpad_output = ALT_CI_2D_INTERFACE25_INST(0x39,0,0);
	 block[6][4] = scratchpad_output >> 16;
	 block[7][4] = scratchpad_output & 0xffff;
	 //printf("**** %d\n",scratchpad_output);

	 scratchpad_output = ALT_CI_2D_INTERFACE25_INST(0x0b,0,0);
	 block[0][5] = scratchpad_output >> 16;
	 block[1][5] = scratchpad_output & 0xffff;
	 //printf("**** %d\n",scratchpad_output);

	 scratchpad_output = ALT_CI_2D_INTERFACE25_INST(0x1b,0,0);
	 block[2][5] = scratchpad_output >> 16;
	 block[3][5] = scratchpad_output & 0xffff;
	 //printf("**** %d\n",scratchpad_output);

	 scratchpad_output = ALT_CI_2D_INTERFACE25_INST(0x2b,0,0);
	 block[4][5] = scratchpad_output >> 16;
	 block[5][5] = scratchpad_output & 0xffff;
	 //printf("**** %d\n",scratchpad_output);

	 scratchpad_output = ALT_CI_2D_INTERFACE25_INST(0x3b,0,0);
	 block[6][5] = scratchpad_output >> 16;
	 block[7][5] = scratchpad_output & 0xffff;
	 //printf("**** %d\n",scratchpad_output);

	 scratchpad_output = ALT_CI_2D_INTERFACE25_INST(0x0d,0,0);
	 block[0][6] = scratchpad_output >> 16;
	 block[1][6] = scratchpad_output & 0xffff;
	 //printf("**** %d\n",scratchpad_output);

	 scratchpad_output = ALT_CI_2D_INTERFACE25_INST(0x1d,0,0);
	 block[2][6] = scratchpad_output >> 16;
	 block[3][6] = scratchpad_output & 0xffff;
	 //printf("**** %d\n",scratchpad_output);

	 scratchpad_output = ALT_CI_2D_INTERFACE25_INST(0x2d,0,0);
	 block[4][6] = scratchpad_output >> 16;
	 block[5][6] = scratchpad_output & 0xffff;
	 //printf("**** %d\n",scratchpad_output);

	 scratchpad_output = ALT_CI_2D_INTERFACE25_INST(0x3d,0,0);
	 block[6][6] = scratchpad_output >> 16;
	 block[7][6] = scratchpad_output & 0xffff;
	 //printf("**** %d\n",scratchpad_output);

	 scratchpad_output = ALT_CI_2D_INTERFACE25_INST(0x0f,0,0);
	 block[0][7] = scratchpad_output >> 16;
	 block[1][7] = scratchpad_output & 0xffff;
	 //printf("**** %d\n",scratchpad_output);

	 scratchpad_output = ALT_CI_2D_INTERFACE25_INST(0x1f,0,0);
	 block[2][7] = scratchpad_output >> 16;
	 block[3][7] = scratchpad_output & 0xffff;
	 //printf("**** %d\n",scratchpad_output);

	 scratchpad_output = ALT_CI_2D_INTERFACE25_INST(0x2f,0,0);
	 block[4][7] = scratchpad_output >> 16;
	 block[5][7] = scratchpad_output & 0xffff;
	 //printf("**** %d\n",scratchpad_output);

	 scratchpad_output = ALT_CI_2D_INTERFACE25_INST(0x3f,0,0);
	 block[6][7] = scratchpad_output >> 16;
	 block[7][7] = scratchpad_output & 0xffff;
	 //printf("**** %d\n",scratchpad_output);

}

#else /* Null implementation */

void idct(pdct_block_t DCAC, pcolor_block_t block)
{
    for(int row = 0 ; row < 8; row ++)
        for(int column = 0; column < 8; column++)
            block[row][column] = DCAC[row][column];
}

#endif

