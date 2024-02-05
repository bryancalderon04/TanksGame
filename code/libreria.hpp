#ifndef LIBRERIA_HPP
#define LIBRERIA_HPP
#include "Shell.cpp"

Window ventana("Proyecto Shell",420,240);
void borraTanque(int x, int y);
void dibujacaja(COLORREF***cajas,int*x,int*y,int n);
int chocaCaja(int x,int y, int*xCoord,int*yCoord);
int disparaBala(int x,int y, int posicion,COLORREF***imagen,COLORREF***cajas,int*xCoord,int*yCoord,int xEnemigo,int yEnemigo);
int destruyeCaja(COLORREF***cajas,int*xCoord,int*yCoord,int x,int y);
int destruyeTanque(int xr,int yr,int x,int y);
int*coordenadas(int n,int k);
COLORREF*** matrizCajas(int n,int*x,int*y);
COLORREF***balasMatriz();





#endif
