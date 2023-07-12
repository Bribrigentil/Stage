#include<iostream>
#include<cmath>
#include"fonctions.h"

using namespace std;

double rejetgaussien(double moyenne, double variance) { // Fournit valeur alea. de densite gaussienne
  double x, y, ecarttype = sqrt(variance); // avec variance var et moyenne 0.
  do { // On prend x dans l'intervalle [-5sigma,5sigma], on suppose que la
    x = 10.*ecarttype * drand48() - 5.*ecarttype + moyenne; // gaussienne est nulle ailleurs.
    y = drand48();
  } while(y > exp(-(x-moyenne)*(x-moyenne)/(2.*variance)));
  return x;
}

double distribution_lorentzienne(double moyenne, double largeur) {
  double x;
  x = largeur/2 * tan(M_PI*(drand48()-0.5)) + moyenne;
  return x;
}

double distribution_exponentielle(double alpha) {
  double x;
  x = -alpha*log(drand48());
  return x;
}

double norm(double x, double y) {
  double n = sqrt(x*x+y*y);
  return n;
}

