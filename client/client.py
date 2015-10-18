
import pygame
from pygame.locals import *
from sys import exit

SCREEN_WIDTH = 400
SCREEN_HEIGHT = 800

pygame.init()
screen = pygame.display.set_mode((SCREEN_WIDTH, SCREEN_HEIGHT))
pygame.display.set_caption('airplane fight')

background = pygame.image.load("back.png")

while True:
	screen.fill(0)
	screen.blit(background, (0,0))

	pygame.display.update()
	for event in pugame.event.get()
		if event.type == pygame.QUIT
			pygame.quit()
			exit()


