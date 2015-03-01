#include "movable.h"

namespace engine {
namespace component {

Movable::Movable() : velocity_(), speed_(0.0), speed_multiplier_(1.0)
                   , clock_() {

}

Movable::~Movable() {

}

void Movable::SetParameter(const std::string& key, const Json::Value& value) {
  if (key == "speed") {
    speed(value.asDouble());
  }

  else if (key == "speed_multiplier") {
    speed_multiplier(value.asDouble());
  }
}

void Movable::Update(const thor::ActionMap<std::string>& map) {
  LOG->trace("Movable::Update");

  // Only run this every x ms.
  if (clock_.getElapsedTime().asMilliseconds() < kUpdateFrequencyMs) {
    LOG->trace("Done (early) Movable::Update");
    return;
  }

  // The object _must_ be drawable.
  if (not entity_->HasComponent<Drawable>()) {
    LOG->emerg("Movable entity does not have required Drawable component.");
  }

  // Update the current position of the object.
  Drawable& d = entity_->GetComponent<Drawable>();
  sf::Vector2f old_position = d.sprite().getPosition();
  d.sprite().setPosition(
      d.sprite().getPosition().x + (velocity_.x * speed_ * speed_multiplier_),
      d.sprite().getPosition().y - (velocity_.y * speed_ * speed_multiplier_));

  // If the sprite's new position interferes with anything that blocks movement,
  // don't actually move.
  for (const Entity* entity :
        game::Map::GetActive().GetEntitiesWithComponent<BlockMovement>()) {
    sf::FloatRect other_boundary;
    if (entity->HasComponent<Drawable>()) {
      other_boundary = entity->GetComponent<Drawable>().HitBox();
    } else if (entity->HasComponent<Zone>()) {
      other_boundary = entity->GetComponent<Zone>().area();
    } else {
      LOG->emerg("Something blocks movement but isn't a Zone or Drawable!");
    }

    // Add some padding around the movement-blocking object. It makes the
    // controls a little less likely to get stuck.
    other_boundary.left -= 4;
    other_boundary.top -= 4;
    other_boundary.width += 8;
    other_boundary.height += 8;
    if (other_boundary.intersects(d.HitBox())) {
      LOG->debug("Collision detected! Not moving the sprite.");
      d.sprite().setPosition(old_position);
      break;
    }
  }

  // If the object is directional (optional), change the direction.
  entity_->HasComponent<Directional>();
  if (entity_->HasComponent<Directional>()) {
    Directional& directional = entity_->GetComponent<Directional>();
    if (velocity_.x < 0) {
      directional.ChangeDirection(Directional::LEFT);
    } else if (velocity_.x > 0) {
      directional.ChangeDirection(Directional::RIGHT);
    } else if (velocity_.y > 0) {
      directional.ChangeDirection(Directional::UP);
    } else if (velocity_.y < 0) {
      directional.ChangeDirection(Directional::DOWN);
    }
  }

  clock_.restart();
  LOG->trace("Done Movable::Update");
}

Movable* Movable::speed(double speed) {
  speed_ = speed;
  return this;
}

Movable* Movable::speed_multiplier(double speed_multiplier) {
  speed_multiplier_ = speed_multiplier;
  return this;
}

sf::Vector2i& Movable::velocity() {
  return velocity_;
}

const sf::Vector2i& Movable::velocity() const {
  return velocity_;
}

double Movable::speed() const {
  return speed_;
}

double Movable::speed_multiplier() const {
  return speed_multiplier_;
}

}}  // namepsace engine::component
