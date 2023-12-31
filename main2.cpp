#include<iostream>
#include<time.h>
#include<cmath>
#include<vector>
#include<fstream>
#include<string>
#include<iomanip>
#include<SFML/Graphics.hpp>
#include"fonctions.h"
#include"chlamy2.h"

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
  double Tailleporte =10, Angleporte = M_PI/4;
  
  //Définition de la vitesse moyenne  des chlamys en adaptant au nombre de frame par seconde
  double vmoy = 90./framerate;

  //Initialisation du groupe de chlamy
  vector<chlamy> groupe;

  //Nombre de chlamy dans le groupe
  int N = 500;

  //Constante de ressort de répulsion entre chlamys
  double k = 0.2;

  
  for(int i = 0; i < N; i++) {
    chlamy C(groupe, Taillesurfacex, Taillesurfacey, vmoy);
    groupe.push_back(C);
  }

  //Ouverture de la fenêtre SFML
  sf::RenderWindow window(sf::VideoMode(Taillesurfacex, Taillesurfacey), "Simulation numerique", sf::Style::Default);

  
  //On fait tourner le programme jusqu'à ce que la fenêtre soit fermée
  while (window.isOpen()) {


    t+=1;


    //On inspecte tous les évènements de la fenêtre qui ont été émis depuis la précédente itération
    sf::Event event;
    while (window.pollEvent(event)) {

      //Evènement "fermeture demandée" : on ferme la fenêtre en appuyant sur close
      if (event.type == sf::Event::Closed)
	window.close();
    }

    //Effacement de la fenêtre en noir
    window.clear(sf::Color(200,200,200));

    //1ere boucle sur toutes les chlamys
    for (int i = 0; i < N; i++) {

      // création d'un cercle pour dessiner les chlamys
      sf::CircleShape chlamydomonas(groupe[i].Taille/2);
      chlamydomonas.setFillColor(sf::Color(100,100,100,200));
      chlamydomonas.setPosition(groupe[i].x-groupe[i].Taille/2,groupe[i].y-groupe[i].Taille/2);
      window.draw(chlamydomonas);
    }


    //On dessine la porte
    
    //Partie haute de la porte
    sf::VertexArray porte1(sf::Triangles, 3);
    porte1[0].position = sf::Vector2f(Taillesurfacex/2-(Taillesurfacey-Tailleporte)/2*tan(Angleporte), 0);
    porte1[1].position = sf::Vector2f(Taillesurfacex/2, (Taillesurfacey-Tailleporte)/2);
    porte1[2].position = sf::Vector2f(Taillesurfacex/2+(Taillesurfacey-Tailleporte)/2*tan(Angleporte), 0);

    //Partie basse de la porte
    sf::VertexArray porte2(sf::Triangles, 3);
    porte2[0].position = sf::Vector2f(Taillesurfacex/2-(Taillesurfacey-Tailleporte)/2*tan(Angleporte), Taillesurfacey);
    porte2[1].position = sf::Vector2f(Taillesurfacex/2, (Taillesurfacey+Tailleporte)/2);
    porte2[2].position = sf::Vector2f(Taillesurfacex/2+(Taillesurfacey-Tailleporte)/2*tan(Angleporte), Taillesurfacey);


    porte1[0].color = sf::Color(0,0,0);
    porte1[1].color = sf::Color(0,0,0);
    porte1[2].color = sf::Color(0,0,0);
    porte2[0].color = sf::Color(0,0,0);
    porte2[1].color = sf::Color(0,0,0);
    porte2[2].color = sf::Color(0,0,0);
    
    window.draw(porte1);
    window.draw(porte2);


    //Affichage du texte sur la fenêtre SFML
    sf::Text text;  //déclaration de la variable texte
    ostringstream s;   //déclaration du flux à afficher
    s << "t = " << t/framerate << endl;  //on remplit le flux
    text.setFont(font);  //on définit la police du texte
    text.setString(s.str());   //on remplit le texte avec le flux convertit en string
    text.setCharacterSize(10);
    text.setPosition(1300.f,80.f);
    text.setColor(sf::Color(0,0,0));
    window.draw(text);  
      

    //synchronisation des FPS SFML avec l'écran d'ordinateur
    window.setFramerateLimit(framerate);
    // fin de la frame courante, affichage de tout ce qu'on a dessiné
    window.display();


    
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
	// || (groupe[i].sphere_dure(groupe) == true)
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

  return 0;
}
