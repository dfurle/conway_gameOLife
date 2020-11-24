#pragma once
#include <SFML/Graphics.hpp>
#include <math.h>


namespace sf{
  class Line {
  public:
    sf::Vertex line[4];
    sf::Vertex point[2];
    float thickness;

  public:
    Line() {
      thickness = 1;
      point[0].position = sf::Vector2f(0.f, 0.f);
      point[0].color = sf::Color::White;
      point[1].position = sf::Vector2f(0.f, 0.f);
      point[1].color = sf::Color::White;
      update();
    }
    Line(sf::Vector2f ln1, sf::Vector2f ln2) {
      thickness = 1;
      point[0].position = ln1;
      point[0].color = sf::Color::White;
      point[1].position = ln2;
      point[1].color = sf::Color::White;
      update();
    }
    Line(sf::Vector2f ln1, sf::Color c1, sf::Vector2f ln2, sf::Color c2) {
      thickness = 1;
      point[0].position = ln1;
      point[0].color = c1;
      point[1].position = ln2;
      point[1].color = c2;
      update();
    }
    void setPosition(sf::Vector2f ln1, sf::Vector2f ln2) {
      point[0].position = ln1;
      point[1].position = ln2;
      update();
    }
    void setThickess(float thick) {
      thickness = thick;
      update();
    }
    void draw(sf::RenderWindow* window){
      window->draw(line, 4, sf::Quads);
    }

  private:
    void update() {
      sf::Vector2f diff(point[0].position.x - point[1].position.x, point[0].position.y - point[1].position.y);
      sf::Vector2f df_thick, df_thick2;
      float alpha = atan2(diff.y, diff.x) + M_PI_2;
      df_thick = sf::Vector2f(cos(alpha) * thickness / 2.f, sin(alpha) * thickness / 2.f);
      line[0].position = point[1].position + df_thick;
      line[0].color = point[1].color;
      line[1].position = point[1].position - df_thick;
      line[1].color = point[1].color;
      line[2].position = point[0].position - df_thick;
      line[2].color = point[0].color;
      line[3].position = point[0].position + df_thick;
      line[3].color = point[0].color;
    }
  };
}
