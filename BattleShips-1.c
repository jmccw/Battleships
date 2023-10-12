// This program ...
//Author: ..

#include<stdio.h>
#include<time.h>

#define MINBATTLEZONEHEIGHT 6
#define MINBATTLEZONEWIDTH  6
#define MAXBATTLEZONEHEIGHT 20
#define MAXBATTLEZONEWIDTH  20
#define MINFLEETSIZE        1

#define SEA         '~'
#define COMPUTERSHIP O
#define USERSHIP     X

int zoneWidth, zoneHeight;
char battleZone[MAXBATTLEZONEWIDTH][MAXBATTLEZONEHEIGHT];

int fleetSize;

int getIntInRange(int bot, int top){
	//Get an Integer from the input and
	//return it, if it is in the range bot to top, inclusive	
	
	int val; 
	
	printf("\n(%d to %d) > : ", bot, top);
	scanf("%d", &val);
	
	while(val < bot || val > top){
		printf("\nOut of Range\nTry Again: ");
		scanf("%d", &val);
	}
	return val;
}

int initBattleZoneWidth(){
	printf("\nEnter Valid Battle Zone Width");
	return getIntInRange(MINBATTLEZONEWIDTH, MAXBATTLEZONEWIDTH);
}

int initBattleZoneHeight(){
	printf("\nEnter Valid Battle Zone Height");
	return getIntInRange(MINBATTLEZONEHEIGHT, MAXBATTLEZONEHEIGHT);
}

int initFleetSize(int max){
	
	printf("\nEnter Fleet Size");
	return getIntInRange(MINFLEETSIZE, max);
}



void setUp(){
	//Initialize Battlezone Width
	//Initialize Battlezone Height
	//Initialize Fleet size
	
	//Initialize the BattleZone with "empty sea"
	
	//Initialize Random Number seed.
	
	zoneWidth  = initBattleZoneWidth();
	zoneHeight = initBattleZoneHeight();
	fleetSize  = initFleetSize((zoneWidth*zoneHeight)/2);
	
}

void printBattleZone(){
	//
}

void placeMyShips(){
	//Ask the user for fleet size
	//Check fleet size is valid
	//Set a global variable (fleetSize) to hold this size
	//Ask user for fleet size number of x,y coordinates
	//so that the ships can be placed in the BattleZone
	//Ensure user not overwriting previously placed ship
	//Placing a ship, means writing a character to represent the user's 
	//ship into the grid at x,y
}

void placeComputerShips(){
	//Place fleet size number of ships.
	//It will generate a random number, x, between 0 and zoneWidth
	//for the x ordinate.
	//It will generate a random number, y, between 0 and zoneHeigh
	//for the y ordinate.
	//Check that there is no ship at x,y and place a ship there.
	//If a ship already exists at x,y then choose again.
	//Placing a ship, means writing a character to represent the computer's 
	//ship into the grid at x,y
}

void play(){
	//
}


int main(){
	
	setUp();
	printBattleZone();
	placeMyShips();
	placeComputerShips();
	play();
	return(0);
}
