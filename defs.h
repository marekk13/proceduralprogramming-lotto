#pragma once
#define MAXBETS      6   // ilosc zakladow
#define ALLOCPLAYER  3   // liczba realokowanych graczy za jednym razem ( realloc() )
#define ALLOCBET     2   // liczba realokowanych zakladow dla danego gracza
#define MAXNAME      31  //dlugosc nazwiska -30 znakow - string, Imie analogicznie
#define POSSNUM			 49

#define LPESEL 11

#define SWIFTMAX1 4
#define SWIFTMAX2 2
#define SWIFTMAX3 2
#define SWIFTMAX4 3

#define IBANMAX1 2
#define IBANMAX2 16

#define BBANMAX1 2
#define BBANMAX2 10

typedef enum { MALE, FEMALE } SexType; //gdy wywolamy bedzie 0, 1, ...
typedef enum { Mon, Tue, Wed, Thu, Fri, Sat, Sun }    DayNames;
typedef enum { Jan, Feb, Mar, Apr, May, Jun, Jul, Aug, Sep, Oct, Nov, Dec }  MonthNames;

//typy strukturalne dla konta (3 typy - BBAN IBAN SWIFT)
//i czwarty ktory ma pola kazdego z tycvh trzech typow
typedef struct
{
	const char instCode[SWIFTMAX1]; //4 znaki
	const char countryCode[SWIFTMAX2]; //2 znaki
	const char cityCode[SWIFTMAX3]; //2 znaki
	const char branchCode[SWIFTMAX4]; //3 znaki
} SWIFT;

typedef struct
{
	const char contrIban[BBANMAX1]; //2 litery
	const char bankCode[BBANMAX2]; //10 cyfr
}BBAN;

typedef struct
{
	char countryCode[IBANMAX1]; //2 litery
	BBAN bban;
	char accNumber[IBANMAX2]; //16 cyfr
} IBAN;


typedef struct
{
	SWIFT swift;
	IBAN iban;
} ACC;


typedef struct
{
	int nDay;
	DayNames weekDay;
	MonthNames Month;
	int nYear;
} DateType;

typedef struct
{
	int nHour;
	int nMin;
} TimeType;

typedef struct
{
	char SName[MAXNAME]; //nazwisko
	char FName[MAXNAME]; //imie
	char Pesel[LPESEL];
	TimeType BetsHour; //godzina zawarcia zakladow
	DateType BetsDate; //data zawarcia zakladow
	SexType Sex;
	ACC  AccData; //dane na temat konta (swift iban i bban zawarty w iban)
	int  BetsMax; //rozmiar tabl pBets      (obliczane podczas wczytywania)
	int** pBets; //tablica wskaznikow do zakladow (dynamicznie alokowana podczas wczytywania - realloc())
	//Jesli na koncu nie ma zakladu to element tabl jest NULL
} Player;