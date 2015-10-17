
#define UNIT_TEST

#include "../card.c"

int main(int argc, char* argv[]){

	char card[54];
	
	deal_card(card, 54, 0);
	return 0;
}

