#pragma once
#include "engine/game_system.hpp"
#include "game_parameters.hpp"
#include "graphics_cmps.hpp"
#include "physics_cmps.hpp"
#include "renderer.hpp"
#include "tile_level_loader/level_system.hpp"
#include <iostream>
#include <memory>
#include <string>
#include <vector>

/*

*/
class MenuScene : public Scene {
public:
  MenuScene() = default;
  void update(const float &dt) override;
  void render() override;
  void load() override;
  void unload() override;
  void handleEvent(const sf::Event &event) override;

private:
  std::shared_ptr<Entity> _player;
  sf::Text _text;
  sf::Font _font;
};

class SettingsScene : public Scene {
public:
  void update(const float &dt) override;
  void render() override;
  void load() override;
  void unload() override;
  void handleEvent(const sf::Event &event) override;

private:
  std::shared_ptr<Entity> _player;
  sf::Text _settingsText;
  sf::Font _settingsFont;
  std::vector<sf::Text> menuItems;
  int selectedIndex = 0;
};

/*

*/
class LevelScene : public Scene {
public:
  LevelScene() = default;
  void update(const float &dt) override;
  void render() override;
  void load() override;
  void unload() override;

private:
  std::shared_ptr<Entity> _player;
  std::vector<std::shared_ptr<Entity>> _walls;
  void _load_level(const std::string &file_path);
};

struct Scenes {
  //
  static std::shared_ptr<Scene> menu;
  static std::shared_ptr<Scene> level;
  static std::shared_ptr<Scene> settings;
};