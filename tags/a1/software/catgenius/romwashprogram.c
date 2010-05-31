/******************************************************************************/
/* File    :	romwashprogram.c					      */
/* Function:	CatGenius washing program				      */
/* Author  :	Robert Delien						      */
/*		Copyright (C) 2010, Clockwork Engineering		      */
/* History :	21 Feb 2010 by R. Delien:				      */
/*		- Initial revision.					      */
/******************************************************************************/
#include <htc.h>

#include "romwashprogram.h"
#include "litterlanguage.h"
#include "../common/catgenie120.h"


/******************************************************************************/
/* Macros								      */
/******************************************************************************/


/******************************************************************************/
/* Global Data								      */
/******************************************************************************/

static unsigned char		cmdptr = 0;

static const struct command	washprogram[] = {
	{CMD_START,	CMD_LAST | 
			FLAGS_DRYRUN |
			FLAGS_WETRUN },
	{CMD_BOWL,	BOWL_CCW},	/* Scoop */
	{CMD_ARM,	ARM_DOWN},
	{CMD_WAITTIME,	13217},
	{CMD_ARM,	ARM_STOP},	/* Scoop + 1 */
	{CMD_WAITTIME,	18141},
	{CMD_BOWL,	BOWL_CW},	/* Scoop + 2 */
	{CMD_WAITTIME,	6201},
	{CMD_BOWL,	BOWL_CCW},	/* Scoop + 3 */
	{CMD_ARM,	ARM_DOWN},
	{CMD_WAITTIME,	5765},
	{CMD_ARM,	ARM_UP},	/* Scoop + 4 */
	{CMD_WAITTIME,	532},
	{CMD_ARM,	ARM_STOP},	/* Scoop + 5 */
	{CMD_WAITTIME,	25206},
	{CMD_ARM,	ARM_UP},	/* Scoop + 6 */
	{CMD_WAITTIME,	10671},
	{CMD_ARM,	ARM_DOWN},	/* Scoop + 7 */
	{CMD_WAITTIME,	6602},
	{CMD_ARM,	ARM_UP},	/* Scoop + 8 */
	{CMD_WAITTIME,	17204},
	{CMD_ARM,	ARM_DOWN},	/* Scoop + 9 */
	{CMD_WAITTIME,	12703},
	{CMD_ARM,	ARM_STOP},	/* Scoop + 10 */
	{CMD_WAITTIME,	4701},
	{CMD_ARM,	ARM_DOWN},	/* Scoop + 11 */
	{CMD_WAITTIME,	11203},
	{CMD_ARM,	ARM_UP},	/* Scoop + 12 */
	{CMD_WAITTIME,	532},
	{CMD_ARM,	ARM_STOP},	/* Scoop + 13 */
	{CMD_WAITTIME,	25206},
	{CMD_ARM,	ARM_UP},	/* Scoop + 14 */
	{CMD_WAITTIME,	10671},
	{CMD_ARM,	ARM_DOWN},	/* Scoop + 15 */
	{CMD_WAITTIME,	6601},
	{CMD_ARM,	ARM_UP},	/* Scoop + 16 */
	{CMD_WAITTIME,	20141},
	{CMD_BOWL,	BOWL_CW},	/* Scoop + 17 */
	{CMD_ARM,	ARM_DOWN},
	{CMD_WAITTIME,	21769},
	{CMD_ARM,	ARM_UP},	/* Scoop + 18 */
	{CMD_WAITTIME,	932},
	{CMD_ARM,	ARM_STOP},	/* Scoop + 19 */
	{CMD_WAITTIME,	12108},
	{CMD_BOWL,	BOWL_CCW},	/* Scoop + 20 */
	{CMD_ARM,	ARM_DOWN},
	{CMD_WAITTIME,	3264},
	{CMD_ARM,	ARM_UP},	/* Scoop + 21 */
	{CMD_WAITTIME,	532},
	{CMD_ARM,	ARM_STOP},	/* Scoop + 22 */
	{CMD_WAITTIME,	24206},
	{CMD_ARM,	ARM_UP},	/* Scoop + 23 */
	{CMD_WAITTIME,	10571},
	{CMD_ARM,	ARM_DOWN},	/* Scoop + 24 */
	{CMD_WAITTIME,	6602},
	{CMD_ARM,	ARM_UP},	/* Scoop + 25 */
	{CMD_WATER,	1},
	{CMD_WAITTIME,	17141},
	{CMD_ARM,	ARM_STOP},	/* Scoop + 26 */

	{CMD_SKIPIFWET, 3},		/* Prepare for surfacing */
	{CMD_ARM,	ARM_DOWN},
	{CMD_WAITTIME,	13217},
	{CMD_SKIPIFDRY, 153},		/* Skip washing to surfacing */

	{CMD_BOWL,	BOWL_CW},	/* Scoop + 26 */
	{CMD_WAITTIME,	18768},
	{CMD_ARM,	ARM_DOWN},	/* Wash */
	{CMD_WAITTIME,	25206},
	{CMD_ARM,	ARM_UP},	/* Wash + 1 */
	{CMD_WAITTIME,	1132},
	{CMD_ARM,	ARM_STOP},	/* Wash + 2 */
	{CMD_WAITWATER, 1},		/* Wash + 3 */
	{CMD_WATER,	0},
	{CMD_WAITTIME,	63582},		/* From full program sheet */
	{CMD_PUMP,	1},		/* Wash + 4 */
	{CMD_WAITTIME,	25206},
	{CMD_PUMP,	0},		/* Wash + 5 */
	{CMD_WAITTIME,	65535},		/* Delay split in two because it exceeds maximum */
	{CMD_WAITTIME,	10183},		/* 65535 + 10183 = 75718 */
	{CMD_PUMP,	1},		/* Wash + 6 */
	{CMD_WAITTIME,	25206},
	{CMD_PUMP,	0},		/* Wash + 7 */
	{CMD_WAITTIME,	8202},
	{CMD_PUMP,	1},		/* Wash + 8 */
	{CMD_WAITTIME,	25206},
	{CMD_PUMP,	0},		/* Wash + 9 */
	{CMD_WAITTIME,	8202},
	{CMD_PUMP,	1},		/* Wash + 10 */
	{CMD_WAITTIME,	65535},		/* Delay split in two because it exceeds maximum */
	{CMD_WAITTIME,	81},		/* 65535 + 81 = 65616 */
	{CMD_PUMP,	0},		/* Wash + 11 */
	{CMD_WATER,	1},
	{CMD_WAITTIME,	55418},	
	{CMD_BOWL,	BOWL_CCW},	/* Wash + 12 */
	{CMD_DOSAGE,	1},
	{CMD_WAITTIME,	2601},
	{CMD_DOSAGE,	0},		/* Wash + 13 */
	{CMD_BOWL,	BOWL_CW},
	{CMD_WAITWATER, 1},		/* Wash + 14 */
	{CMD_WATER,	0},
	{CMD_WAITTIME,	44002},		/* From full program sheet */
	{CMD_PUMP,	1},		/* Wash + 15 */
	{CMD_WAITTIME,	25206},
	{CMD_PUMP,	0},		/* Wash + 16 */
	{CMD_WAITTIME,	65535},		/* Delay split in two because it exceeds maximum */
	{CMD_WAITTIME,	10183},		/* 65535 + 10183 = 75718 */
	{CMD_PUMP,	1},		/* Wash + 17 */
	{CMD_WAITTIME,	24206},
	{CMD_PUMP,	0},		/* Wash + 18 */
	{CMD_WAITTIME,	8202},
	{CMD_PUMP,	1},		/* Wash + 19 */
	{CMD_WAITTIME,	24206},
	{CMD_PUMP,	0},		/* Wash + 20 */
	{CMD_WAITTIME,	8202},
	{CMD_PUMP,	1},		/* Wash + 21 */
	{CMD_WAITTIME,	65535},		/* Delay split in two because it exceeds maximum */
	{CMD_WAITTIME,	10183},		/* 65535 + 10183 = 75718 */
	{CMD_PUMP,	0},		/* Wash + 22 */
	{CMD_WATER,	1},
	{CMD_WAITTIME,	25502},
	{CMD_ARM,	ARM_DOWN},	/* Wash + 23 */
	{CMD_WAITTIME,	21205},
	{CMD_ARM,	ARM_UP},	/* Wash + 24 */
	{CMD_BOWL,	BOWL_STOP},
	{CMD_DOSAGE,	1},
	{CMD_WAITTIME,	1132},
	{CMD_ARM,	ARM_STOP},	/* Wash + 25 */
	{CMD_WAITTIME,	9580},
	{CMD_DOSAGE,	0},		/* Wash + 26 */
	{CMD_BOWL,	BOWL_CCW},
	{CMD_WAITTIME,	5329},
	{CMD_BOWL,	BOWL_CW},	/* Wash + 27 */
	{CMD_WAITTIME,	55482},
	{CMD_WAITWATER, 1},		/* Wash + 28 */
	{CMD_WATER,	0},
	{CMD_WAITTIME,	39107},
	{CMD_PUMP,	1},		/* Wash + 29 */
	{CMD_BOWL,	BOWL_CCW},
	{CMD_WAITTIME,	65535},		/* Delay split in two because it exceeds maximum */
	{CMD_WAITTIME,	113},		/* 65535 + 113 = 65648 */
	{CMD_BOWL,	BOWL_CW},	/* Wash + 30 */
	{CMD_WAITTIME,	10075},
	{CMD_BOWL,	BOWL_STOP},	/* Wash + 31 */
	{CMD_DOSAGE,	1},
	{CMD_WAITTIME,	9802},
	{CMD_BOWL,	BOWL_CW},	/* Wash + 32 */
	{CMD_DOSAGE,	0},
	{CMD_WAITTIME,	25270},
	{CMD_BOWL,	BOWL_CCW},	/* Dry */
	{CMD_DRYER,	1},
	{CMD_WAITTIME,	35372},
	{CMD_BOWL,	BOWL_CW},	/* Dry + 1 */
	{CMD_WAITTIME,	55513},
	{CMD_BOWL,	BOWL_CCW},	/* Dry + 2 */
	{CMD_WAITTIME,	35277},
	{CMD_PUMP,	0},		/* Dry + 3 */
	{CMD_WAITTIME,	1732},
	{CMD_BOWL,	BOWL_CW},	/* Dry + 4 */
	{CMD_WAITTIME,	55514},
	{CMD_BOWL,	BOWL_CCW},	/* Dry + 5 */
	{CMD_WAITTIME,	35309},
	{CMD_BOWL,	BOWL_CW},	/* Dry + 6 */
	{CMD_WAITTIME,	55514},
	{CMD_BOWL,	BOWL_CCW},	/* Dry + 7 */
	{CMD_WAITTIME,	35309},
	{CMD_BOWL,	BOWL_CW},	/* Dry + 8 */
	{CMD_WAITTIME,	45411},
	{CMD_BOWL,	BOWL_CCW},	/* Dry + 9 */
	{CMD_WAITTIME,	35404},
	{CMD_ARM,	ARM_UP},	/* Dry + 10 */
	{CMD_WAITTIME,	9980},
	{CMD_ARM,	ARM_STOP},	/* Dry + 11 */
	{CMD_BOWL,	BOWL_STOP},
	{CMD_DOSAGE,	1},
	{CMD_WAITTIME,	2601},
	{CMD_BOWL,	BOWL_CCW},	/* Dry + 12 */
	{CMD_DOSAGE,	0},
	{CMD_WAITTIME,	5392},
	{CMD_ARM,	ARM_DOWN},	/* Dry + 13 */
	{CMD_WAITTIME,	10303},
	{CMD_ARM,	ARM_STOP},	/* Dry + 14 */
	{CMD_WAITTIME,	35245},
	{CMD_BOWL,	BOWL_CW},	/* Dry + 15 */
	{CMD_WAITTIME,	45411},
	{CMD_BOWL,	BOWL_CCW},	/* Dry + 16 */
	{CMD_WAITTIME,	35309},
	{CMD_BOWL,	BOWL_CW},	/* Dry + 17 */
	{CMD_WAITTIME,	45411},
	{CMD_BOWL,	BOWL_CCW},	/* Dry + 18 */
	{CMD_WAITTIME,	35404},
	{CMD_ARM,	ARM_UP},	/* Dry + 19 */
	{CMD_WAITTIME,	10380},
	{CMD_ARM,	ARM_STOP},	/* Dry + 20 */
	{CMD_BOWL,	BOWL_STOP},
	{CMD_DOSAGE,	1},
	{CMD_WAITTIME,	2601},
	{CMD_BOWL,	BOWL_CCW},	/* Dry + 21 */
	{CMD_DOSAGE,	0},
	{CMD_WAITTIME,	4392},
	{CMD_ARM,	ARM_DOWN},	/* Dry + 22 */
	{CMD_WAITTIME,	10803},
	{CMD_ARM,	ARM_STOP},	/* Dry + 23 */
	{CMD_WAITTIME,	35245},
	{CMD_BOWL,	BOWL_CW},	/* Dry + 24 */
	{CMD_WAITTIME,	35309},
	{CMD_BOWL,	BOWL_CCW},	/* Dry + 25 */
	{CMD_WAITTIME,	35309},
	{CMD_BOWL,	BOWL_CW},	/* Dry + 26 */
	{CMD_WAITTIME,	35308},
	{CMD_BOWL,	BOWL_CCW},	/* Dry + 27 */
	{CMD_WAITTIME,	35404},
	{CMD_ARM,	ARM_UP},	/* Dry + 28 */
	{CMD_WAITTIME,	11503},
	{CMD_ARM,	ARM_STOP},	/* Dry + 29 */
	{CMD_WAITTIME,	2169},
	{CMD_ARM,	ARM_DOWN},	/* Dry + 30 */
	{CMD_WAITTIME,	11703},

	{CMD_ARM,	ARM_STOP},	/* Surface + 31 */
	{CMD_WAITTIME,	45347},
	{CMD_BOWL,	BOWL_CW},	/* Dry + 32 */
	{CMD_WAITTIME,	35309},
	{CMD_BOWL,	BOWL_CCW},	/* Dry + 33 */
	{CMD_WAITTIME,	45411},
	{CMD_BOWL,	BOWL_CW},	/* Dry + 34 */
	{CMD_WAITTIME,	35309},
	{CMD_BOWL,	BOWL_CCW},	/* Dry + 35 */
	{CMD_WAITTIME,	45411},
	{CMD_BOWL,	BOWL_CW},	/* Dry + 36 */
	{CMD_WAITTIME,	35308},
	{CMD_BOWL,	BOWL_CCW},	/* Dry + 37 */
	{CMD_WAITTIME,	45411},
	{CMD_BOWL,	BOWL_CW},	/* Dry + 38 */
	{CMD_WAITTIME,	35404},
	{CMD_ARM,	ARM_UP},	/* Dry + 39 */
	{CMD_WAITTIME,	300},
	{CMD_ARM,	ARM_STOP},	/* Dry + 40 */
	{CMD_WAITTIME,	12203},
	{CMD_ARM,	ARM_UP},	/* Dry + 41 */
	{CMD_WAITTIME,	300},
	{CMD_ARM,	ARM_STOP},	/* Dry + 42 */
	{CMD_WAITTIME,	12203},
	{CMD_ARM,	ARM_UP},	/* Dry + 43 */
	{CMD_WAITTIME,	300},
	{CMD_ARM,	ARM_STOP},	/* Dry + 44 */
	{CMD_WAITTIME,	10203},
	{CMD_ARM,	ARM_UP},	/* Dry + 45 */
	{CMD_WAITTIME,	300},
	{CMD_ARM,	ARM_STOP},	/* Dry + 46 */
	{CMD_WAITTIME,	10202},
	{CMD_ARM,	ARM_UP},	/* Dry + 47 */
	{CMD_WAITTIME,	9170},
	{CMD_ARM,	ARM_DOWN},	/* Dry + 48 */
	{CMD_WAITTIME,	6474},
	{CMD_ARM,	ARM_UP},	/* Dry + 49 */
	{CMD_WAITTIME,	18268},
	{CMD_ARM,	ARM_DOWN},	/* Dry + 50 */
	{CMD_BOWL,	BOWL_STOP},
	{CMD_WAITTIME,	2832},
	{CMD_ARM,	ARM_STOP},	/* Dry + 51 */
	{CMD_END,	0}
};


/******************************************************************************/
/* Local Prototypes							      */
/******************************************************************************/


/******************************************************************************/
/* Global Implementations						      */
/******************************************************************************/

void romwashprogram_getcmd (unsigned char cmd_pointer)
{
	cmdptr = cmd_pointer;
}

unsigned char romwashprogram_gotcmd (struct command *command)
{
	command->cmd = washprogram[cmdptr].cmd;
	command->arg = washprogram[cmdptr].arg;

	return 1;
}


/******************************************************************************/
/* Local Implementations						      */
/******************************************************************************/