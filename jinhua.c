/*
 * Copyright 2015 tristan_fei <feixiaoxing@163.com>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// game struct

#define MAX_GROUP 0xffff

typedef struct _Game{

	// current holding card number
	int current_card_num;

	// current game state
	int current_game_state;

	// current card owner
	int current_owner;

	// current card type
	int current_card_type;

	// current landlord
	int current_boss;

	// record how many people have set bet
	int current_cnt;

	// current bet value
	int current_bet;

	// currnet big data for specified type
	int current_big;

	// current user id
	int current_user[3];

	// current seed for group
	int current_seed;

	// current round card information
	int current_card_info[54];

}Game;

static Game game[MAX_GROUP];

// data type

#define u32 unsigned int
#define s32 int
#define u16 unsigned short
#define s16 short
#define u8 unsigned char
#define s8 char

#define STATUS unsigned int
#define TRUE  0x0
#define FALSE 0xffffffff

// other macro

#define ASSERT(a) assert(a)
#define SINGLE_CARD_NUM 13

// type definition

#define BOSS            0x1
#define SEQ_SAME_COLOR  0x2
#define SAME_COLOR      0x3
#define SEQ             0x4
#define DOUBLE_CARD     0x5
#define NORMAL_CARD     0x6

// type check

static STATUS is_same_card(char var1, char var2);

static int get_big_data(char card[], int length, int type) {

	switch(type) {

		case BOSS:
		case SEQ_SAME_COLOR:
		case SAME_COLOR:
		case SEQ:
		case NORMAL_CARD:
			return card[2];


		case DOUBLE_CARD:

			if(TRUE == is_same_card(card[0], card[1])) {

				return card[0];

			}else if(TRUE == is_same_card(card[1], card[2])) {

				return card[1];
			}else {

				return card[0];
			}

	}
}

static STATUS is_same_card(char var1, char var2) {

	return (var1 % SINGLE_CARD_NUM == var2 % SINGLE_CARD_NUM) ? TRUE : FALSE;
}

STATUS is_boss(char card[], int length) {

	ASSERT(card);

	if(TRUE == is_same_card(card[0], card[1]) &&
		TRUE == is_same_card(card[1], card[2])) {

		return TRUE;
	}

	return FALSE;
}

STATUS is_seq(char card[], int length) {

	int i;

	ASSERT(card);

	for(i = 0; i < 2;  i ++) {

		// card can not be 12 1 2 this type

		if(card[i] % SINGLE_CARD_NUM >= card[i + 1] % SINGLE_CARD_NUM) {

			return FALSE;
		}


		// check sequence
		if(FALSE == is_same_card(card[i] + 1, card[i + 1])) {

			return FALSE;
		}
	}

	return TRUE;	
}

STATUS is_seq_same(char card[], int length) {

	if(FALSE == is_seq(card, length)) {

		return FALSE;
	}

	if(card[0] / SINGLE_CARD_NUM == card[1] / SINGLE_CARD_NUM &&
		card[0]/ SINGLE_CARD_NUM == card[1] / SINGLE_CARD_NUM) {

		return TRUE;
	}

	return FALSE;
}

STATUS is_double_card(char card[], int length) {

	ASSERT(card);

	if(TRUE == is_boss(card, length)) {

		return FALSE;
	}

	if(TRUE == is_same_card(card[0], card[1]) ||
		TRUE == is_same_card(card[1], card[2]) ||
		TRUE == is_same_card(card[0], card[2])) {

		return TRUE;
	}

	return FALSE;
}

STATUS is_normal_card(char card[], int length) {

	ASSERT(card);

	if(TRUE == is_boss(card, length)) {

		return FALSE;
	}

	if(TRUE == is_seq(card, length)) {

		return FALSE;
	}

	if(TRUE == is_double_card(card, length)) {

		return FALSE;
	}

	return TRUE;
}

// process card

static void _process_card(char card[], int length, int group) {

	return;
}

void process_card(char card[], int length, int group, int sender) {

	_process_card(card, length, group);
}

// function entry

int main(int argc, char* argv[]) {

	// get event and process event

	while(1) {

		// process event
	}

	return 0;
}


