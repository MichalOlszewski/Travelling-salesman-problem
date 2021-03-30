// OK-TSP1.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//
#include <iostream>
#include <fstream>
#include <math.h>
#include <cstdlib>
#include <time.h>
#include <string>

#define LICZBA_KOMBINACJI 5
#define L_POP 80

// chyba jeszcze nie uzyte:


// do grafiki i rysowania:
#include "grafika.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>


// genetyczny
#include "genetyczny.h"
#include <vector>


using namespace sf;

#define granica 5001

using namespace std;

string wczytajPlik(double**& graf, int& rozmiar);
double** zrobGraf(double** graf, int rozmiar);
void wypiszMacierz(double** graf, int rozmiar);
void usunGraf(double**& graf, int& rozmiar);
string wygenerujGraf(double**& graf, int& rozmiar);
void zachlanny(double** graf, int rozmiar,int *&wynik);



int main()
{
	

	bool petla = true;		// glowna petla programu
	int opcja = 0;		 // opcja w menu
	int rozmiar = 0;		 // rozmiar grafu (ilosc wezlow)
	double** graf = NULL;		  // macierz sasiedztwa
	string nazwaPliku; // nazwa aktualnego pliku na ktorym dzialamy 


	while (petla)
	{
		cout << "Wybierz opcje:\n1) Wczytaj graf z pliku\n2) Wygeneruj graf (instancje problemu)\n3) Rozwiaz TSP algorytmem zachlannym\n4) Usun graf\n5) Wypisz macierz\n6) Koniec\n7) Genetyczny\n";
		cin >> opcja;

		switch (opcja)
		{
		case 1:
		{
			nazwaPliku=wczytajPlik(graf, rozmiar);
			break;
		}
		case 2:
		{
			nazwaPliku=wygenerujGraf(graf, rozmiar);
			break;
		}
		case 3:
		{
			if (rozmiar != 0)
			{

				int* wynik;
				wynik = new int[rozmiar];

				clock_t start, stop;
				double czas;
				start = clock();

				zachlanny(graf, rozmiar,wynik);

				stop = clock();
				czas = (double)(stop - start) / CLOCKS_PER_SEC;
				cout << "Czas wykonywania algorytmu: " << czas << "s" << endl;

				main_grafika(nazwaPliku,wynik);

				delete[] wynik;

			}
			else
				cout << "brak zapisanej macierzy!" << endl;
			break;
		}
		case 4:
		{

			usunGraf(graf, rozmiar);
			break;
		}
		case 5:
		{
			if (rozmiar != 0)
			{
				wypiszMacierz(graf, rozmiar);
			}
			else
				cout << "brak zapisanej macierzy!" << endl;
			break;
		}
		case 6:
		{
			usunGraf(graf, rozmiar);
			petla = false;
			break;
		}
		case 7:
		{

			if (rozmiar != 0)
			{
				


			


			
				
				int* wynik;
				wynik = new int[rozmiar];

				clock_t start, stop;
				double czas;
				start = clock();

				genetyczny(graf, rozmiar, wynik, L_POP); //2*rozmiar - liczba kombinacji sciezke

				stop = clock();
				czas = (double)(stop - start) / CLOCKS_PER_SEC;
				cout << "Czas wykonywania algorytmu: " << czas << "s" << endl;

				main_grafika(nazwaPliku, wynik);

				delete[] wynik;

				
			}
			
			else
				cout << "brak zapisanej macierzy!" << endl;
			break;
		}
		default:
		{
			usunGraf(graf, rozmiar);
			petla = false;
			break;
		}
		}


	}


}

//-------------------------------------------------------------------------
//---------------------FUNKCJE---------------------------------------------
//-------------------------------------------------------------------------

void zachlanny(double** graf, int rozmiar,int *&wynik)
{
	int najblizszyW = 0;
	double odleglosc = sqrt(2) * 5000; // maksymalna odleglosc
	
	int row = 0;
	double lacznaOdleglosc = 0;


	wynik[0] = 0; // zawsze zaczynamy od wierzcholłka o indeksie nr 0 
	int indeks = 1;
	graf[0][0] = 1;
	while (indeks < rozmiar)
	{
		for (int col = 0; col < rozmiar; col++)
		{
			if (graf[row][col] < odleglosc && graf[col][col] == 0 && row != col)
			{
				najblizszyW = col;
				odleglosc = graf[row][col];
			}
		}
		wynik[indeks] = najblizszyW;
		indeks++;
		graf[najblizszyW][najblizszyW] = 1;
		row = najblizszyW;
		lacznaOdleglosc += odleglosc;
		odleglosc = sqrt(2) * 5000;


	}
	lacznaOdleglosc += graf[0][najblizszyW];
	for (int i = 0; i < rozmiar; i++)
	{
		cout << wynik[i] + 1 << "->";
	}
	cout << 1 << endl;
	cout << "Odleglosc: " << lacznaOdleglosc;

	// do zapisu wyniku
	fstream plikWyniki;
	plikWyniki.open("wyniki.txt", ios_base::app);
	plikWyniki << lacznaOdleglosc;
	plikWyniki << '\n';
	plikWyniki.close();
	//koniec zapisu wyniku
	cout << endl;
	

}

