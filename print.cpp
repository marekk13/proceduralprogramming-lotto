#include "Print.h"
#include <stdio.h> 

void PrintResults( int* pNums49, Player** pPlayers, int nPlayersNo )
{
  FILE* fout = fopen( "wyniki.txt", "w+" ); //w+ tworzy nowy plik w trybie do odczytu i zapisu
  Player** pPlTemp = pPlayers;
  //petla po kazdym graczu
  for( int i = 0; i < nPlayersNo; i++, pPlTemp++)
  {
    Player* pPlayer = *pPlTemp; //wskaznik na kolejnych graczy
    fprintf( fout, "%s %s    ", pPlayer->SName, pPlayer->FName ); //wydruk nazwiska
    PrintSex( fout, pPlayer );//wydruk plci
    PrintHour( fout, pPlayer );//wydruk godzint zakladu
    PrintDate( fout, pPlayer );//wydruk daty
    fprintf( fout, "\n" );//nowa linia
    fprintf( fout, "%.4s %.2s %.2s %.3s     %.2s %.2s %.10s %.16s\n", pPlayer->AccData.swift.instCode,
      pPlayer->AccData.swift.countryCode, pPlayer->AccData.swift.cityCode, pPlayer->AccData.swift.branchCode,
      pPlayer->AccData.iban.countryCode, pPlayer->AccData.iban.bban.contrIban, pPlayer->AccData.iban.bban.bankCode,
      pPlayer->AccData.iban.accNumber );//wydruk danych bankowych
    fprintf( fout, "--------------------------------------------------------\n" );
    PrintBets( fout, pPlayer );//wydruk zakladow
  }
  //przejscie do statystyk
  fprintf( fout, "\n\n\n ======================================= \n     S T A T I S T I C S\n =======================================\n" );

  //petla po kazdej mozliwej postawionej licznie
  for( int i = 0; i < POSSNUM; i++ )
  {//wypisanie ile razy zostala postawiona kazda liczba
    //mozna bez wsk pom bo tablica ze statystykami nie bedzie wiecej wykorzystana
    fprintf( fout, "%2d:%2d ", i+1, *pNums49++ );
    if( !( ( i + 1 ) % 10 ) )
      fprintf( fout, "\n" );//nowa linia co 10
  }
  fclose( fout );//zamkniecie pliku
}

void PrintSex( FILE* fout, Player* p )
{
  if( !p )
  {
    printf( "Invalid pointer, PrintSex\n" );
    return;
  }


  if(  p->Sex  == MALE )
  {
    fprintf( fout, "*Mezczyzna* " );
    return;
  }
  else
  {
    fprintf( fout, "*Kobieta* " );
    return;
  }
}

void PrintDate( FILE* fout, Player* p )
{
  if( !p )
  {//spr wskaznika
    printf( "Invalid pointer, PrintDate\n" );
    return;
  }
  //wypisanie skrotu dnia tygodnia
  switch( p->BetsDate.weekDay )
  {
  case Mon: fprintf( fout, "Mon, " ); break;
  case Tue: fprintf( fout, "Tue, " ); break;
  case Wed: fprintf( fout, "Wed, " ); break;
  case Thu: fprintf( fout, "Thu, " ); break;
  case Fri: fprintf( fout, "Fri, " ); break;
  case Sat: fprintf( fout, "Sat, " ); break;
  case Sun: fprintf( fout, "Sun, " ); break;
  }
  //wypisanie dnia miesiaca
  fprintf( fout, "%d/", p->BetsDate.nDay );

  //wypisanie miesiaca
  switch( p->BetsDate.Month )
  {
  case Jan: fprintf( fout, "Jan/" ); break;
  case Feb: fprintf( fout, "Feb/" ); break;
  case Mar: fprintf( fout, "Mar/" ); break;
  case Apr: fprintf( fout, "Apr/" ); break;
  case May: fprintf( fout, "May/" ); break;
  case Jun: fprintf( fout, "Jun/" ); break;
  case Jul: fprintf( fout, "Jul/" ); break;
  case Aug: fprintf( fout, "Aug/" ); break;
  case Sep: fprintf( fout, "Sep/" ); break;
  case Oct: fprintf( fout, "Oct/" ); break;
  case Nov: fprintf( fout, "Nov/" ); break;
  case Dec: fprintf( fout, "Dec/" ); break;
  }
  //wypisanie roku
  fprintf( fout, "%d", p->BetsDate.nYear );
}

void PrintHour( FILE* fout, Player* p )
{
  if( !p )
  {//spr wskanzika
    printf( "Invalid pointer, PrintHour\n" );
    return;
  }
  //wydruk godziny
  fprintf( fout, "%d:", p->BetsHour.nHour );
  if( p->BetsHour.nMin < 10 )
  {
    fprintf( fout, "0" );
  }
  fprintf( fout, "%d ", p->BetsHour.nMin );
}

void PrintBets( FILE* fout, Player* p )
{//nalezy wykorzystac wsk pomocnicze, aby wejsciowe wskazniki pozostaly niezmienione
  //i zeby mozna bylo je pozniej poprawnie wykorzystac
  Player* pTemp = p; 
  int** pTab = p->pBets;//wskaznik pomocniczy, aby nie modyfikowac wskaznikow na wiersze
  for( int i = 0; i < p->BetsMax; i++ )  //petla po tablicy wskaznikow
  {
    int* temp = *pTab++; 
    for( int j = 0; j < MAXBETS; j++ )
    {
      fprintf( fout, "%2d ", *temp++ ); //wypisanie kolejnych elementow
    }
    fprintf( fout, "\n" ); //konczy wiersz nowa linia
  }
  fprintf(fout, "\n");//nowa linia przed nastepnym graczem
}
