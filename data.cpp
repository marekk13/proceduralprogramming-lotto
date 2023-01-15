#include "data.h"
#include <stdio.h> //do obslugi plikow i printf
#include <time.h> //do wyliczania dnia tygodnia ( struktura tm )
#include <malloc.h> //do funkcji realloc
#include <memory.h> // do memset

int  ReadData( Player*** pAllPlayers, const char* sfileName )
{// otwiera plik do odczytu z badaniem bledu otwarcie - gdy blad to funkcja 
  // i zwraca zero
  FILE* fin = NULL; //zdefiniowanie wskaznika na plik
  if( !( fin = fopen( sfileName, "rt" ) ) ) //jesli nic nie odczyta wypisuje sie blad
    //r-otwiera sie do czytania
    //t-w trybie przetlumaczonym
  {
    printf( "blad przy otwietaniu pliku\n" );
    return 0;
  }
  int il = 0;//zmienna liczaca ilosc wczytanych danych
  int maxsize = 0; //akturalny maksymalny rozmiar tablicy pAllPlayers
  Player** pDraws = *pAllPlayers; //kopia parametru *pAllPlayers 
  //aby nie wprowadzac modyfikacji w wejsciowej tablicy
  char c;

  //warunek wejscia petli na graczy- plik sie nie skonczyl i pierwszym znakiem jest gwiazdka
  while( !feof( fin ) && ( c = fgetc( fin ) == '*' ) )
  {
    if( il == maxsize )// jesli trzeba to realokowac  ( ilosc wczytanych danych == maxym rozmiar ) 
    {
      maxsize += AllocAddMem( &pDraws, il );
      if( il == maxsize )// if(  ( ilosc wczytanych danych == maxym rozmiar )  to koniec ( przerw petli )
      {//w razie niepoprawnej alokacji
        break;
      }
    }

    //zmienne pomocnicze
    char gend;
    int hh = 0;
    int mm = 0;
    int d = 0;
    int m = 0;
    int y = 0;
    
    Player* p = pDraws[il] = ( Player* )calloc( 1, sizeof(Player) ); //alokacja pamieci dla gracza
    if( !p )
    {//komunikat o bledzie
      printf( "Allocation error (ReadData)\n" );
      break;
    }

   fscanf(fin, "%s %s %c %4c %2c %2c %3c %2c %2c %8c %16c %d:%d %d/%d/%d %11c ", p->SName, p->FName, &gend,
      p->AccData.swift.instCode, p->AccData.swift.countryCode, p->AccData.swift.cityCode,
      p->AccData.swift.branchCode, p->AccData.iban.countryCode,
      p->AccData.iban.bban.contrIban, p->AccData.iban.bban.bankCode,
      p->AccData.iban.accNumber, &hh, &mm, &d, &m, &y, p->Pesel );
   //operator -> wykorzystujemy dla zmiennych wskaznikowych
   //operator . gdy zmienne nie sa wskaznikami

    if( !SetSex( p, gend ) || !SetDate( p, d, m, y ) || !SetHour( p, hh, mm ) )
      {//wykonanie funkcji z jednoczesnym spr bledu
       printf( "Operation of ReadData function was stopped( SetSex/SetDate/SetHour )\n" );
       break;
      }

    //czytanie zakladow
    int drawNo = 0;//ilosc wczytanych zakladow
    int maxBetsNo = 0;//aktualna maksymalna liczba zakladow, na ktore zostala zaalokowana
    //pamiec z pomoca AllocAddMemBets
    fscanf(fin, "\n");

    //nieskonczona petla na zaklady - za kazdym razem sczytywany jeden zaklad
    //zmienna sign to flaga - pozwala odroznic kiedy mamy przejsc do nastepnego gracza
    //a kiedy pozostac w petli(przejscie do nast zakladu)
    int sign = 0;
    while( 1 )
    {
      if( maxBetsNo == drawNo )
      {//ten sam warunek co w realokacji dla graczy
        maxBetsNo+=AllocAddMemBets( &(pDraws[il]->pBets), drawNo ); 
        if( maxBetsNo == drawNo )
        {
          break;
        }
      }
      int* v = p->pBets[drawNo] = ( int* )calloc( MAXBETS, sizeof( int ) ); 
      if( !v )
      {
       printf( "Allocation error\n" );
       break;
      }
        
       for( int i = 0; i < MAXBETS; i++ ) //6-liczby postawione w jednym zakladzie
        {//wykorzystanie indeksowania aby nie korzystac ze zmiennych pomocniczych
          fscanf( fin, "%d", &v[i] );
        }
      drawNo++;//zaklad wczytany - inkrementacja zmiennej liczacej ilosc zakladow gracza
      p->BetsMax = drawNo;//przypisanie zmiennej(skrocenie zapisu)
      fscanf( fin, "\n" );

      //sprawdzenie warunkow wyjscia petli(zamiast w while)
      //jesli wczytywany znak jest gwiazdka zmieniamy flage, tak aby nie wyjsc z duzej petli
      //i przejsc do nast gracza
      //jesli koniec pliku to nie zmieniamy flagi aby wyjsc z duzej petli
      //w innym przypadku zwracamy znak(bo jest to liczba postawiona przez gracza) i przechodzimy do nastepnego zakladu
      if( (c = fgetc(fin)) == '*' )
      {
        sign = 1;
        ungetc( c, fin );
        break;
      }
      else if( feof(fin) )
      {
        break;
      }
      else ungetc( c, fin );
    }
    
    il++; //gracz zostal dodany prawidlowo

    //sprawdzenie czy chcemy przejsc do next gracza czy koniec wczytywania danych
    if( !sign )
    {
      break;
    }
  }
  *pAllPlayers = pDraws;//przywrocenie wsk
  fclose( fin ); //zamkniecie pliku
  return il;
}


