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
#include <string.h>
#include <memory.h>

// check card type

#define SINGLE_CARD    0x1
#define DOUBLE_CARD    0x2
#define BOMB           0x3
#define SEQUENCE       0x4
#define DOUBLE_QUEENS  0x5
#define THREE_CARD     0x6
#define THREE_ONE      0x7
#define THREE_TWO      0x8
#define FOUR_TWO_DIFF  0x9
#define FOUR_TWO_SAME  0xa
#define MORE_DOUBLE    0xb
#define MORE_THREE     0xc
#define MORE_THREE_ONE 0xd
#define MORE_THREE_TWO 0xe
#define TYPE_ERR       0xffffffff

#define SMALL_QUEEN     52
#define BIG_QUEEN       53
#define SINGLE_CARD_NUM 13


// data type definition

#define ASSERT(a)
#define STATUS unsigned int
#define TRUE 0
#define FALSE ~0x0

#define MAX_GROUP  0xffff

// function declaration

static STATUS is_three_cards(char card[], int length);
static STATUS is_bomb(char card[], int length);

// game struct

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

// define game state

#define INIT_STATE   0x0
#define SELECT_STATE 0x1
#define GAME_STATE   0x2
#define RUN_STATE    0x3

// has 2 card

static STATUS is_there_2_card(char card[], int length){

	int i;

	ASSERT(card);
	ASSERT(length);

	for(i = 0; i < length; i ++) {
		if(12 == (card[i] % SINGLE_CARD_NUM))
			return TRUE;
	}

	return FALSE;
}

// has queen card

static STATUS is_there_queen_card(char card[], int length) {

	int i;

	ASSERT(card);
	ASSERT(length);

	for(i = 0; i < length; i ++) {
		if(SMALL_QUEEN == card[i] || BIG_QUEEN == card[i])
			return TRUE;
	}

	return FALSE;
}

// select boss

STATUS select_boss(int bet, int sender, int group) {

	STATUS ret = FALSE;

	switch(bet) {

		case 0:
			break;

		case 1:
		case 2:
			if(bet > game[group].current_bet) {

				game[group].current_boss = sender;
				game[group].current_bet = bet;
			}
			break;

		case 3:
			game[group].current_boss = sender;
			game[group].current_bet = 3;
			ret = TRUE;
			break;

		default:
			ASSERT(0);

	}

	// increase user count

	game[group].current_cnt ++;
	if(game[group].current_cnt == 3) {

		ret = TRUE;
	}

	return ret;
}

// deal card first

void deal_card(char card[], int length, int group) {

	int i;
	int j;
	int tmp;

	ASSERT(card);
	ASSERT(54 == length);

	// init card

	for(i =0 ; i < length; i ++) {
		card[i] = i;
	}

	// resort card

	srand(game[group].current_seed);

	for(i = 0; i < length; i ++) {

		j = rand() % 54;
		if(i != j) {

			tmp = card[i];
			card[i] = card[j];
			card[j] = tmp;
		}
	}

	game[group].current_seed = rand();
	
}


// mapping method

// for send

// 3 - 0, 4 - 1, 5 - 2, 6 - 3, 7 - 4, 8 - 5
// 9 - 6, 10 -7, j - 8, p - 9, k - 10, A - 11, 2 - 12

// for recv

// 0 - 3, 1 - 4, 2 - 5, 3 - 6, 4 - 7, 5 - 8
// 6 - 9, 7 - 10, 8 - j, 9 - p, 10 - k, 11 - A, 12 - 2

// check cheat function

static STATUS check_cheat(char card[], int length, int group) {

	int i;

	for(i = 0; i < length; i ++) {

		if(game[group].current_card_info[card[i]] == 0)
			return FALSE;

		game[group].current_card_info[card[i]] = 0;
	}

	return TRUE;
}

// shuffle cards by real value

