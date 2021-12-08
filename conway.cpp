#include "game.hpp"
#include <random>
#include <time.h>
#include <unistd.h>

std::string glider_text("  x\nx x\n xx");

bool Game::sim = false;


void glider(sf::Vector2i pos, std::vector<std::vector<Cell> >& boxes){
  if(pos.x+1 < boxes.size() && pos.x-1 >= 0 && pos.y-2 >= 0){
    boxes[(pos.x)%Game::vecWidth][(pos.y)%Game::vecHeight].state = true;
    boxes[(pos.x+1)%Game::vecWidth][(pos.y)%Game::vecHeight].state = true;
    boxes[(pos.x+1)%Game::vecWidth][(pos.y-1)%Game::vecHeight].state = true;
    boxes[(pos.x)%Game::vecWidth][(pos.y-2)%Game::vecHeight].state = true;
    boxes[(pos.x-1)%Game::vecWidth][(pos.y)%Game::vecHeight].state = true;
  }
}


Game::Game(): boxes(vecWidth,std::vector<Cell>(vecHeight))
{
  srand(time(NULL));
  window = new sf::RenderWindow(sf::VideoMode(WIDTH,HEIGHT),"Conway", sf::Style::Close | sf::Style::Titlebar);
  window->setFramerateLimit(60);

  for(int i = 0; i < vecWidth; i++){
    for(int j = 0; j < vecHeight; j++){
      boxes[i][j] = Cell(size,i*size,j*size);
    }
  }

  for(int i = 0; i < boxes.size(); i++){
    for(int j = 0; j < boxes[i].size(); j++){
      if(rand()/(double)RAND_MAX > chance)
        boxes[i][j].state = true;
      else
        boxes[i][j].state = false;
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
  for(int x=0;x<boxes.size();x++){
    for(int y=0;y<boxes[x].size();y++){
      int a = 0;
      if(x+1 < boxes.size())
        boxes[x+1][y].state ? a++ : a;
      if(x+1 < boxes.size() && y+1 < boxes[x].size())
        boxes[x+1][y+1].state ? a++ : a;
      if(x-1 >= 0)
        boxes[x-1][y].state ? a++ : a;
      if(x-1 >= 0 && y-1 >= 0)
        boxes[x-1][y-1].state ? a++ : a;
      if(y+1 < boxes[x].size())
        boxes[x][y+1].state ? a++ : a;
      if(y-1 >=0)
        boxes[x][y-1].state ? a++ : a;
      if(x+1 < boxes.size() && y-1 >= 0)
        boxes[x+1][y-1].state ? a++ : a;
      if(x-1 >= 0 && y+1 < boxes[x].size())
        boxes[x-1][y+1].state ? a++ : a;

      if(!boxes[x][y].state && a == 3){
        revive++;
        boxes[x][y].next_state = true;
      } else if(boxes[x][y].state && (a == 2 || a == 3)){
        alive++;
        boxes[x][y].next_state = true;
      } else {
        dead++;
        boxes[x][y].next_state = false;
      }
    }
  }
  for(int x=0;x<boxes.size();x++){
    for(int y=0;y<boxes[x].size();y++){
      boxes[x][y].state = boxes[x][y].next_state;
    }
  }
  printf("r: %d, a: %d, d: %d\n", revive, alive, dead);
}

void Game::run(){
  Button leftMB(sf::Mouse::Left);
  Key keyG(sf::Keyboard::Key::G);
  Key keySpace(sf::Keyboard::Key::Space);
  while(window->isOpen()){
    pollSFML();
    if(leftMB.poll()){
      sf::Vector2f mousePos = window->mapPixelToCoords(sf::Mouse::getPosition(*window));
      int x = 0, y = 0;
      x = mousePos.x / size;
      y = mousePos.y / size;
      if(x >= 0 && x < vecWidth && y >= 0 && y < vecHeight)
        boxes[x][y].state = !boxes[x][y].state;
    }
    if(keyG.poll()){
      sf::Vector2f mousePos = window->mapPixelToCoords(sf::Mouse::getPosition(*window));
      int x = 0, y = 0;
      x = mousePos.x / size;
      y = mousePos.y / size;
      if(x >= 0 && x < vecWidth && y >= 0 && y < vecHeight)
        glider(sf::Vector2i(x,y),boxes);
    }
    if(keySpace.poll()){
      sim = !sim;
    }
    if(sim)
      simulate();

    render();
    usleep(5e4);
  }
}

void Game::render(){
  window->clear(sf::Color::White);
  for(int x=0;x<vecWidth;x++){
    for(int y=0;y<vecHeight;y++){
      if(boxes[x][y].state)
        boxes[x][y].setFillColor(sf::Color::Red);
      else
        boxes[x][y].setFillColor(sf::Color::White);
      window->draw(boxes[x][y]);
    }
  }
  window->display();
}


int main(int argc, char* argv[]){
  Game game;
  return 0;
}