#ifndef WORLD_H
#define WORLD_H

#include <stdbool.h>
#include "being.h"

extern int maxx;
extern int maxy;


/* Function prototypes */

void placeObstacles();
void buildWorld();
int getNbrOfBeings();
void spawnBeings(Being *beings, int nbrOfBeings);
int setSimulationSpeed();
void runWorld();
bool checkIfCoordinatesAreClear(int x, int y);


#endif

