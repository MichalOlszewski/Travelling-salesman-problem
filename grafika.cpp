#include <iostream>
#include <fstream>
#include <math.h>
#include <cstdlib>
#include <time.h>
#include <string>


// do grafiki i rysowania:
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

using namespace sf;
using namespace std;


double maxY(double **tab,int rozmiar) {
	double maks = 0;
	for (int i = 0; i < rozmiar; i++)
	{
		if (tab[i][1] > maks)
		{
			maks = tab[i][1];
		}
	}
	return maks;
}
double maxX(double **tab,int rozmiar)
{
	double maks = 0;
	for (int i = 0; i < rozmiar; i++)
	{
		if (tab[i][0] > maks)
		{
			maks = tab[i][0];
		}
	}
	return maks;

}




int main_grafika(string nazwaPliku,int* wynik)
{
	
	

	fstream plik;		  // plik z danymi
	int rozmiar;
	string wynikk;
	

	// do odczytu wyniku
	fstream plikWyniki;
	plikWyniki.open("wyniki.txt", ios::in | ios::app);

	while (!plikWyniki.eof())
	{
		plikWyniki >> wynikk;
	}

	
		
	plikWyniki.close();
	//koniec odczytu wyniku


	// najpierw wczytamy z pliku wsp wierzcholkow:
	plik.open(nazwaPliku + ".txt", ios::in | ios::app);
	plik >> rozmiar;
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


	int krawedz = maxY(wspolrzedne, rozmiar) > maxX(wspolrzedne, rozmiar) ? maxY(wspolrzedne, rozmiar) : maxX(wspolrzedne, rozmiar);
	

	const Color& kolorTla = Color(236, 236, 236, 255);
	const Color& kolor1lini = Color(232, 154, 44, 255);
	const Color& kolorSiatki = Color(217, 215, 215, 255);
	const Color& kolorWynik = Color(221, 31, 22, 255);

	RenderWindow window{ VideoMode{900,900},"Rozwiazanie" };
	window.setActive(true);
	window.setFramerateLimit(10);

	// kolo
	CircleShape kolo(6);
	kolo.setFillColor(Color(84, 109, 165, 255));
	kolo.setOrigin(kolo.getRadius(), kolo.getRadius());

	// linia
	Vertex linia[] 
	{
	Vertex(Vector2f(wspolrzedne[0][0] + 50,wspolrzedne[0][1] + 50),Color::Red),
	Vertex(Vector2f(wspolrzedne[1][0] + 50,wspolrzedne[1][1] + 50),Color::Red)

	};
	// numerki
	Font tff;
	if (!tff.loadFromFile("KeepCalm-Medium.ttf"))
	{
		cout << "NIE ZALADOWANO" << endl;
	}

	string numer = "1";
	Text txt;
	txt.setFont(tff);
	txt.setString(to_string(krawedz));   //
	txt.setCharacterSize(48);   //
	txt.setFillColor(Color::Black);


	// linie ukladu wsp:
	RectangleShape rect1;
	rect1.setSize(sf::Vector2f(800, 2));
	rect1.setPosition(50,850 );
	rect1.setFillColor(Color::Black);
	RectangleShape rect2;
	rect2.setSize(sf::Vector2f(2, 800));
	rect2.setPosition(50, 50);
	rect2.setFillColor(Color::Black);

	Event event;

	while (window.isOpen())
	{
		window.clear(kolorTla);
		
		window.pollEvent(event);
		if (event.type == Event::Closed)
		{
			window.close();
			break;
		}


		//linia[0] = Vertex(Vector2f(tab[0][0] + 150, tab[0][1] + 150), Color::Red);


	
		for (int i = 0; i < 8; i++)
		{

			linia[0] = Vertex(Vector2f(150+100*i,50), kolorSiatki);
			linia[1] = Vertex(Vector2f(150+100*i,850), kolorSiatki);
			window.draw(linia, 2, Lines);
			linia[0] = Vertex(Vector2f(50, 50+100*i), kolorSiatki);
			linia[1] = Vertex(Vector2f(850, 50+100*i), kolorSiatki);
			window.draw(linia, 2, Lines);
		}

		window.draw(rect1);
		window.draw(rect2);


		txt.setString(to_string(krawedz));   //
		txt.setCharacterSize(30);   //
		txt.setPosition(765, 855);
		window.draw(txt);
		txt.setString("x");
		txt.setPosition(860, 830);
		window.draw(txt);
		txt.setString("y");
		txt.setPosition(20, 38);
		window.draw(txt);

		txt.setCharacterSize(25);
		txt.setString("Rozwiazanie (laczna odleglosc): ");
		txt.setPosition(50, 856);
		window.draw(txt);

		txt.setFillColor(kolorWynik);
		txt.setString(wynikk);
		txt.setPosition(500, 856);
		window.draw(txt);
		txt.setFillColor(Color::Black);

		window.draw(linia, 2, Lines);
		//rysuje polaczenia miedzy wierzcholkami

		

		for (int i = 0; i < rozmiar-1; i++)
		{
			if (i == 0)
			{
				linia[0] = Vertex(Vector2f(50 + (wspolrzedne[wynik[i]][0] / krawedz * 800), 849 - (wspolrzedne[wynik[i]][1] / krawedz * 800)), kolor1lini);
				linia[1] = Vertex(Vector2f(50 + (wspolrzedne[wynik[i + 1]][0] / krawedz * 800), 849 - (wspolrzedne[wynik[i + 1]][1] / krawedz * 800)), kolor1lini);
				window.draw(linia, 2, Lines);
				linia[0] = Vertex(Vector2f(50 + (wspolrzedne[wynik[i]][0] / krawedz * 800), 851 - (wspolrzedne[wynik[i]][1] / krawedz * 800)), kolor1lini);
				linia[1] = Vertex(Vector2f(50 + (wspolrzedne[wynik[i + 1]][0] / krawedz * 800), 851 - (wspolrzedne[wynik[i + 1]][1] / krawedz * 800)), kolor1lini);
				window.draw(linia, 2, Lines);
				linia[0] = Vertex(Vector2f(50 + (wspolrzedne[wynik[i]][0] / krawedz * 800), 850 - (wspolrzedne[wynik[i]][1] / krawedz * 800)), kolor1lini);
				linia[1] = Vertex(Vector2f(50 + (wspolrzedne[wynik[i + 1]][0] / krawedz * 800), 850 - (wspolrzedne[wynik[i + 1]][1] / krawedz * 800)), kolor1lini);
			}
			else
			{
				linia[0] = Vertex(Vector2f(50 + (wspolrzedne[wynik[i]][0] / krawedz * 800), 850 - (wspolrzedne[wynik[i]][1] / krawedz * 800)), Color::Red);
				linia[1] = Vertex(Vector2f(50 + (wspolrzedne[wynik[i + 1]][0] / krawedz * 800), 850 - (wspolrzedne[wynik[i + 1]][1] / krawedz * 800)), Color::Red);

			}
			
		
			window.draw(linia, 2, Lines);
		}
		linia[0] = Vertex(Vector2f(50 + (wspolrzedne[wynik[rozmiar-1]][0] / krawedz * 800), 850 - (wspolrzedne[wynik[rozmiar-1]][1] / krawedz * 800)), Color::Red);
		linia[1] = Vertex(Vector2f(50 + (wspolrzedne[wynik[0]][0] / krawedz * 800), 850 - (wspolrzedne[wynik[0]][1] / krawedz * 800)), Color::Red);

		window.draw(linia, 2, Lines);


		//rysuje punkty
		txt.setCharacterSize(10);
		for (int i = 0; i < rozmiar; i++)
		{
			kolo.setPosition(50 + (wspolrzedne[i][0]/krawedz*800), 850 - (wspolrzedne[i][1]/krawedz*800));
			window.draw(kolo);

			txt.setPosition(46 + (wspolrzedne[i][0]/krawedz*800), 860 - (wspolrzedne[i][1]/krawedz*800));
			txt.setString(to_string(i + 1));
			window.draw(txt);
		}

		window.display();
	}


	return 0;
}