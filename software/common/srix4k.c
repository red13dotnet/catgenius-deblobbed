/******************************************************************************/
/* File    :	srix4k.c						      */
/* Function:	srix4k RFID tag functional implementation		      */
/* Author  :	Robert Delien						      */
/*		Copyright (C) 2010, Clockwork Engineering		      */
/* History :	7 Mar 2010 by R. Delien:				      */
/*		- Initial revision.					      */
/******************************************************************************/
#include <htc.h>
#include <stdio.h>

#include "srix4k.h"
#include "hardware.h"
#include "cr14.h"

#include "timer.h"
#include "serial.h"

/******************************************************************************/
/* Macros								      */
/******************************************************************************/

#define IDLE		0
#define CARRIEROFF	1
#define CARRIERON	7
#define READUID
#define READBLOCK	12
#define WRITEBLOCK	22


/******************************************************************************/
/* Global Data								      */
/******************************************************************************/

static struct timer	timer	= EXPIRED;
static unsigned char	state	= 0;
static unsigned long	cid[2]	= {0, 0};

//static unsigned char	block	= 0;

/******************************************************************************/
/* Local Prototypes							      */
/******************************************************************************/


/******************************************************************************/
/* Global Implementations						      */
/******************************************************************************/

void srix4k_init (void)
/******************************************************************************/
/* Function:	Module initialisation routine				      */
/*		- Initializes the module				      */
/* History :	5 Mar 2010 by R. Delien:				      */
/*		- Initial revision.					      */
/******************************************************************************/
{
}
/* End: srix4k_init */


void srix4k_work (void)
/******************************************************************************/
/* Function:	Module worker routine					      */
/*		-       */
/* History :	5 Mar 2010 by R. Delien:				      */
/*		- Initial revision.					      */
/******************************************************************************/
{
	unsigned char	error = 0;
	unsigned char	frame[4];
	unsigned char	length;

	switch (state) {
	case IDLE:
		if (timeoutexpired(&timer))
			state++;
		break;
	case 1:
		/* Turn on the carrier */
		if (cr14_writeparamreg(0x10)) {
			error = 1;
			goto out;
		}
		state++;
		break;
	case 2:
		/* Send an Init frame */
		frame[0] = 0x06;
		frame[1] = 0x00;
		if (cr14_writeframe(frame, 2)) {
			error = 2;
			goto off;
		}
		state++;
		break;
	case 3:
		/* Read the assigned node ID */
		length = sizeof(frame);
		if (cr14_readframe(frame, &length)) {
			error = 3;
			goto off;
		}
		state++;
		break;
	case 4:
		/* Select the node */
		frame[1] = frame[0];
		frame[0] = 0x0E;
		if (cr14_writeframe(frame, 2)) {
			error = 5;
			goto off;
		}
		state++;
		break;
	case 5:
		/* Read the response */
		length = sizeof(frame);
		if (cr14_readframe(frame, &length)) {
			error = 6;
			goto off;
		}
		state++;
		break;
	case 6:
		/* Request the unique ID */
		frame[0] = 0x0B;
		if (cr14_writeframe(frame, 1)) {
			error = 8;
			goto off;
		}
		state++;
		break;
	case 7:
		/* Read the requested ID */
		length = sizeof(cid);
		if (cr14_readframe((unsigned char *)cid, &length)) {
			error = 9;
			goto off;
		}
		if (length != 8) {
			error = 10;
			goto off;
		}
//		printf("ID: %d 0x%.8lX%.8lX\n", length, cid[1], cid[0]);
		state++;
state = 14;
#if 0
	case 8:
		/* Get the unique ID */
		frame[0] = 0x08;
		frame[1] = block;
		cr14_writeframe(frame, 2);
		state++;
		break;
	case 9:
		/* Read the response */
		length = 8;
		cr14_readframe(frame, &length);
		putst("Block ");
		putchhex(block);
		putch(' ');
		putch(' ');
		putchhex(frame[3]);
		putch(' ');
		putchhex(frame[2]);
		putch(' ');
		putchhex(frame[1]);
		putch(' ');
		putchhex(frame[0]);
		putch(' ');
		putch('\n');
		block++;
		if (block > 127)
			block = 255;
		if (block)
			state = 8;
		else
			state++;
		break;
#endif
	default:
		goto off;
	}

	return;

off:
	/* Turn off the carrier */
	cr14_writeparamreg(0x00);
out:
	/* Reset state machine */
	state = IDLE;
	/* Schedule next time */
	settimeout(&timer, 3*SECOND);
	if (error)
		printf("Err %d\n", error);
	return;
} /* srix4k_work */


void srix4k_term (void)
/******************************************************************************/
/* Function:	Module termination routine				      */
/*		- Terminates the module					      */
/* History :	5 Mar 2010 by R. Delien:				      */
/*		- Initial revision.					      */
/******************************************************************************/
{
}
/* End: srix4k_term */


/******************************************************************************/
/* Local Implementations						      */
/******************************************************************************/