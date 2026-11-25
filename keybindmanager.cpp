#include "keybindmanager.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <fstream>
#include <iostream>
#include <string>

bool KeyBindManager::loadFromFile(const std::string &filepath) {
  std::ifstream keybindsOpener;
  keybindsOpener.open(filepath);
  if (!keybindsOpener.is_open()) {
    std::cerr << "Failed to open the keybinds file: " << filepath << "\n";
    return false;
  }

  _bindings.clear();

  std::string action;
  int keyCode;

  /*Reads the action word and the key number directly, but uses static_cast
   *so that the compiler doesn't throw an error. It basically converts
   *it into a string that can be used later on instead of an integer
   */
  while (keybindsOpener >> action >> keyCode) {
    // Uses index of action (key), and then assigns the keycode value
    _bindings[action] = static_cast<sf::Keyboard::Key>(keyCode);
  }

  return true;
}

/* This saves all bindings back to keybinds.txt using std::ofstream
 * Initially wouldn't work until I found out that using a const at the end
 * was needed to protect the KeyBindManager, and read from _bindings only as
opposed to modify it
*/

bool KeyBindManager::writeToFile(const std::string &filepath) const {
  std::ofstream keybindsSaver(filepath);
  // Returns false and shows an error if keybinds.txt can't be opened
  if (!keybindsSaver.is_open()) {
    std::cerr << "Failed to open file for writing: " << filepath << "\n";
    return false;
  }

  /* Loop through every pair in the map and write "Action KeyCode"
   * Note the use of "pair" which is where std::map stores everything as
   * key/value pairs. "first" reads the key, "second" reads the value
   * Also the casting of int really stumped me, as it would not compile
   * because the values are sf::Keyboard::Key enums??
   * I tried dynamic_cast which didn't work, but static_cast does
   */
  for (const auto &pair : _bindings) {
    keybindsSaver << pair.first << " " << static_cast<int>(pair.second) << "\n";
  }

  return true;
}

// 2. Updates (or adds) a keybind in the map
void KeyBindManager::setBinding(const std::string &action,
                                sf::Keyboard::Key key) {
  _bindings[action] = key;
}

// 3. Looks up which key is currently bound to an action
sf::Keyboard::Key KeyBindManager::getBinding(const std::string &action) const {
  auto it = _bindings.find(action);

  // If the action exists in the map, return its key
  if (it != _bindings.end()) {
    return it->second;
  }

  return sf::Keyboard::Unknown;
}

// Checks if the key bound to an action is currently being held down, using SFML
// sf::Keyboard::Key
bool KeyBindManager::isActionPressed(const std::string &action) const {
  sf::Keyboard::Key key = getBinding(action);

  if (key != sf::Keyboard::Unknown) {
    return sf::Keyboard::isKeyPressed(key);
  }

  return false;
}

// So this part ensures that, for common keys used in games, the key names
// appear on-screen/in a UI for easier use
std::string KeyBindManager::getKeyName(sf::Keyboard::Key key) const {
  switch (key) {
  case sf::Keyboard::Left:
    return "Left";
  case sf::Keyboard::Right:
    return "Right";
  case sf::Keyboard::Up:
    return "Up";
  case sf::Keyboard::Down:
    return "Down";
  case sf::Keyboard::Space:
    return "Space";
  case sf::Keyboard::Enter:
    return "Enter";
  case sf::Keyboard::A:
    return "A";
  case sf::Keyboard::D:
    return "D";
  case sf::Keyboard::S:
    return "S";
  case sf::Keyboard::W:
    return "W";
  default:
    return "Unknown";
  }
}
