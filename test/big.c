
#define UNIT_TEST

#include "../card.c"

int main(int argc, char* argv[]){

	char card[54];
	
	ASSERT(TRUE == get_big_data(card, 2));
	return 0;
}

