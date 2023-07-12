#ifndef chlamy_H
#define chlamy_H

#include<ostream>
#include<vector>

using namespace std;

class porte;
class chlamy {
public:

  //Position et vitesse de la chlamy
  double x;
  double y;
  double vx;
  double vy;

  //Autres propriétés de la chlamy
  double v0; //vitesse moyenne de la chlamy
  double Taille; //taille de la chlamy

  //Constructeurs et destructeur de la classe
  chlamy();
  chlamy(vector<chlamy> groupe, double Taillesurfacex, double Taillesurfacey, double vmoy);
  ~chlamy();

  //Premier comportement des chlamys
    bool enceinte(double Tailleporte, double Angleporte, double Taillesurfacex, double Taillesurfacey);
  bool sphere_dure(vector<chlamy> groupe);
  void brownien_avec_collision(double Taillesurfacex, double Taillesurfacey);
  void brownien_sans_collision(vector<chlamy> groupe, double Taillesurfacex, double Taillesurfacey, double k);

  //Deuxième comportement des chlamys

  //Génération de la première frame
  void positioninitiale(vector<chlamy> groupe, double Taillesurfacex, double Taillesurfacey);
  void vitesseinitiale(double vmoy);
  
  
};

#endif
