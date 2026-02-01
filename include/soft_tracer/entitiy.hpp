
#pragma once

#include <functional>

struct Entity {
  [[nodiscard]] bool Valid() const { return id != 0; }

  bool operator<(const Entity &other) const { return id < other.id; }
  bool operator==(const Entity &other) const { return id == other.id; }

  uint32_t id = 0;
};

template <> struct std::hash<Entity> {
    std::size_t operator()(const Entity &entity) const noexcept {
        return std::hash<uint32_t>()(entity.id);
    }
};
