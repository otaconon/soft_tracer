#pragma once

#include "component_array.hpp"
#include "entitiy.hpp"

#include <memory>
#include <optional>
#include <ranges>
#include <typeindex>
#include <unordered_map>
#include <unordered_set>

class EntityManager {
public:
  EntityManager() { _singleton_entity = create_entity(); }

  void clear() {
    std::vector<std::uint32_t> ids;
    ids.reserve(_entities.size() + _prototype_entities.size());
    ids.insert(ids.end(), _entities.begin(), _entities.end());
    ids.insert(ids.end(), _prototype_entities.begin(),
               _prototype_entities.end());

    if (_singleton_entity && !_entities.contains(_singleton_entity->id) &&
        !_prototype_entities.contains(_singleton_entity->id)) {
      ids.push_back(_singleton_entity->id);
    }

    for (const std::uint32_t id : ids) {
      remove_entity(Entity{id});
    }

    _component_arrays.clear();

    _entities.clear();
    _prototype_entities.clear();
    _entity_components.clear();

    _singleton_entity.reset();
    _next_entity_id = 1;
  }

  Entity create_entity() {
    const Entity entity{_next_entity_id++};
    _entities.insert(entity.id);
    return entity;
  }

  Entity create_prototype_entity() {
    const Entity entity{_next_entity_id++};
    _prototype_entities.insert(entity.id);
    return entity;
  }

  Entity clone(const Entity& src_entity) {
    const Entity dst_entity = create_entity();

    for (auto &componentType : _entity_components[src_entity.id]) {
      auto it = _component_arrays.find(componentType);
      if (it == _component_arrays.end()) {
        continue;
      }

      it->second->clone_component(src_entity.id, dst_entity.id);

      _entity_components[dst_entity.id].insert(componentType);
    }

    return dst_entity;
  }

  template <typename... Ts>
  void add_components(const Entity& entity, Ts &&...components) {
    (add_component<std::decay_t<Ts>>(entity, std::forward<Ts>(components)), ...);
  }

  template <typename... Ts> void remove_components(const Entity& entity) {
    (remove_component<std::decay_t<Ts>>(entity), ...);
  }

  template <typename T> void add_singleton_component(T &&component) {
    add_component<std::decay_t<T>>(_singleton_entity.value(),
                                  std::forward<T>(component));
  }

  void remove_entity(const Entity& entity) {
    // it's ok to just call the erases, if the entity doesn't exist, nothing
    // will happen
    _entities.erase(entity.id);
    _prototype_entities.erase(entity.id);
    _entity_components.erase(entity.id);
    for (const auto &arr : _component_arrays | std::views::values) {
      arr->remove_data(entity.id);
    }
  }

  template <typename Driver, typename... Rest, typename F> void each(F &&f) {
    auto &driver_arr = get_component_array<Driver>();

    const std::size_t count = driver_arr.get_size();
    const auto *entities = driver_arr.get_entities();
    Driver *comps0 = driver_arr.get_components();

    for (std::size_t i = 0; i < count; ++i) {
      Entity e{entities[i]};

      if (!(has_component<Rest>(e) && ...)) {
        continue;
      }

      f(e, comps0[i], *get_component<Rest>(e)...);
    }
  }

  template <typename T> ComponentArray<T> &get_component_array() {
    auto &base_ptr = _component_arrays[typeid(T)];
    if (!base_ptr) {
      base_ptr = std::make_unique<ComponentArray<T>>();
    }

    return static_cast<ComponentArray<T> &>(*base_ptr);
  }

  // Returns pointer to the component if entity has it, otherwise returns
  // nullptr
  template <typename T> T *get_component(const Entity& entity) {
    return get_component_array<T>().get_data(entity.id);
  }

  // Returns pointer to the singleton component if entity has it, otherwise
  // returns nullptr
  template <typename T> T *get_singleton_component() {
    return get_component<T>(_singleton_entity.value());
  }

  template <typename... Ts> bool has_components(const Entity& entity) {
    return (has_components<Ts>(entity) && ...);
  }

private:
  template <typename T> void add_component(const Entity& entity, T &&component) {
    using DT = std::decay_t<T>;
    get_component_array<DT>().insert_data(entity.id, std::forward<T>(component));
    _entity_components[entity.id].insert(std::type_index(typeid(DT)));
  }

  template <typename T> void remove_component(const Entity& entity) {
    using DT = std::decay_t<T>;
    get_component_array<DT>().remove_data(entity.id);
  }

  template <typename T> bool has_component(const Entity& entity) {
    return get_component_array<T>().has_data(entity.id);
  }

  std::uint32_t _next_entity_id = 1;
  std::unordered_set<std::uint32_t> _entities;
  // prototype entities are meant to not be updated by systems
  std::unordered_set<std::uint32_t> _prototype_entities;
  std::unordered_map<std::type_index, std::unique_ptr<IComponentArray>>
      _component_arrays;
  std::unordered_map<std::uint32_t, std::unordered_set<std::type_index>>
      _entity_components;

  std::optional<Entity> _singleton_entity;
};