void resort_cards(char card[], int length) {

	int i;
	int j;
	char tmp;
	char var1;
	char var2;

	for(i = length -1; i >= 1; i --) {
		for(j = 0; j < i; j ++) {

			var1 = card[j];
			var2 = card[j + 1];

			if(var1 != SMALL_QUEEN && var1 != BIG_QUEEN)
				var1 = var1 % SINGLE_CARD_NUM;

			if(var2 != SMALL_QUEEN && var2 != BIG_QUEEN)
				var2 = var2 % SINGLE_CARD_NUM;

			if(var1 > var2) {

				tmp = card[j];
				card[j] = card[j +1];
				card[j + 1] = tmp;
			}
		}
	}
}

// get big data

int get_big_data(char card[], int length, int type) {

	int i;

	ASSERT(card);
	ASSERT(length);

	switch(type) {

		case SINGLE_CARD:
		{
			if(card[0] == SMALL_QUEEN || card[0] == BIG_QUEEN)
				return card[0];
			else
				return card[0] % SINGLE_CARD_NUM;
		}

		case DOUBLE_CARD:
		case THREE_CARD:
		case BOMB:
		{
			return card[0] % SINGLE_CARD_NUM;
		}

		case SEQUENCE:
		case MORE_DOUBLE:
		case MORE_THREE:
		{
			return card[length -1] % SINGLE_CARD_NUM;
		}

		case FOUR_TWO_DIFF:
		case FOUR_TWO_SAME:
		{
			for(i = 0; i < length -3; i ++){
				if(TRUE == is_bomb(card +i, 4))
					break;
			}

			return card[i] % SINGLE_CARD_NUM;
		}

		case THREE_ONE:
		case THREE_TWO:
		{
			for(i = 0; i < length -2; i ++){
				if(TRUE == is_three_cards(card +i, 3))
					break;
			}

			return card[i] % SINGLE_CARD_NUM;
		}

		case MORE_THREE_ONE:
		{
			for(i = 0; i < length -2; i ++){
				if(TRUE == is_three_cards(card +i, 3))
					break;
			}

			return card[i + (length / 4 -1) * 3] % SINGLE_CARD_NUM;
		}

		case MORE_THREE_TWO:
		{
			for(i = 0; i < length -2; i ++){
				if(TRUE == is_three_cards(card +i, 3))
					break;
			}

			return card[i + (length / 5 -1) * 3] % SINGLE_CARD_NUM;
		}

		default:
			ASSERT(0);
	}
}


// check specified card

static STATUS is_same_card(char a, char b) {

	return ((a % SINGLE_CARD_NUM) == (b % SINGLE_CARD_NUM)) ? TRUE : FALSE;
}


static STATUS is_single_card(char card[], int length) {

	ASSERT(card);
	ASSERT(length == 1);

	return TRUE;
}


static STATUS is_double_queens(char card[], int length) {

	ASSERT(card);
	ASSERT(2 == length);

	if(card[0] == SMALL_QUEEN && card[1] == BIG_QUEEN) {

		return TRUE;
	}

	return FALSE;
}


static STATUS is_double_cards(char card[], int length) {

	ASSERT(card);
	ASSERT(2 == length);

	return is_same_card(card[0], card[1]);
}


static STATUS is_three_cards(char card[], int length) {

	ASSERT(card);
	ASSERT(3 == length);

	if(TRUE == is_same_card(card[0], card[1]) &&
		TRUE == is_same_card(card[1], card[2])) {

		return TRUE;
	}

	return FALSE;
}


static STATUS is_bomb(char card[], int length) {

	ASSERT(card);
	ASSERT(4 == length);

	if(TRUE == is_same_card(card[0], card[1]) &&
		TRUE == is_same_card(card[1], card[2]) &&
		TRUE == is_same_card(card[2], card[3])) {

		return TRUE;
	}

	return FALSE;
}


static STATUS is_sequence(char card[], int length) {

	int i;

	ASSERT(card);
	ASSERT(length >= 5);

	// check if has queen card

	if(TRUE == is_there_queen_card(card, length)) {

		return FALSE;
	}

	// check if has 2 card

	if(TRUE == is_there_2_card(card, length)){

		return FALSE;
	}

	// check sequence

	for(i = 0; i < (length - 1); i ++) {

		if(TRUE != is_same_card(card[i] + 1, card[i+1]))
			return FALSE;
	}

	return TRUE;
}


