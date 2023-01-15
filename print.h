#pragma once
#include "defs.h"
#include "data.h"
#include <stdio.h>


void PrintResults(int* pNums49, Player** pPlayers, int nPlayersNo);
void PrintSex(FILE* fout, Player* p);
void PrintDate(FILE* fout, Player* p);
void PrintHour(FILE* fout, Player* p);
void PrintBets(FILE* fout, Player* p);