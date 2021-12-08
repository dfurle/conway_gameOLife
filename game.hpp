#include <SFML/Graphics.hpp>
#include "Line.hpp"


class Cell: public sf::RectangleShape{
public:
  bool state;
  bool next_state;
  Cell(){
    state=false;
    next_state=false;
    setOutlineThickness(1.f);
    setOutlineColor(sf::Color::Black);
  }
  Cell(double size, double i, double j): sf::RectangleShape(sf::Vector2f(size,size)){
    state=false;
    next_state=false;
    setPosition(sf::Vector2f(i,j));
    setOutlineThickness(1.f);
    setOutlineColor(sf::Color::Black);
  }
};

class Key{
public:
  bool keySwitch = true;
  sf::Keyboard::Key key;
  Key(sf::Keyboard::Key _key){
    this->key = _key;
  }
  bool poll(){
    if(sf::Keyboard::isKeyPressed(this->key)){
      if(keySwitch){
        this->keySwitch = false;
        return true;
      } 
    } else {
      this->keySwitch = true;
    }
    return false;
  }
};

class Button{
public:
  bool keySwitch = true;
  sf::Mouse::Button key;
  Button(sf::Mouse::Button _key){
    this->key = _key;
  }
  bool poll(){
    if(sf::Mouse::isButtonPressed(this->key)){
      if(keySwitch){
        this->keySwitch = false;
        return true;
      }
    } else {
      this->keySwitch = true;
    }
    return false;
  }
};


class Game{
public:

  static const int WIDTH = 1600;
  static const int HEIGHT = 1200;
  static const int size = 10;
  static const int vecWidth = WIDTH/size;
  static const int vecHeight = HEIGHT/size;
  static bool sim;
  static const double chance = 0.95;

  sf::RenderWindow* window;
  std::vector<std::vector<Cell> > boxes;

public:

  Game();

  void pollSFML();
  void run();
  void simulate();
  void render();

};