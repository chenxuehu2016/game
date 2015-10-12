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
 
// head file definition

#include <stdio.h>
#include <stdlib.h>

// type definition

#define STATUS unsigned int
#define TRUE 0x0
#define FALSE 0xffffffff

// simple macro definition

#define SINGLE_CARD_NUM 13
#define NORMAL_CARD_BOUNDARY (13 * 8)

#define SINGLE_CARD      0x1
#define DOUBLE_CARD      0x2
#define THREE_CARD       0x3
#define SAME_QUEENS      0x4
#define THREE_TWO_CARD   0x5
#define SEQUENCE         0x6
#define MORE_DOUBLE_CARD 0x7
#define MORE_THREE_CARD  0x8
#define MORE_THREE_TWO   0x9
#define BOMB_4           0xa
#define BOMB_5           0xb
#define BOMB_6           0xc
#define BOMB_7           0xd
#define BOMB_8           0xe
#define ALL_QUEENS       0xf
#define ERR_TYPE         0xffffffff

#define ASSERT(a) assert(a)
#define MAX_GROUP_NUM    0xffff

// struct definition

typedef struct _Game {

	int current_boss;

	int current_owner;

	int current_type;

	int current_num;

}Game;

static Game game[MAX_GROUP_NUM];

// check card type

static STATUS is_same_card(char var1, char var2){

	return (var1 % SINGLE_CARD_NUM) == (var2 % SINGLE_CARD_NUM) ? TRUE: FALSE; 
}

static STATUS is_normal_card(char card[], int length){

	int i;

	for(i = 0; i < length; i ++) {

		if(card[i] >= NORMAL_CARD_BOUNDARY)
			return FALSE;
	}

	return TRUE;
}

static STATUS is_there_2_card(char card[], int length) {

	int i;

	for(i = 0; i < length; i ++) {

		if(12 == card[i] % SINGLE_CARD_NUM)
			return TRUE;
	}

	return FALSE;
}

static STATUS is_single_card(char card[], int length) {

	ASSERT(card);
	ASSERT(1 == length);

	return TRUE;
}

static STATUS is_double_card(char card[], int length) {

	ASSERT(card);
	ASSERT(2 == card);
	ASSERT(TRUE == is_nomal_card(card, length));

	return is_same_card(card[0], card[1]);
}

static STATUS is_same_queens(char card[], int length) {

	ASSERT(card);
	ASSERT(2 == card);
	ASSERT(FALSE == is_nomal_card(card, length));


	if(card[0] %2 && card[1] % 2) {

		return TRUE;
	}

	if(!(card[0] % 2) && !(card[1] % 2)) {

		return TRUE;
	}

	return FALSE;
}

static STATUS is_three_card(char card[], int length) {

	ASSERT(card);
	ASSERT(3 == length);
	ASSERT(TRUE == is_nomal_card(card, length));

	if(TRUE == is_same_card(card[0], card[1]) &&
		TRUE == is_same_card(card[1], card[2])) {

		return TRUE;
	}

	return FALSE;
}

static STATUS is_bomb4(char card[], int length) {

	ASSERT(card);
	ASSERT(4 == length);
	ASSERT(TRUE == is_nomal_card(card, length));

	if(TRUE == is_same_card(card[0], card[1]) &&
		TRUE == is_same_card(card[1], card[2]) &&
		TRUE == is_same_card(card[2], card[3])) {

		return TRUE;
	}

	return FALSE;
}

static STATUS is_bomb5(char card[], int length) {

	ASSERT(card);
	ASSERT(5 == length);
	ASSERT(TRUE == is_nomal_card(card, length));

	if(TRUE == is_same_card(card[0], card[1]) &&
		TRUE == is_same_card(card[1], card[2]) &&
		TRUE == is_same_card(card[2], card[3]) &&
		TRUE == is_same_card(card[3], card[4])) {

		return TRUE;
	}

	return FALSE;
}

static STATUS is_bomb6(char card[], int length) {

	ASSERT(card);
	ASSERT(6 == length);
	ASSERT(TRUE == is_nomal_card(card, length));

	if(TRUE == is_same_card(card[0], card[1]) &&
		TRUE == is_same_card(card[1], card[2]) &&
		TRUE == is_same_card(card[2], card[3]) &&
		TRUE == is_same_card(card[3], card[4]) &&
		TRUE == is_same_card(card[4], card[5])) {

		return TRUE;
	}

	return FALSE;
}

static STATUS is_bomb7(char card[], int length) {

	ASSERT(card);
	ASSERT(7 == length);
	ASSERT(TRUE == is_nomal_card(card, length));

	if(TRUE == is_same_card(card[0], card[1]) &&
		TRUE == is_same_card(card[1], card[2]) &&
		TRUE == is_same_card(card[2], card[3]) &&
		TRUE == is_same_card(card[3], card[4]) &&
		TRUE == is_same_card(card[4], card[5]) &&
		TRUE == is_same_card(card[5], card[6])) {

		return TRUE;
	}

	return FALSE;
}

