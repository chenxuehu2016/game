
.PHONY: all clean

all:
	gcc card.c -g -o card
	
clean:
	rm -rf card.o card
