#include<iostream>
#include<time.h>
#include<cmath>
#include<vector>
#include<fstream>
#include<string>
#include<iomanip>
#include<SFML/Graphics.hpp>
#include"fonctions.h"
#include"chlamy.h"

using namespace std;


int main() {

  //Initialisation de la graine des processus aléatoires
  srand48(time(NULL));


  //Variables et paramètres temporels de la simulation
  int t = 0;
  int framerate = 20;

  //Import d'une police d'écriture pour l'interface SFML
  sf::Font font;
  font.loadFromFile("Roboto-Black.ttf");

  //Définition de la taille de la simulation
  double Taillesurfacex = 1600;
  double Taillesurfacey = 100;

  //Définition de la porte
  double Tailleporte = 10, Angleporte = M_PI/4;
  
  //Définition de la vitesse moyenne  des chlamys en adaptant au nombre de frame par seconde
  double vmoy = 90./framerate;

  //Initialisation du groupe de chlamy
  vector<chlamy> groupe;

  //Nombre de chlamy dans le groupe
  int N = 10000;

  //Constante de ressort de répulsion entre chlamys
  double k = 0.15;


  //On ouvre un fichier pour stocker des informations sur le programme
  fstream fich_parametre, fich_x, fich_y;


  fich_parametre.open("parametres.txt", ios::out);
  fich_parametre <<  "Taille_porte Angle_Porte Vitesse_moyenne N k"  << endl << Tailleporte << " " << Angleporte << " " << vmoy << " " << N << " " << k << endl;
  fich_parametre.close();

  cout << "ok1" << endl;
  
  fich_x.open("x.txt", ios::out);
  fich_y.open("y.txt", ios::out);
  fich_x << "frame ";
  fich_y << "frame ";
  
  for (int i = 0; i < N; N++) {
    fich_x << to_string(i) << " ";
    fich_y << to_string(i) << " ";
  }
  fich_x << endl;
  fich_y << endl;

  cout << "ok2" << endl;
  
  for(int i = 0; i < N; i++) {
    chlamy C(groupe, Taillesurfacex, Taillesurfacey, vmoy);
    groupe.push_back(C);
  }

  
  //On fait tourner le programme pendant 40000 frames
  for (unsigned int frame = 0; frame < 5000; frame++) {


    fich_x << frame << " ";
    fich_y << frame << " ";
    for (int i = 0; i < N; i++) {
      fich_x << groupe[i].x << " ";
      fich_y << groupe[i].y << " ";
    }
    fich_x << endl;
    fich_y << endl;


    t+=1;
    
    //Premier comportement des chlamys
    for (int i = 0; i < N; i++) {

      //Taux de changement de direction pour une aluge (en seconde) avec framerate 
      double alpha = 0.33;
      double p = drand48();

      //Pour représenter la persistence, on inclue une probabilité de modifier la vitesse d'une chlamy
      if (p < alpha)
	groupe[i].brownien_sans_collision(groupe, Taillesurfacex, Taillesurfacey, k);

      //Si la chlamy se dirige sur un mur, elle attend une nouvelle orientation de vitesse
      if ((groupe[i].enceinte(Tailleporte, Angleporte, Taillesurfacex, Taillesurfacey) == false)) {
	groupe[i].vx = 0;
	groupe[i].vy = 0;
      }
    }


    //Mise à jour de la position des oiseaux
    for (int i = 0; i < N; i++) {
      groupe[i].x = groupe[i].x + groupe[i].vx; //position selon x
      groupe[i].y = groupe[i].y + groupe[i].vy; //position selon y

      //condition périodique de la surface SFML selon x
      if (groupe[i].x < -2*Taillesurfacex)
	groupe[i].x += 3*Taillesurfacex;
      else if (groupe[i].x > Taillesurfacex)
	groupe[i].x -= 3*Taillesurfacex;

      //condition périodique de la surface SFML selon y
      if (groupe[i].y < 0)
	groupe[i].y += Taillesurfacey;
      else if (groupe[i].y > Taillesurfacey)
	groupe[i].y -= Taillesurfacey;
    }
  }

  fich_x.close();
  fich_y.close();

  cout << "ok3" << endl;

  return 0;
}
