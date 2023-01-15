#pragma once
#include "defs.h"

int  ReadData(Player*** pAllPlayers, const char* sfileName);
int  AllocAddMem(Player*** pTab, int nCurrSize);  //realok dla graczy 
int AllocAddMemBets(int*** pTab, int maxBets); //dodac miejsce dla kolejnych zakladow
void FreeMem(Player*** pTab, int nPlayersNo);
int  SetSex(Player* p, char c);
int  SetDate(Player* p, int d, int m, int y);
int  SetHour(Player* p, int hh, int mm);