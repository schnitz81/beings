#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include "being.h"
#include "world.h"
#include "ai.h"
#include "event.h"

//Being beings[nbrOfBeings];  // Create data type to store all beings.

void setBeingDefaults(Being *beingToGiveLife,int *x,int *y)
{
	beingToGiveLife->posx = *x;
	beingToGiveLife->posy = *y;
	beingToGiveLife->myHeading = getRndNum(8)-1;

	beingToGiveLife->obstacles.leftfar = 0;
	beingToGiveLife->obstacles.leftnear = 0;
	beingToGiveLife->obstacles.middlefar = 0;
	beingToGiveLife->obstacles.middlenear = 0;
	beingToGiveLife->obstacles.rightfar = 0;
	beingToGiveLife->obstacles.rightnear = 0;
	
	beingToGiveLife->resting = TRUE;
	beingToGiveLife->myColor = getRndNum(5)+1;
}


void spawnBeing(Being *beingToGiveLife, int *beingNbr)
{
	//choose coordinate without obstacle or other being
	bool coordinateIsClear = FALSE;
	int testx;
	int testy;
	while(!coordinateIsClear){
		testx = getRndNum(maxx-1);
		testy = getRndNum(maxy-1);
		coordinateIsClear = checkIfCoordinatesAreClear(testx,testy);
	}
	setBeingDefaults(beingToGiveLife,&testx,&testy);
	
	// Initial placing of being.
	init_pair(*beingNbr, beingToGiveLife->myColor, -1);  // Set color to beingNbr 	
	attron(COLOR_PAIR(*beingNbr));
	mvprintw(beingToGiveLife->posy,beingToGiveLife->posx,"*");
	attroff(COLOR_PAIR(*beingNbr));
}


void movement(Being *beingToTurn)
{
	if(!beingToTurn->resting){
		if(beingToTurn->myHeading == UP){
			beingToTurn->posy--;		
		}
		else if(beingToTurn->myHeading == UPRIGHT){
			beingToTurn->posy--;		
			beingToTurn->posx++;
		}
		else if(beingToTurn->myHeading == RIGHT){
			beingToTurn->posx++;
		}
		else if(beingToTurn->myHeading == DOWNRIGHT){
			beingToTurn->posy++;		
			beingToTurn->posx++;
		}
		else if(beingToTurn->myHeading == DOWN){
			beingToTurn->posy++;				
		}
		else if(beingToTurn->myHeading == DOWNLEFT){
			beingToTurn->posy++;		
			beingToTurn->posx--;
		}
		else if(beingToTurn->myHeading == LEFT){
			beingToTurn->posx--;
		}	
		else if(beingToTurn->myHeading == UPLEFT){
			beingToTurn->posy--;		
			beingToTurn->posx--;
		}
	}
}


void turnBeing(Being *beingToTurn, int *beingNbr)
{
	//Erase old position
	mvprintw(beingToTurn->posy,beingToTurn->posx, " ");
	
	// Evaluate and choose
	decision(beingToTurn);
	
	//Move according to decision.
	movement(beingToTurn);
	
	// Print new position.
	init_pair(*beingNbr, beingToTurn->myColor, -1);  // Set color to beingNbr 	
	attron(COLOR_PAIR(*beingNbr));
	mvprintw(beingToTurn->posy,beingToTurn->posx, "*");
	attroff(COLOR_PAIR(*beingNbr));
}