string wczytajPlik(double**& graf, int& rozmiar)
{
	fstream plik;		  // plik z danymi
	string nazwaPliku;


	cout << "Podaj nazwe pliku z danymi(bez .txt): "; cin >> nazwaPliku;
	plik.open(nazwaPliku + ".txt", ios::in | ios::app);
	plik >> rozmiar;

	graf = zrobGraf(graf, rozmiar);

	// wczytanie i obliczenie wspolrzednych punktow
	double** wspolrzedne = new double* [rozmiar];
	int licznik = 0;
	int smieci;
	while (licznik < rozmiar)
	{
		plik >> smieci;  // wczytuje nr wierzecholka ale nie jest potrzebny
		wspolrzedne[licznik] = new double[2];
		plik >> wspolrzedne[licznik][0];
		plik >> wspolrzedne[licznik][1];
		licznik++;
	}

	for (int i = 0; i < rozmiar; i++)
	{
		for (int j = 0; j < rozmiar; j++)
		{
			graf[i][j] = sqrt(pow(wspolrzedne[j][0] - wspolrzedne[i][0], 2) + pow(wspolrzedne[j][1] - wspolrzedne[i][1], 2));
		}
	}

	for (int i = 0; i < rozmiar; i++)
	{
		delete[]wspolrzedne[i];
	}
	delete[]wspolrzedne;
	plik.close();
	return nazwaPliku;
}

double** zrobGraf(double** graf, int rozmiar)   // tworzy dynamicznie macierz
{
	graf = new double* [rozmiar];

	for (int i = 0; i < rozmiar; i++)
		graf[i] = new double[rozmiar];

	return graf;

}

void wypiszMacierz(double** graf, int rozmiar)
{
	cout << "Liczba wezlow w grafie: " << rozmiar << endl;
	for (int i = 0; i < rozmiar; i++)
	{
		for (int j = 0; j < rozmiar; j++)
		{
			printf("%10.2lf", graf[i][j]);
		}
		cout << "\n";
	}
}

void usunGraf(double**& graf, int& rozmiar)
{
	for (int i = 0; i < rozmiar; i++)
		delete[]graf[i];

delete[]graf;
	rozmiar = 0;
}

string wygenerujGraf(double**& graf, int& rozmiar)
{

	fstream plik;		  // plik z danymi
	string nazwaPliku;

	int zmPrzedzialuY = 0;
	int wyborZm = 0;
	
	cout << "Podaj nazwe pliku do zapisania: "; cin >> nazwaPliku;
	plik.open(nazwaPliku + ".txt", ios::out | ios::app);

	srand(time(NULL));
	cout << "Podaj liczbe wezlow: "; cin >> rozmiar;
	plik << rozmiar;
	plik << '\n';
	cout << "1 - kwadrat, 2 - prostokat 2:1   " << endl;
	cin >> wyborZm;
	zmPrzedzialuY = wyborZm > 1 ? 2500 : 0;

	graf = zrobGraf(graf, rozmiar);
	double** wspolrzedne = new double* [rozmiar];

	bool petla = true;
	for (int i = 0; i < rozmiar; i++)
	{
		wspolrzedne[i] = new double[2];

		petla = true;
		while (petla)
		{
			wspolrzedne[i][0] = rand() % (granica);  // 0<x<5000
			wspolrzedne[i][1] = rand() % (granica- zmPrzedzialuY);  // 0<y<5000

			petla = false;
			for (int j = 0; j < i; j++)
			{
				if ((wspolrzedne[j][0] == wspolrzedne[i][0]) || (wspolrzedne[j][1] == wspolrzedne[i][1]))  // wykluczamy takie same wspolrzedne
				{
					petla = true;
					break;
				}
			}
		}

		plik << i + 1;
		plik << ' ';
		plik << wspolrzedne[i][0] << ' ' << wspolrzedne[i][1] << '\n';
	}


	for (int i = 0; i < rozmiar; i++)
	{
		for (int j = 0; j < rozmiar; j++)
		{
			graf[i][j] = sqrt(pow(wspolrzedne[j][0] - wspolrzedne[i][0], 2) + pow(wspolrzedne[j][1] - wspolrzedne[i][1], 2));
		}
	}

	for (int i = 0; i < rozmiar; i++)
	{
		delete[]wspolrzedne[i];
	}
	delete[]wspolrzedne;


	plik.close();
	return nazwaPliku;
}
/*
int zachlanny(double** graf, int rozmiar)
{

}
*/