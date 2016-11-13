#ifndef BEING_H
#define BEING_H

typedef enum heading { UP, UPRIGHT, RIGHT, DOWNRIGHT, DOWN, DOWNLEFT, LEFT, UPLEFT } Heading;
typedef enum object { NONE, OTHERBEING, FENCE } Object;
typedef struct sight {
	 Object leftnear;
	 Object leftfar; 
	 Object middlenear;
	 Object middlefar;
	 Object rightnear;
	 Object rightfar;
} Sight;

typedef struct being{
	int posx;
	int posy;
	Heading myHeading;
	//Sight peek;
	Sight obstacles;
	bool resting;
	int myColor;
} Being;


/* Function prototypes */

void setBeingDefaults(Being *beingToGiveLife,int *x,int *y);
void spawnBeing(Being *beingToGiveLife, int *beingNbr);
void movement(Being *beingToTurn);
void turnBeing(Being *beingToTurn, int *beingNbr);

#endif

