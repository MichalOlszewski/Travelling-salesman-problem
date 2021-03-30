#pragma once
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

int main_grafika(string,int*);

double maxY(double**,int);
double maxX(double**,int);