#include "scenes.hpp"
#include "game_parameters.hpp"
#include "graphics_cmps.hpp"
#include "keybindmanager.hpp"
#include "physics_cmps.hpp"
#include "renderer.hpp"
#include "tile_level_loader/level_system.hpp"
#include <iostream>
#include <memory>

using ls = LevelSystem;
using param = Parameters;
using gs = GameSystem;

std::shared_ptr<Scene> Scenes::menu;
std::shared_ptr<Scene> Scenes::level;
std::shared_ptr<Scene> Scenes::settings;

// Instantiate the key bind manager here, note that a reference is used because
// otherwise a copy of the object would be created
KeyBindManager &settingsKeyBind = KeyBindManager::get_instance();

void MenuScene::update(const float &dt) {
  // Responds to user key presses that match type "Space" and passes float &dt.
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
    gs::set_active_scene(Scenes::level);
  } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::K)) {
    gs::set_active_scene(Scenes::settings);
  }
  Scene::update(dt);
}

void MenuScene::render() {
  //
  Renderer::queue(&_text);
  Scene::render();
}

void SettingsScene::update(const float &dt) { Scene::update(dt); }

void SettingsScene::handleEvent(const sf::Event &event) {
  if (event.type == sf::Event::KeyPressed) {
    if (event.key.code == sf::Keyboard::Down) {
      menuItems[selectedIndex].setFillColor(sf::Color::White);
      selectedIndex++;
      if (selectedIndex >= menuItems.size()) {
        selectedIndex = 0;
      }
      menuItems[selectedIndex].setFillColor(sf::Color::Yellow);
    } else if (event.key.code == sf::Keyboard::Up) {
      menuItems[selectedIndex].setFillColor(sf::Color::White);
      selectedIndex--;
      if (selectedIndex < 0) {
        selectedIndex = menuItems.size() - 1;
      }
      menuItems[selectedIndex].setFillColor(sf::Color::Yellow);
    } else if (event.key.code == sf::Keyboard::Escape) {
      gs::set_active_scene(Scenes::menu);
    } else if (event.key.code == sf::Keyboard::Enter) {
      switch (selectedIndex) {
      case 0:
        // Change MoveLeft key
        break;
      case 1:
        // Change MoveRight key
        break;
      case 2:
        // Change Jump key
        break;
      case 3:
        // Change Meow key
        break;
      }
    }
  }
}

void SettingsScene::render() {
  // Draw the heading
  Renderer::queue(&_settingsText);

  // Draw each menu option
  for (auto &item : menuItems) {
    Renderer::queue(&item);
  }

  // Render the rest of the scene
  Scene::render();
}

void MenuScene::load() {
  //
  _font.loadFromFile("resources/fonts/PixelGameFont.ttf");
  _text.setFont(_font);
  _text.setCharacterSize(60);
  _text.setString(
      "Welcome to Koal the Kat!\n\nPress Space to start\n\nPress Esc "
      "to close\n\nPress K to change Keyboard settings");
}

void SettingsScene::load() {
  // Load the font
  _settingsFont.loadFromFile("resources/fonts/PixelGameFont.ttf");
  // Set up the heading
  _settingsText.setFont(_settingsFont);
  _settingsText.setCharacterSize(60);
  _settingsText.setString("You can change keyboard settings here:\n");
  _settingsText.setPosition(100.f, 40.f);
  // Load keyboard bindings
  settingsKeyBind.loadFromFile(settingsKeyBind.filepath);
  // Menu options
  std::vector<std::string> options = {"MoveLeft", "MoveRight", "Jump", "Meow"};
  // Clear the existing menu items
  menuItems.clear();
  // Start with the first option selected
  selectedIndex = 0;
  // Create each menu item
  for (int i = 0; i < options.size(); i++) {
    sf::Text text;
    text.setFont(_settingsFont);
    text.setString(options[i]);
    text.setCharacterSize(40);
    text.setPosition(340.f, 140.f + i * 70.f);
    if (i == 0) {
      text.setFillColor(sf::Color::Yellow);
    } else {
      text.setFillColor(sf::Color::White);
    }
    menuItems.push_back(text);
  }
}

void SettingsScene::unload() { Scene::unload(); }

void MenuScene::unload() {}

void LevelScene::load() { _load_level(param::level_1); }

void LevelScene::_load_level(const std::string &file_path) {
  //
  ls::load_level(file_path, param::tile_size);
  _player = make_entity();
  _player->set_position(ls::get_start_position());

  //
  std::shared_ptr<ShapeComponent> shape =
      _player->add_component<ShapeComponent>();
  shape->set_shape<sf::RectangleShape>(
      sf::Vector2f(param::player_size[0], param::player_size[1]));
  shape->get_shape().setFillColor(sf::Color::Yellow);
  shape->get_shape().setOrigin(
      sf::Vector2f(param::player_size[0] / 2.f, param::player_size[1] / 2.f));

  //
  std::shared_ptr<PlayerPhysicsComponent> cmp =
      _player->add_component<PlayerPhysicsComponent>(
          sf::Vector2f(param::player_size[0], param::player_size[1]));

  //
  cmp->create_capsule_shape(
      sf::Vector2f(param::player_size[0], param::player_size[1]),
      param::player_weight, param::player_friction, param::player_restitution);

  // Gets the wall items that are grouped together, of type WALL
  std::vector<std::vector<sf::Vector2i>> wall_groups = ls::get_groups(ls::WALL);

  //
  for (const std::vector<sf::Vector2i> &walls : wall_groups) {
    _walls.push_back(make_entity());
    _walls.back()->add_component<PlatformComponent>(walls);
  }
}

// This loads the level, unless it is the end level/level 2, then it unloads the
// level
void LevelScene::update(const float &dt) {
  //
  Scene::update(dt);
  _entities.update(dt);
  if (ls::get_tile_at(_player->get_position()) == ls::END) {
    unload();
    _load_level(param::level_2);
  }
}

// This renders the scene and entities
void LevelScene::render() {
  //
  ls::render(Renderer::get_window());
  Scene::render();
  _entities.render();
}

void LevelScene::unload() {
  // Unload level geometry and clean up the player.
  Scene::unload();
  _player.reset();
  _walls.clear();
}
