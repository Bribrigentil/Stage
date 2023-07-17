#include<iostream>
#include<ostream>
#include<cmath>
#include"fonctions.h"
#include"chlamy2.h"

using namespace std;

//Constructeurs de la classe chlamy

chlamy::chlamy() {
  x = 0;
  y = 0;
  vx = 0;
  vy = 0;
  v0 = 1;
  Taille = 9;
}

chlamy::chlamy(vector<chlamy> groupe, double Taillesurfacex, double Taillesurfacey, double vmoy) {
  Taille = rejetgaussien(8,0.5);
  positioninitiale(groupe, Taillesurfacex, Taillesurfacey);
  vitesseinitiale(vmoy);
}


//Destructeur de la classe chlamy
chlamy::~chlamy() {
}

//On initialise les positions initiales des oiseaux
void chlamy::positioninitiale(vector<chlamy> groupe, double Taillesurfacex, double Taillesurfacey) {
  //Distribution uniforme de la position des chlamy si il n'y a pas d'obstacle

  bool contact;
  do {
    contact = false;
    x = 2*Taillesurfacex * double(drand48()) - 5*Taillesurfacex/3;
    y = (Taillesurfacey-2*Taille) * double(drand48()) + Taille;

    for (unsigned int i = 0; i < groupe.size(); i++) {
      if((norm(groupe[i].x-x, groupe[i].y-y) < groupe[i].Taille+Taille) && (norm(groupe[i].x-x, groupe[i].y-y) != 0))
	contact = true;
    }
  } while (contact == true);
}



//On initialise les vitesses initiales des oiseaux
void chlamy::vitesseinitiale(double vmoy) {
  double variancevitesse = vmoy/10, normvitesse;

  //La distribution initiale des vitesses et gaussiennes pour choisir la direction de l'oiseau aléatoirement
  v0 = rejetgaussien(vmoy, variancevitesse);
  vx = rejetgaussien(0, variancevitesse);
  vy = rejetgaussien(0, variancevitesse);
  normvitesse = norm(vx, vy);

  //On normalise la vitesse initiale à la vitesse du groupe
  vx = v0 * vx/normvitesse;
  vy = v0 * vy/normvitesse;
}




//Cette fonction permettra d'être sûr qu'aucune chlamy ne pénétrera un mur
bool chlamy::enceinte(double Tailleporte, double Angleporte, double Taillesurfacex, double Taillesurfacey) {
  
  //On a six murs dans la simulation, on calcule la distance de la chlamy à chacun de ces murs
  //On différencie néanmoins si la chlamy est à gauche ou à droite de la porte pour faciliter le calcul

  double dist1, dist2;
  if (((x <= Taillesurfacex/2) && (y < (Taillesurfacey-Tailleporte+Taille)/2)) ||  ((x <= Taillesurfacex/2) && (y > (Taillesurfacey+Tailleporte-Taille)/2))) {
    dist1 = fabs(tan(Angleporte)*(x+vx) - (y+vy) + (Taillesurfacey-Tailleporte-Taillesurfacex*tan(Angleporte))/2)/sqrt(tan(Angleporte)*tan(Angleporte)+1);
    dist2 = fabs(-tan(Angleporte)*(x+vx) - (y+vy) + (Taillesurfacey+Tailleporte+Taillesurfacex*tan(Angleporte))/2)/sqrt(tan(Angleporte)*tan(Angleporte)+1);
  }

  else if (((x > Taillesurfacex/2) && (y < (Taillesurfacey-Tailleporte)/2)) ||  ((x > Taillesurfacex/2) && (y > (Taillesurfacey+Tailleporte)/2))) {
    dist1 = fabs(-tan(Angleporte)*(x+vx) - (y+vy) + (Taillesurfacey-Tailleporte+Taillesurfacex*tan(Angleporte))/2)/sqrt(tan(Angleporte)*tan(Angleporte)+1);
    dist2 = fabs(tan(Angleporte)*(x+vx) - (y+vy) + (Taillesurfacey+Tailleporte-Taillesurfacex*tan(Angleporte))/2)/sqrt(tan(Angleporte)*tan(Angleporte)+1);
  }

  else
    return true;
  double distance = min(dist1,dist2);
  return distance >= Taille/2;
}



bool chlamy::sphere_dure(vector<chlamy> groupe) {

  bool contact = false;
  for (unsigned int i = 0; i < groupe.size(); i++) {
    //cout << (norm(groupe[i].x-x-vx, groupe[i].y-y-vy) < groupe[i].Taille+Taille) << "   " << (norm(groupe[i].x-x, groupe[i].y-y) > 0) << endl;
    if ((norm(groupe[i].x-x-vx, groupe[i].y-y-vy) < (groupe[i].Taille+Taille)/2) && (norm(groupe[i].x-x, groupe[i].y-y) > 0))
      contact = true;
  }
  return contact;
}


void chlamy::brownien_avec_collision(double Taillesurfacex, double Taillesurfacey) {

  double theta;

  //L'angle theta est légèrement modifié entre chaque itération
  theta = distribution_lorentzienne(0, 1.2);

  //v0 = v0 + rejetgaussien(0, v0/100.);
  vx = v0 * cos(theta);
  vy = v0 * sin(theta);
}



void chlamy::brownien_sans_collision(vector<chlamy> groupe, double Taillesurfacex, double Taillesurfacey, double k) {

  double theta;

  //L'angle theta est légèrement modifié entre chaque itération
  theta = distribution_lorentzienne(0, 1.2);

  //v0 = v0 + rejetgaussien(0, v0/100.);
  vx = v0 * cos(theta);
  vy = v0 * sin(theta);


  //On peut alterner cette partie de la fonction brownien avec l'utilisation de la fonction sphere_dure dans la fonction main
  for (unsigned int i= 0; i < groupe.size(); i++) {
    if ((norm(groupe[i].x-x-vx, groupe[i].y-y-vy) < (groupe[i].Taille+Taille)/2) && (norm(groupe[i].x-x, groupe[i].y-y) > 0)) {
      vx = vx - k*(groupe[i].x - x);
      vy = vy - k*(groupe[i].y - y);
    }
  }  
}