static STATUS is_three_one(char card[], int length) {

	ASSERT(card);
	ASSERT(length == 4);

	if(TRUE == is_three_cards(card, 3) &&
		FALSE == is_same_card(card[2], card[3])) {

		return TRUE;

	}else if ( TRUE == is_three_cards(card + 1, 3) &&
		FALSE == is_same_card(card[0], card[1])) {

		return TRUE;
	}else {

		return FALSE;
	}
}

static STATUS is_three_two(char card[], int length) {

	ASSERT(card);
	ASSERT(length == 5);

	if(TRUE == is_three_cards(card, 3) &&
		TRUE == is_double_cards(card +3, 2) &&
		FALSE == is_same_card(card[2], card[3])) {

		return TRUE;

	}else if ( TRUE == is_three_cards(card + 2, 3) &&
		TRUE == is_double_cards(card, 2) &&
		FALSE == is_same_card(card[1], card[2])) {

		return TRUE;
	}else {

		return FALSE;
	}
}

static STATUS is_four_two_diff(char card[], int length) {

	int i;

	ASSERT(card);
	ASSERT(6 == length);

	for(i = 0; i < 3; i ++) {
		if(is_bomb(card + i, 4)) {
			break;
		}
	}

	if(3 == i) {
		return FALSE;
	}

	if(0 == i) {
		if(TRUE == is_same_card(card[4], card[5])) {
			return FALSE;
		}

		return TRUE;

	} if(1 == i) {
		if(TRUE == is_same_card(card[0], card[5])) {
			return FALSE;
		}

		return TRUE;
	}else {
		if(TRUE == is_same_card(card[0], card[1])) {
			return FALSE;
		}

		return  TRUE;
	}
}

static STATUS is_four_two_same(char card[], int length) {

	int i;

	ASSERT(card);
	ASSERT(8 == length);

	for(i = 0; i < 5; i ++) {
		if(TRUE == is_bomb(card +i, 4))
			break;
	}

	if((5 == i) || (i % 2))
		return FALSE;

	if(0 == i) {

		if(TRUE == is_double_cards(card +4, 2) &&
			TRUE == is_double_cards(card +6, 2) &&
			FALSE == is_same_card(card[5], card[6])) {

			return TRUE;
		}

		return FALSE;

	}else if(2 == i) {

		if(TRUE == is_double_cards(card, 2) &&
			TRUE == is_double_cards(card +6, 2) &&
			FALSE == is_same_card(card[1], card[6])) {

			return TRUE;
		}

		return FALSE;
	}else {

		if(TRUE == is_double_cards(card, 2) &&
			TRUE == is_double_cards(card + 2, 2) &&
			FALSE == is_same_card(card[1], card[2])) {

			return TRUE;
		}

		return FALSE;
	}
}

static STATUS is_more_double(char card[], int length) {

	int i;

	ASSERT(card);
	ASSERT((length >= 6) && (0 == (length % 2)));

	// check if has queen card

	if(TRUE == is_there_queen_card(card, length)) {

		return FALSE;
	}

	// check if has 2 card

	if(TRUE == is_there_2_card(card, length)){

		return FALSE;
	}

	// check same cards

	for(i = 0; i < (length >> 1); i ++) {

		if(FALSE == is_same_card(card[2*i], card[2*i + 1] )) {
			return FALSE;
		}
	}

	// check sequence

	for(i = 0; i < ((length >> 1) -1); i ++) {

		if(FALSE == is_same_card(card[2*i] + 1, card[2*(i + 1)])) {
			return FALSE;
		}
	}

	return TRUE;
}

