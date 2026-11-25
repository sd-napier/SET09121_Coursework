#pragma once
#include <SFML/Window/Keyboard.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

struct Action {
  const std::string MoveLeft = "Left";
  const std::string MoveRight = "Right";
  const std::string Jump = "Up";
  const std::string Meow = "Space";
};

class KeyBindManager {

public:
  static KeyBindManager &get_instance();
  static KeyBindManager instance;

  bool loadFromFile(const std::string &filepath);
  bool writeToFile(const std::string &filepath) const;

  void setBinding(Action action, sf::Keyboard::Key key);
  sf::Keyboard::Key getBinding(Action action) const;
  bool isActionPressed(Action action) const;
  std::string getKeyName(sf::Keyboard::Key key) const;

private:
  KeyBindManager() {};
  KeyBindManager(const KeyBindManager &) = delete;
};