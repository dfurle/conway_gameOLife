#include "game.hpp"
#include <random>
#include <time.h>
#include <unistd.h>

typedef sf::Vector2f Vec;

int WIDTH = 1600;
int HEIGHT = 1200;
bool sim = false;
double chance = 0.95;
int size = 10;

void glider(sf::Vector2i pos, std::vector<std::vector<std::pair<sf::RectangleShape,bool> > >& boxes){
  if(pos.x+1 < boxes.size() && pos.x-1 >= 0 && pos.y-2 >= 0){
    boxes[pos.x][pos.y].second = true;
    boxes[pos.x+1][pos.y].second = true;
    boxes[pos.x+1][pos.y-1].second = true;
    boxes[pos.x][pos.y-2].second = true;
    boxes[pos.x-1][pos.y].second = true;
  }
}


Game::Game()
{
  srand(time(NULL));
  window = new sf::RenderWindow(sf::VideoMode(WIDTH,HEIGHT),"Conway", sf::Style::Close | sf::Style::Titlebar);
  window->setFramerateLimit(60);

  for(int i = 0; i < WIDTH; i+=size){
    std::vector<std::pair<sf::RectangleShape,bool> > tmp;
    for(int j = 0; j < HEIGHT; j+=size){
      sf::RectangleShape rect(sf::Vector2f(size,size));
      rect.setPosition(sf::Vector2f(i,j));
      rect.setOutlineColor(sf::Color::Black);
      rect.setOutlineThickness(1.f);
      tmp.push_back(std::pair<sf::RectangleShape,bool>(rect,false));
    }
    boxes.push_back(tmp);
  }

  for(int i = 0; i < boxes.size(); i++){
    for(int j = 0; j < boxes[i].size(); j++){
      if(rand()/(double)RAND_MAX > chance)
        boxes[i][j].second = true;
      else
        boxes[i][j].second = false;
    }
  }
  
  run();
}

void Game::pollSFML(){
  sf::Event event;
  while (window->pollEvent(event)){
    switch(event.type){
    case sf::Event::Closed:
      window->close();
      break;
    }
  }
}


void Game::simulate(){
  int revive = 0;
  int alive = 0;
  int dead = 0;
  std::vector<std::pair<std::pair<int,int>,bool> > changed;
  for(int x=0;x<boxes.size();x++){
    for(int y=0;y<boxes[x].size();y++){
      int a = 0;
      if(x+1 < boxes.size())
        boxes[x+1][y].second ? a++ : a;

      if(x+1 < boxes.size() && y+1 < boxes[x].size())
        boxes[x+1][y+1].second ? a++ : a;

      if(x-1 >= 0)
        boxes[x-1][y].second ? a++ : a;

      if(x-1 >= 0 && y-1 >= 0)
        boxes[x-1][y-1].second ? a++ : a;

      if(y+1 < boxes[x].size())
        boxes[x][y+1].second ? a++ : a;

      if(y-1 >=0)
        boxes[x][y-1].second ? a++ : a;

      if(x+1 < boxes.size() && y-1 >= 0)
        boxes[x+1][y-1].second ? a++ : a;

      if(x-1 >= 0 && y+1 < boxes[x].size())
        boxes[x-1][y+1].second ? a++ : a;


      if(!boxes[x][y].second && a == 3){
        revive++;
        changed.push_back(std::pair<std::pair<int,int>,bool>(std::pair<int,int>(x,y),true));
        // boxes[x][y].second = true;
      } else if(boxes[x][y].second && (a == 2 || a == 3)){
        alive++;
      } else {
        dead++;
        changed.push_back(std::pair<std::pair<int,int>,bool>(std::pair<int,int>(x,y),false));
        // boxes[x][y].second = false;
      }
    }
  }
  for(int i=0;i<changed.size();i++){
    boxes[changed[i].first.first][changed[i].first.second].second = changed[i].second;
  }
  printf("r: %d, a: %d, d: %d\n", revive, alive, dead);
}

void Game::run(){
  while(window->isOpen()){
    pollSFML();
    if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
      sf::Vector2f mousePos = window->mapPixelToCoords(sf::Mouse::getPosition(*window));
      for(int x=0;x<boxes.size();x++){
        for(int y=0;y<boxes[x].size();y++){
          if(boxes[x][y].first.getGlobalBounds().contains(mousePos))
            boxes[x][y].second = true;
        }
      }
    }
    if(sf::Mouse::isButtonPressed(sf::Mouse::Right)){
      sf::Vector2f mousePos = window->mapPixelToCoords(sf::Mouse::getPosition(*window));
      for(int x=0;x<boxes.size();x++){
        for(int y=0;y<boxes[x].size();y++){
          if(boxes[x][y].first.getGlobalBounds().contains(mousePos))
            boxes[x][y].second = false;
        }
      }
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::G)){
      sf::Vector2f mousePos = window->mapPixelToCoords(sf::Mouse::getPosition(*window));
      for(int x=0;x<boxes.size();x++){
        for(int y=0;y<boxes[x].size();y++){
          if(boxes[x][y].first.getGlobalBounds().contains(mousePos))
            glider(sf::Vector2i(x,y),boxes);
        }
      }
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
      sim = true;
    if(sim)
      simulate();
    

    render();
    usleep(5e4);
  }
}

void Game::render(){
  window->clear(sf::Color::White);
  for(int x=0;x<boxes.size();x++){
    for(int y=0;y<boxes[x].size();y++){
      if(boxes[x][y].second)
        boxes[x][y].first.setFillColor(sf::Color::Red);
      else
        boxes[x][y].first.setFillColor(sf::Color::White);
      window->draw(boxes[x][y].first);
    }
  }
  window->display();
}


int main(int argc, char* argv[]){
  if(argc>1){
    std::string str(argv[1]);
    if(str.find("none",1))
      chance = 1.0;

  }
  Game game;
  return 0;
}