int AllocAddMem( Player*** pTab, int nCurrSize ) //dodac miejsce dla kolejnego gracza
{
  Player** pTemp = *pTab;//wskaznik pomocniczy
  int newsize = ALLOCPLAYER + nCurrSize; //na tyle zakladow ma byc pamiec
  pTemp = ( Player** )realloc( pTemp, newsize * sizeof( Player* ) ); //przypisanie do innego wskaznika, aby nie stracic bloku pTab
  if( !pTemp )
  {//niepowodzenie realokacji
    printf( "Memory was not reallocated( AllocAddMem )\n" );
    return 0;
  }
  *pTab = pTemp; //przypisanie wsk pom
  memset( *pTab+nCurrSize, 0, ALLOCPLAYER* sizeof(Player*) );//wyzerowanie pamieci po przesunieciu wskaznika
  return ALLOCPLAYER;
}

int AllocAddMemBets( int*** pTab, int maxBets ) //dodac miejsce dla kolejnych zakladow
{
  int** pTemp = *pTab;//wskaznik pomocniczy
  int newsize = ALLOCBET + maxBets;
  pTemp = ( int** )realloc( pTemp, newsize * sizeof( int* ) ); //przypisanie do innego wskaznika, aby nie stracic bloku pTab
  if( !pTemp )
  {//niepowodzenie realokacji
    printf( "Memory was not reallocated( AllocAddMemBets )\n" );
    return 0;
  }
  *pTab = pTemp; //przypisanie wsk pom 
  memset( *pTab+maxBets, 0, ALLOCBET*sizeof(int*) );//wyzerowanie pamieci po przesunieciu wskaznika
  return ALLOCBET;
}

void FreeMem( Player*** pTab, int nPlayersNo ) //funkcja zwalniajaca pamiec
{
  Player** p = *pTab; //wskaznik pomocniczy
  //!!!brak zmian w obrebie glownej tablicy graczy
  for( int i = 0; i < nPlayersNo; i++, p++)
  {
    int** pTemp = (*p)->pBets;//jak wyzej, ale na tablicy z zakladami
    for( int j = 0; j < (*p)->BetsMax; j++ )//petla na zaklady
    {
      free( *pTemp++ );//zwalnianie pamieci kolejnych wierszy
    }
    free( (*p)->pBets ); //zwolnienie adresu pierwszego elementu zakladu(nie moze byc pomocniczy)
    pTemp = NULL;
    free( *p ); //zwolnienie pamieci elem player

  }
  free( *pTab ); //zwolnienie wskaznika na 1 elem tablicy player
  *pTab = NULL;
}

int  SetSex( Player* p, char c )
{
  if( c == 'M' || c == 'm' )
  {//uwzglednia zarowno mala i duza litere
    p->Sex = MALE;
    return 1;
  }

  if( c == 'F' || c == 'f' )
  {
    p->Sex = FEMALE;
    return 1;
  }

  printf( "Invalid gender\n" );
  return 0;
}

int  SetDate( Player* p, int d, int m, int y )
// ma obliczyc nazwe dnia tygodnia  ( time.h ) ( switch )  - fukcje biblioteczne WYLACZNIE
  // uzywa nazw zdefiniowanych w typach wyliczeniowych
{
  if( d < 1 || d>31 || m < 1 || m>12 || y < 1990 )
  {//sprawdzanie czy inty sa odpowiednie, inaczej zwraca 0
    printf( "Invalid date\n" );
    return 0;
  }
  p->BetsDate.nYear = y;
  p->BetsDate.nDay = d;

  switch( m )
  {//przypisanie skrotow z typow wyliczeniowych
  case 1: p->BetsDate.Month = Jan; break;
  case 2: p->BetsDate.Month = Feb; break;
  case 3: p->BetsDate.Month = Mar; break;
  case 4: p->BetsDate.Month = Apr; break;
  case 5: p->BetsDate.Month = May; break;
  case 6: p->BetsDate.Month = Jun; break;
  case 7: p->BetsDate.Month = Jul; break;
  case 8: p->BetsDate.Month = Aug; break;
  case 9: p->BetsDate.Month = Jan; break;
  case 10: p->BetsDate.Month = Sep; break;
  case 11: p->BetsDate.Month = Nov; break;
  case 12: p->BetsDate.Month = Dec; break;
  }

  struct tm st {}; //zainicjowanie na 0
  st.tm_isdst = -1;//wylaczenie info o znaczniku czasu letniego
  st.tm_year = y - 1900; //lata od 1900
  st.tm_mon = m - 1; //miesiace od stycznia
  st.tm_mday = d;
  time_t t = mktime( &st ); //gdy funkcja zadziala st.tm_wday obliczany na podstawie info z innych pól struktur
  //niedziela - 0, pon - 1, ...
  switch(st.tm_wday)
  {
  case 0: p->BetsDate.weekDay = Sun; break;
  case 1: p->BetsDate.weekDay = Mon; break;
  case 2: p->BetsDate.weekDay = Tue; break;
  case 3: p->BetsDate.weekDay = Wed; break;
  case 4: p->BetsDate.weekDay = Thu; break;
  case 5: p->BetsDate.weekDay = Fri; break;
  case 6: p->BetsDate.weekDay = Sat; break;
  }
  return 1;
}

int  SetHour( Player* p, int hh, int mm )
{
  if( hh < 0 || hh>23 || mm < 0 || mm >59 )
  {//sprawdzenie czy inty sa odpowiednie
    printf( "Invalid time\n" );
    return 0;
  }
  p->BetsHour.nHour = hh;
  p->BetsHour.nMin = mm;

  return 1;
}