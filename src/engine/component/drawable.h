#ifndef _ENGINE_COMPONENTS_DRAWABLE_H_
#define _ENGINE_COMPONENTS_DRAWABLE_H_

#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#include "engine/component/component.h"
#include "engine/utility/texture_loader.h"

namespace engine {
namespace component {

// Drawable objects have an associated image (which can either be a full file or
// a subsection within a file). When you create the component, you must specify
// the sprite parameters.
class Drawable : public Component {
public:
  // Construct a new drawable component.
  Drawable(const std::string& sprite_filename);
  Drawable(const std::string& sprite_filename, sf::Vector2u offset,
           sf::Vector2u size=sf::Vector2u());

  // Draw this sprite onto the screen at its current location.
  virtual void Draw(sf::RenderWindow& window) const;

  // Get access to the internal sprite.
  sf::Sprite& Sprite();

protected:
  sf::Sprite sprite_;
};

}}  // namepsace engine::component

#endif  // _ENGINE_COMPONENTS_DRAWABLE_H_