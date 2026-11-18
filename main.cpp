#include "engine/physics.hpp"
#include "game_parameters.hpp"
#include "scenes.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

//Creates variable of the struct Parameters in file game_parameters.hpp
using param = Parameters;

int main() {

  //initialises physics declared in physics.hpp
  Physics::initialise();

  //creates and loads the manu and game starting with the menu so that they are preloaded and able to be loaded with ease
  Scenes::menu = std::make_shared<MenuScene>();
  Scenes::menu->load();
  Scenes::level = std::make_shared<LevelScene>();
  Scenes::level->load();

  //sets current screen to menu
  GameSystem::set_active_scene(Scenes::menu);

  //Opens the game window and runs with the set parameters
  GameSystem::start(param::game_width, param::game_height, "Platformer", Physics::time_step, true);

  //deloads the box2d 
  Physics::shutdown();
  return 0;
}
