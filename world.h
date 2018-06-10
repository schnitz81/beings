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
void spawnBeings(Being *beings, const int *nbrOfBeings);
int setSimulationSpeed();
void runWorld();
bool checkIfCoordinatesAreClear(const int *x, const int *y);


#endif

