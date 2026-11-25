#pragma once
#include <SFML/Window/Keyboard.hpp>
#include <map>
#include <string>

// struct with default keybindings, which will only be used if the keybinds.txt
// file is empty
struct Action {
  const std::string MoveLeft = "Left";
  const std::string MoveRight = "Right";
  const std::string Jump = "Up";
  const std::string Meow = "Space";
};

class KeyBindManager {

public:
  static KeyBindManager &get_instance() {
    static KeyBindManager instance;
    return instance;
  }

  KeyBindManager(const KeyBindManager &) = delete;
  bool loadFromFile(const std::string &filepath);
  bool writeToFile(const std::string &filepath) const;
  void setBinding(const std::string &action, sf::Keyboard::Key key);
  sf::Keyboard::Key getBinding(const std::string &action) const;
  bool isActionPressed(const std::string &action) const;
  std::string getKeyName(sf::Keyboard::Key key) const;
  std::string filepath = "resources/keys/keybinds.txt";

private:
  KeyBindManager() {};

  // A map to store the key bindings, will be needed as the
  std::map<std::string, sf::Keyboard::Key> _bindings;
};