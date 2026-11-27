#pragma once
#include "ecm.hpp"
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

class Scene; // forward definition

/*

*/
class GameSystem {
public:
  static void start(unsigned int width, unsigned int height,
                    const std::string &name, const float &time_step,
                    bool physics_enabled = false);
  static void clean();
  static void reset();
  static void set_active_scene(const std::shared_ptr<Scene> &act_sc);
  static std::shared_ptr<Scene> &get_active_scene() { return _active_scene; }

  //
private:
  static void _init();
  static void _update(const float &dt);
  static void _render();
  static std::shared_ptr<Scene> _active_scene;
  static bool _physics_enabled;
};

/*
 * Modified the Scene class to have an event handler overried, necessary to
 * ensure the new window for keyboard settings menu on the loading menu has
 * events to capture keypresses in that outside of the main settings menu
 *
 */
class Scene {
public:
  Scene() = default;

  virtual ~Scene() = default;
  virtual void update(const float &dt);
  virtual void render();
  virtual void load() = 0;
  virtual void unload();
  virtual void handleEvent(const sf::Event &event) {}

  const std::shared_ptr<Entity> &make_entity();

  std::vector<std::shared_ptr<Entity>> &get_entities() {
    return _entities.list;
  }

protected:
  EntityManager _entities;
};
