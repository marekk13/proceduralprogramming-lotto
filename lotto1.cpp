// lotto1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "data.h"
#include "defs.h"
#include "Print.h"
#include <malloc.h> //do heapchk

void CalcStat( int* pNums49, Player** pPlayers, int nPlayersNo );
void Sort( Player** pPlayers, int nPlayersNo );  // sortuje po nazwisko + imie ( jesli nazwisko ident )
int stringcomp( const char* p1, const char* p2 );//funkcja pomocnicza do porownania lancuchow
void HeapCheck();

int main( int argc, char* argv[] )
{
  //sprawdzenie czy dwa parametry funkcji jesli nie to blad
  if( argc != 2 )
  {
    printf( "Wrong number of arguments\n" );
    return 2;
  }

  //definicja dynamicznej tablicy typu Player** ( tabl wsk do graczy ) 
  Player** players = NULL;

  //wczytywanie danych
  int noPlayers = ReadData( &players, argv[1] );
  if( !noPlayers )
  {//komunikat o bledzie
    printf( "No data was loaded\n" );
    return 2;
  }

  //sprawdzenie stosu
  HeapCheck();

  //definicja tablicy na statystyki 
  int stats[POSSNUM] = { 0 };

  //obliczenie ile razy dana liczba zostala wybrana
  CalcStat( stats, players, noPlayers );

  //sortowanie po nazwisku
  Sort( players, noPlayers );

  //wypisanie rezultatow w pliku
  PrintResults( stats, players, noPlayers );

  //zwolnienie pamieci
  FreeMem( &players, noPlayers );

  //sprawdzenie stosu
  HeapCheck();

  return 0;
}

void CalcStat( int* pNums49, Player** pPlayers, int nPlayersNo ) //staty w tablicy intow
{//nalezy wykorzystac wsk pomocnicze, aby wejsciowe wskazniki pozostaly niezmienione
  //i zeby mozna bylo je pozniej poprawnie wykorzystac
  if( !pPlayers )
  {
    printf( "Invalid pointer ( Calcstat )\n" );
    return;
  }
  Player** p = pPlayers;
  for( int i = 0; i < nPlayersNo; i++ )//petla po graczach
  {
    Player* p1 = *pPlayers++;
    int** pTemp = p1->pBets;
    for( int j = 0; j < p1->BetsMax; j++, pTemp++ )//petla po zakladach(betsmax - liczba zakladow gracza)
    {
      int* pTemp1 = *pTemp;
      for( int k = 0; k < MAXBETS; k++, pTemp1++ )//petla po pojedynczym zakladzie
      {
        pNums49[*pTemp1-1]++;//indeks - typowana liczba, wartosc w tablicy - ilosc wystapien
      }
    }
  }
}

void Sort( Player** pPlayers, int nPlayersNo ) // sortuje po nazwisko + imie ( jesli nazwisko ident )
//nie przestawiac graczy, zamienic adresy, uzywac indeksowania
//sortowanie przez wybieranie
//warunek zamiany minimum uwzglednia sytuacje gdy powtarzaja sie nazwiska
{
  for( int i = 0; i < nPlayersNo - 1; i++ )
  {
    Player* min = pPlayers[i];
    int x = i;

    for( int j = i + 1; j < nPlayersNo; j++ )
    {
      if( ( stringcomp( pPlayers[j]->SName, min->SName ) ) < 0
        || ( stringcomp( pPlayers[j]->SName, min->SName ) == 0 && ( ( stringcomp( pPlayers[j]->FName, min->FName ) ) < 0 ) ) )
      {
        min = pPlayers[j];
        x = j;
      }
    }
    pPlayers[x] = pPlayers[i];
    pPlayers[i] = min;
  }
}

int stringcomp( const char* p1, const char* p2 )//funkcja pomocnicza porownujaca stringi
{
  char c1;
  char c2;
  do
  {
    c1 = *p1++;
    c2 = *p2++;
    if( c1 == '\0' )
      return c1 - c2;
  } while( c1 == c2 );
  return c1 - c2;
}

void HeapCheck()  //przekopiowana z helpow
{
  int heapstatus = _heapchk();
  switch( heapstatus )
  {
  case _HEAPOK:
    printf(" OK - heap is fine\n\n");
    break;
  case _HEAPEMPTY:
    printf(" OK - heap is empty\n\n");
    break;
  case _HEAPBADBEGIN:
    printf("ERROR - bad start of heap\n\n");
    break;
  case _HEAPBADNODE:
    printf("ERROR - bad node in heap\n\n");
    break;
  }
}