static STATUS is_bomb8(char card[], int length) {

	ASSERT(card);
	ASSERT(8 == length);
	ASSERT(TRUE == is_nomal_card(card, length));

	if(TRUE == is_same_card(card[0], card[1]) &&
		TRUE == is_same_card(card[1], card[2]) &&
		TRUE == is_same_card(card[2], card[3]) &&
		TRUE == is_same_card(card[3], card[4]) &&
		TRUE == is_same_card(card[4], card[5]) &&
		TRUE == is_same_card(card[5], card[6]) &&
		TRUE == is_same_card(card[6], card[7])) {

		return TRUE;
	}

	return FALSE;
}

static STATUS is_sequence(char card[], int length) {

	int i;

	ASSERT(card);
	ASSERT(length >= 5);
	ASSERT(TRUE == is_nomal_card(card, length));
	ASSERT(FALSE == is_there_2_card(card, length));

	for(i = 0; i < length - 1; i ++) {

		if(FALSE == is_same_card(card[i] + 1, card[i + 1]))
			return FALSE;
	}

	return TRUE;
}

static STATUS is_three_two(char card[], int length) {

	int i;

	ASSERT(card);
	ASSERT(5 == length);

	for(i = 0; i < 3; i ++) {

		if(TRUE == is_three_card(card + i, 3))
			break;
	}

	if(3 == i)
		return FALSE;

	if(0 == i) {

		is((TRUE == is_double_card(card + 3, 2) || TRUE == is_double_card(card + 3, 2)) &&
			FALSE == is_same_card(card[2], card[3])) {

			return TRUE;
		} 

		return FALSE;

	}else if(2 = = i) {

		is((TRUE == is_double_card(card, 2) || TRUE == is_double_card(card, 2)) &&
			FALSE == is_same_card(card[1], card[2])) {

			return TRUE;
		} 

		return FALSE;
	}else {
		ASSERT(0);
	}
}

static STATUS is_more_double(char card[], int length){

	int i;

	ASSERT(card);
	ASSERT(length >= 6 && 0 == length % 2);
	ASSERT(TRUE == is_nomal_card(card, length));
	ASSERT(FALSE == is_there_2_card(card, length))

	// check sequence first

	for(i = 0; i < (length >> 1) - 1; i ++) {

		if(FALSE == is_same_card(card[2 * i] + 1, card[2 * (i + 1)]))
			return FALSE;
	}

	// then check same card

	for(i = 0; i < length >> 1; i ++) {

		if(FALSE == is_double_card(card + 2 * i, 2))
			return FALSE;
	}

	return TRUE;
}

static STATUS is_more_three(char card[], int length){

	int i;

	ASSERT(card);
	ASSERT(length >= 6 && 0 == length % 3);
	ASSERT(TRUE == is_nomal_card(card, length));
	ASSERT(FALSE == is_there_2_card(card, length))

	// check sequence first

	for(i = 0; i < (length / 3) - 1; i ++) {

		if(FALSE == is_same_card(card[3 * i] + 1, card[3 * (i + 1)]))
			return FALSE;
	}

	// then check same card

	for(i = 0; i < length / 3; i ++) {

		if(FALSE == is_three_card(card + 3 * j, 3))
			return FALSE;
	}

	return TRUE;
}

static STATUS is_more_three_two(char card[], int length) {

	int i;
	int start;

	ASSERT(card);
	ASSERT(length > =10 && 0 == length % 5);

	// check three card first

	for(i = 0; i < length -2; i ++) {

		if(TRUE == is_more_three(card, length / 5))
			break;
	}

	if(length -2 == i || i % 2) {

		return FALSE;
	}

	// check same card

	start = i;
	for(i = 0; i < start;  i += 2) {

		if(FALSE == is_same_card(card[i], card[i + 1]))
			return FALSE;

		if( i + 2 < start) {

			if(TRUE == is_same_card(card[i + 1], card[i + 2]))
				return FALSE;
		}
	}

	for(i = start + 3 * (length / 5); i < length; i += 2) {

		if(FALSE == is_same_card(card[i], card[i + 1]))
			return FALSE;

		if( i + 2 < start) {

			if(TRUE == is_same_card(card[i + 1], card[i + 2]))
				return FALSE;
		}
	}

	return TRUE;
}

static STATUS is_all_queens(char card[], int length){

	ASSERT(card);
	ASSERT(4 == length);

	if(card[0] == NORMAL_CARD_BOUNDARY &&
		card[1] == NORMAL_CARD_BOUNDARY + 1 && 
		card[2] == NORMAL_CARD_BOUNDARY + 2 &&
		card[3] == NORMAL_CARD_NOUNDARY + 3) {

		return TRUE;
	}

	return FALSE;
}

// check type

