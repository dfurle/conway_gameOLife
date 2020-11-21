#include <SFML/Graphics.hpp>
#include "Line.hpp"

class Game{
public:
  sf::RenderWindow* window;
  std::vector<std::vector<std::pair<sf::RectangleShape,bool> > > boxes;

public:

  Game();

  void pollSFML();
  void run();
  void simulate();
  void render();

};