static STATUS is_more_three(char card[], int length) {

	int i;

	ASSERT(card);
	ASSERT((length >= 6) && (0 == (length % 3)));

	// check if has queen card

	if(TRUE == is_there_queen_card(card, length)) {

		return FALSE;
	}

	// check if has 2 card

	if(TRUE == is_there_2_card(card, length)){

		return FALSE;
	}

	// check three cards

	for(i = 0; i < (length / 3); i += 3) {

		if(FALSE == is_three_cards(card + i * 3, 3))
			return FALSE;
	}

	// check sequence

	for(i = 0; i < (length / 3 - 1); i ++) {

		if(FALSE == is_same_card(card[3 *i] + 1, card[3*(i+1)]))
			return FALSE;
	}

	return TRUE;
}

static STATUS is_more_three_one(char card[], int length) {

	int i;
	int start;

	ASSERT(card);
	ASSERT((length >= 8) && (0 == length %4));

	// check if there is bomb

	for(i = 0; i < (length - 3); i ++) {

		if(TRUE == is_bomb(card + i, 4))
			return FALSE;
	}

	// check three cards

	for(i = 0; i < (length - 2); i ++) {

		if(TRUE == is_more_three(card + i, length - (length / 4)))
			break;
	}

	if(i == (length - 2))
		return FALSE;

	// check single card

	start = i;
	for(i = 0; i < length - 1; i ++) {

		if(i >= start && i < (start + length - (length /4)))
			continue;

		if((i+1) >= start && (i+1) < (start + length - (length /4)))
			continue;

		if(TRUE == is_same_card(card[i], card[i + 1]))
			return FALSE;
	}

	return TRUE;

}

static STATUS is_more_three_two(char card[], int length) {

	int i;
	int start;

	ASSERT(card);
	ASSERT((length >= 10) && (0 == length %5));

	// check if there is bomb

	for(i = 0; i < (length - 3); i ++) {

		if(TRUE == is_bomb(card + i, 4))
			return FALSE;
	}
	// check three card

	for(i = 0; i < (length - 2); i ++) {

		if(TRUE == is_more_three(card + i, length - (length / 5)))
			break;
	}

	if(i == (length - 2))
		return FALSE;

	// check single card

	start = i;
	i = 0;

	while(i < length - 1) {

		if(i >= start && i < (start + length - (length /5))) {

			i ++;
			continue;
		}

		if((i+1) >= start && (i+1) < (start + length - (length /5))) {

			i ++;
			continue;
		}

		if(FALSE == is_same_card(card[i], card[i + 1])) {

			return FALSE;
		}

		i += 2;
	}

	return TRUE;
}

// check card type

static int  check_type(char* card, int length) {

	ASSERT(card);
	ASSERT(length);
	
	switch(length) {
		
		case 1:
			if(TRUE == is_single_card(card, length)) {

				return SINGLE_CARD;
			}

			ASSERT(0);
			
		case 2:

			if(TRUE == is_double_queens(card, length)) {

				return DOUBLE_QUEENS;

			}else if(TRUE == is_double_cards(card, length)) {

				return DOUBLE_CARD;
			}else {

				ASSERT(0);
			}

		case 3:

			if(TRUE == is_three_cards(card, length)) {

				return THREE_CARD;
			}else {

				ASSERT(0);
			}
			
		case 4:

			if(TRUE == is_bomb(card, length)) {

				return BOMB;

			}else if(TRUE == is_three_one(card, length)) {

				return THREE_ONE;
			}else {

				ASSERT(0);
			}

		case 5:

			if(TRUE == is_sequence(card, length)) {

				return SEQUENCE;

			}else if(TRUE == is_three_two(card, length)) {

				return THREE_TWO;
			}else {

				ASSERT(0);
			}

		case 6:

			if(TRUE == is_sequence(card, length)) {

				return SEQUENCE;

			}else if(TRUE == is_four_two_diff(card, length)) {

				return FOUR_TWO_DIFF;

			}else if(TRUE == is_more_three(card, length)) {

				return MORE_THREE;

			}else if(TRUE == is_more_double(card, length)) {

				return MORE_DOUBLE;
			}else {

				ASSERT(0);
			}

		case 7:

			if(TRUE == is_sequence(card, length)) {

				return SEQUENCE;
			}else {

				ASSERT(0);
			}

		case 8:
			if(TRUE == is_sequence(card, length)) {

				return SEQUENCE;

			}else if(TRUE == is_four_two_same(card, length)) {

				return FOUR_TWO_SAME;

			}else if(TRUE == is_more_three_one(card, length)) {

				return MORE_THREE_ONE;

			}else if(TRUE == is_more_double(card, length)) {

				return MORE_DOUBLE;
			}else {

				ASSERT(0);
			}

		default:
			if(TRUE == is_sequence(card, length)) {

				return SEQUENCE;

			}else if(TRUE == is_more_three_two(card, length)) {

				return MORE_THREE_TWO;

			}else if(TRUE == is_more_three_one(card, length)) {

				return MORE_THREE_ONE;

			}else if(TRUE == is_more_three(card, length)) {

				return MORE_THREE;

			}else if(TRUE == is_more_double(card, length)){

				return MORE_DOUBLE;
			}else {

				ASSERT(0);
			}
	}

	return TYPE_ERR;
}

