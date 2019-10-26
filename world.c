#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
//#include <sys/ioctl.h>
#include "world.h"
#include "event.h"

int maxx;
int maxy;

void drawOuterWall()
{
	int i;
		
	// upper frame
	for(i=1;i<maxx-1;i++){
		move(0,i);
		printw("=");
	}
	
	// lower frame
	for(i=1;i<maxx-1;i++){ 
		move(maxy-1,i);
		printw("=");
	}	
	
	// left frame
	for(i=0;i<maxy;i++){
		move(i,0);
		printw("|");
	}
		
	// right frame
	for(i=0;i<maxy;i++){ 
		move(i,maxx-1);
		printw("|");
	}
	refresh();
}

	
void placeObstacles()
{
	int i,j,y,x,obstacleDensity=0;
	bool bLast, bOneMade = FALSE;
	
	// User chooses obstacle density.
	
	while(obstacleDensity<1||obstacleDensity>50){
		mvprintw(maxy-1,(maxx/2)-18," Enter obstacle density (1-50):    ");
		getyx(stdscr,y,x);
		move(y,x-4);
		scanw("%d",&obstacleDensity);
	}
	drawOuterWall(); // Redraw outer wall.
	
	
	// Generate obstacles
	
	// X-wise obstacles
	for(i=1;i<maxy-1;i++){
		for(j=1;j<maxx-1;j++){
			usleep(100);
			if(bOneMade){  // Always make obstacle longer than one square.
				mvprintw(i,j,"#");
				bOneMade = FALSE;
			}
			else if(bLast && getRndNum(4)!=1)  // Keep making obstacle longer.
				mvprintw(i,j,"#");				
			else if(getRndNum(1000/obstacleDensity)==1){  // new obstacle
				mvprintw(i,j,"#");
				bLast = TRUE;
				bOneMade = TRUE;
			}
			else // Obstacle done
				bLast = FALSE;	
			refresh();
		}
	}

	// Y-wise obstacles
	for(i=1;i<maxx-1;i++){
		for(j=1;j<maxy-1;j++){
			usleep(100);
			if(bOneMade){
				mvprintw(j,i,"#");
				bOneMade = FALSE;
			}
			else if(bLast && getRndNum(4)!=1)
				mvprintw(j,i,"#");
			else if(getRndNum(1000/obstacleDensity)==1){ // new obstacle
				mvprintw(j,i,"#");
				bLast = TRUE;
				bOneMade = TRUE;
			}
			else // Obstacle done
				bLast = FALSE;
			refresh();
		}
	}
}


void buildWorld()
{
	initscr();
	curs_set(0);
	start_color();
	use_default_colors();
	srand(time(NULL));  // world will be generated randomly.
	getmaxyx(stdscr,maxy,maxx); // set world outer boundaries (global var)
	if(maxy<20 || maxx<40){  // If terminal window is too small.
		endwin();
		printf("\n\nTerminal window too small. Enlarge it and run again.\n\n");
		exit(0);
	}
	drawOuterWall();
	placeObstacles();
}


int getNbrOfBeings()
{
	int nbrOfBeings=0,y,x;
	while(nbrOfBeings<1||nbrOfBeings>250){
		mvprintw(maxy-1,(maxx/2)-18," Enter number of beings (1-250):    ");
		getyx(stdscr,y,x);  // Get current cursor position.
		move(y,x-4);  // Move back cursor three steps.
		scanw("%d",&nbrOfBeings);
	}
	drawOuterWall(); // Redraw outer wall.
	return nbrOfBeings;
}


void spawnBeings(Being *beings, const int *nbrOfBeings)
{	
	int i;
	//Being *genesisbeing;
	for(i=0;i<*nbrOfBeings;i++){  // Spawn all beings.
		usleep(30000);  // delay
		spawnBeing(&beings[i], &i);
		if(i==0){  // Set special color on genesis being.
			beings[0].myColor=15;
			turnBeing(&beings[i], &i);
		}
		refresh();
	}	
}


int setSimulationSpeed()
{
	int simulationSpeed=0,y,x;
	while(simulationSpeed<1||simulationSpeed>100){
		mvprintw(maxy-1,(maxx/2)-18," Enter simulation speed (1-100):    ");
		getyx(stdscr,y,x);  // Get current cursor position.
		move(y,x-4);  // Move back cursor three steps.
		scanw("%d",&simulationSpeed);
	}
	drawOuterWall(); // Redraw outer wall.
	return simulationSpeed;
}


void runWorld()
{
	
	int i, nbrOfBeings, simulationSpeed, newBeingToSpawnNbr;
	nbrOfBeings = getNbrOfBeings();
	Being *beings = malloc(nbrOfBeings*sizeof(Being)); 
	spawnBeings(&*beings,&nbrOfBeings);
	drawOuterWall();
	simulationSpeed = setSimulationSpeed();
	
	mvprintw(maxy-1,(maxx/2)-19," Press Enter to start simulation. ");
	getch();
	drawOuterWall();
	nodelay(stdscr, TRUE);  // Don't stop by getch().
	noecho();  // Don't type user input keys to screen during simulation.
	char ch;
	while(ch != 27){
		ch=getch();
		for(i=0;i<nbrOfBeings;i++)
			turnBeing(&beings[i], &i);
		refresh();
		
		// Change simulation speed or nomber of beings during run.
		if(ch=='+' && simulationSpeed<100 )
			simulationSpeed++;
		else if(ch=='-' && simulationSpeed>1)
			simulationSpeed--;
		
		// Create and remove beings during run.
		if(ch==',' && nbrOfBeings > 1){
			mvprintw(beings[nbrOfBeings-1].posy,beings[nbrOfBeings-1].posx," ");  // Erase last being.
			nbrOfBeings--;
			beings = realloc(beings,nbrOfBeings*sizeof(Being));
		}
		else if(ch=='.' && nbrOfBeings < 250){  // Create new being.
			nbrOfBeings++;
			beings = realloc(beings,nbrOfBeings*sizeof(Being));
			newBeingToSpawnNbr = nbrOfBeings - 1;
			spawnBeing(&beings[newBeingToSpawnNbr], &newBeingToSpawnNbr);
		}
		usleep(1000000/simulationSpeed);
	}
	free(beings);
	getch();
	endwin();
}


bool checkIfCoordinatesAreClear(const int *x, const int *y)
{
	bool isClear;
	char checksquare;
	checksquare = mvinch(*y,*x) & A_CHARTEXT;
	if(checksquare == ' ')
		isClear = TRUE;
	else
		isClear = FALSE;
	return isClear;
}