static check_type(char card[], int length) {

	switch(length) {

		case 1:
			if(TRUE == is_single_card(card, length)) {

				return SINGLE_CARD;
			}

			break;

		case 2:
			if(TRUE == is_same_queens(card, length)) {

				return SAME_QUEENS;

			}else if(TRUE == is_double_card(card, length)) {

				return DOUBLE_CARD;
			}else {

				ASSERT(0);
			}

			break;

		case 3:

			if(TRUE == is_three_card(card, length)) {

				return THREE_CARD;
			}

			ASSERT(0);

			break;

		case 4:

			if(TRUE == is_all_queens(card, length)) {

				return ALL_QUEENS;

			}else if(TRUE == is_bomb4(card, length)) {

				return BOMB_4;
			}else {

				ASSERT(0);
			}

			break;

		case 5:
			if(TRUE == is_bom5(card, length)) {

				return BOMB_5;

			}else if(TRUE == is_sequence(card, length)) {

				return SEQUENCE;

			}else if(TRUE == is_three_two(card, length)) {

				return THREE_TWO_CARD;
			}else  {

				ASSERT(0);
			}

			break;

		case 6:
			if(TRUE == is_bom6(card, length)) {

				return BOMB_6;

			}else if(TRUE == is_sequence(card, length)) {

				return SEQUENCE;

			}else if(TRUE == is_more_two(card, length)) {

				return MORE_TWO_CARD;

			}else if(TRUE == is_more_three(card, length)){

				return MORE_THREE_CARD;

			} else {
				ASSERT(0);
			}

			break;

		case 7:
			if(TRUE == is_bom7(card, length)) {

				return BOMB_7;

			}else if(TRUE == is_sequence(card, length)) {

				return SEQUENCE;

			}else {
				ASSERT(0);
			}

			break;

		case 8:
			if(TRUE == is_bom8(card, length)) {

				return BOMB_8;

			}else if(TRUE == is_sequence(card, length)) {

				return SEQUENCE;

			}else if(TRUE == is_more_two(card, length)) {

				return MORE_TWO_CARD;

			}else {
				ASSERT(0);
			}

			break;

		default:
			 if(TRUE == is_sequence(card, length)) {

				return SEQUENCE;

			}else if(TRUE == is_more_two(card, length)) {

				return MORE_TWO_CARD;

			}else if(TRUE == is_more_three(card, length)){

				return MORE_THREE_CARD;

			} else if(TRUE == is_more_thre_two(card, length)){

				return MORE_THREE_TWO;

			}else {
				ASSERT(0);
			}

			break;
	}

	return TYPE_ERR;
}

// function definition

static void resort_card(char card[], int length) {

	int i;
	int j;
	int temp;
	int var1;
	int var2;

	for(i = length -1; i >= 1; i --) {
		for(j = 0; j < i; j ++) {

			var1 = card[j];
			var2 = card[j + 1];

			if(var1 > NORMAL_CARD_BOUNDARY)
				var1 %= SINGLE_CARD_NUM;

			if(var2 > NORMAL_CARD_BOUNDARY)
				var2 %= SINGLE_CARD_NUM;

			if(var1 > var2) {

				temp = card[j];
				card[j] = card[j + 1];
				card[j+1] = temp;
			}
		}
	}
}

static int get_big_data(char card[], int length, int type) {

	int i;
	int big;

	switch(type) {
	
		case SINGLE_CARD:
		case SAME_QUEENS:

			if(card[0] >= NORMAL_CARD_BOUNDARY)
				big = card[0];
			else
				big = card[0] / SINGLE_CARD_NUM;

			break;

		case THREE_TWO_CARD:

			for(i = 0; i < 3; i ++) {
			
				if(TRUE == is_three_card(card, length))
					break;
			}

			big = card[0] / SINGLE_CARD_NUM;

			break;

		case DOUBLE_CARD:
		case THREE_CARD:
		case BOMB_4:
		case BOMB_5:
		case BOMB_6:
		case BOMB_7:
		case BOMB_8:
			big = card[0] / SINGLE_CARD_NUM;

			break;

		case MORE_DOUBLE:
		case MORE_THREE:

			big = card[length -1] / SINGLE_CARD_NUM;

			break;

		case MORE_THREE_TWO:

			for(i = 0; i < length - 2; i ++) {
			
				if(TRUE == is_more_three(card, length))
					break;
			}

			big = card[i + 3 * (length / 5)] / SINGLE_CARD_NUM;
			break;

		default:
			break;
	}

}

static STATUS _process_card(char card[], int length, int group) {

	int type;

	// check type first

	type = check_type(card, length);
	if(TYPE_ERR == type) {

		return FALSE;
	}

	// check if it is bomb

	if(game[group].current_type == ALL_QUEENS) {
	
		ASSERT(0);

	}else if(game[group].current_type >= BOMB_4) {

		if(type < BOMB_4) {

			return FALSE;
		}

		return TRUE;

	}else {
	
		if(type >= BOMB_4) {

			return TRUE;

		}else {

			return FALSE;
		}
	}
}

STATUS process_card(char card[], int length, int finish, int sender, int group) {

	// all card has been sent

	if(finish) {

		return TRUE;
	}

	// user try to give up this opportunity

	if(!length) {

		return TRUE;
	}

	resort_card(card, length);
	_process_card(card, length, group);

	return TRUE;
}

// function entry

int main(int argc, char* argv[]) {

	return 0;
}
 