static _process_card(char* card, int length, int group)
{
	int type;
	int big;
	
	if(game[group].current_card_type == DOUBLE_QUEENS) {

			ASSERT(0);
	} else if(game[group].current_card_type == BOMB) {
		
		type = check_type(card, length);
		if(type == DOUBLE_QUEENS) {

			game[group].current_card_type = DOUBLE_QUEENS;

		} else if(type == BOMB) {

			big = get_big_data(card, length, BOMB);
			ASSERT(big > game[group].current_big);
			
			game[group].current_big = big;

		} else {
			ASSERT(0);
		}
	}else {
		
		type = check_type(card, length);
		if(type == DOUBLE_QUEENS) {

			game[group].current_card_type = DOUBLE_QUEENS;

		}else if(type == BOMB) {

			game[group].current_card_type = BOMB;
			game[group].current_big = get_big_data(card, length, BOMB);
		}else {

			ASSERT(game[group].current_card_type == type);
			ASSERT(game[group].current_card_num == length);

			big = get_big_data(card, length, type);
			ASSERT(big > game[group].current_big);

			game[group].current_big = big;
		}
	}
}

// process card

void process_card(char card[], int length, int sender, int finish, int group) {

	int i;

	// make sure all cards are smaller than 54
	
	if(length) {

		for(i = 0; i < length; i ++)
			ASSERT(card[i] < 54);
	}

	// check if finish already
	
	if(finish) {
		
		return;
	}
	
	// someone try to give up this opportunity
	
	if(0 == length) {
		if(GAME_STATE ==game[group].current_game_state) {
			ASSERT(0);
		}
		
		return;
	}

	// resort card

	resort_cards(card, length);

	// first get card from user
	
	if(0 == game[group].current_owner) {
		
		game[group].current_card_num = length;
		game[group].current_owner = sender;
		
		game[group].current_game_state = RUN_STATE;
		game[group].current_card_type = check_type(card, length);
		game[group].current_big = get_big_data(card, length, game[group].current_card_type);
		
		return;
	}
	
	// sender equals to owner
	
	if(sender == game[group].current_owner) {
		
		game[group].current_card_num = length;
		game[group].current_card_type = check_type(card, length);
		game[group].current_big = get_big_data(card, length, game[group].current_card_type);
		
		return;
	}
	
	// other ways of processing
	
	_process_card(card, length, group);
	
	game[group].current_card_num = length;
	game[group].current_owner = sender;
	
	return;
}

void process_event(int event, int sender, int group) {

	if(0xffffffff == group) {

		return;
	}

	switch (game[group].current_game_state) {

		case INIT_STATE:
			break;

		case SELECT_STATE:
			break;

		case GAME_STATE:
			break;

		case RUN_STATE:
			break;

		default:
			break;
		}
}

#ifndef UNIT_TEST
// entry of all function

int main(int argc, char* argv[]) {

	// init module
	
	// get event
	
	while(1) {
		
		// analyse card
		
		// make logic assessment
		
		// broadcast information to all users
		
		// update score if necessary
		
	}

	return 0;
}
#endif
