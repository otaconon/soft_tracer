#pragma once

#include <limits>
#include <vector>

class IComponentArray {
public:
  virtual ~IComponentArray() = default;

  virtual bool remove_data(uint32_t entity_id) = 0;
  virtual bool clone_component(uint32_t src_entity_id,
                              uint32_t dst_entity_id) = 0;
};

template <typename T> class ComponentArray final : public IComponentArray {
public:
  [[nodiscard]] const uint32_t *get_entities() const {
    return _index_to_entity.data();
  }

  [[nodiscard]] size_t get_size() const { return _size; }

  T *get_components() { return _components.data(); }

  [[nodiscard]] bool has_data(uint32_t entity_id) const {
    return entity_id < _entity_to_index.size() &&
           _entity_to_index[entity_id] < _size;
  }

  // Returns true if insertion was successful and false otherwise
  bool insert_data(uint32_t entity_id, T &&component) {
    // Entity already has this component type
    if (has_data(entity_id)) {
      size_t index = _entity_to_index[entity_id];
      _components[index] = std::move(component);
      return true;
    }

    // Grow the vector
    size_t new_index = _size;
    if (entity_id >= _entity_to_index.size()) {
      _entity_to_index.resize(entity_id + 1, std::numeric_limits<size_t>::max());
    }

    if (new_index >= _index_to_entity.size()) {
      _index_to_entity.resize(new_index + 1);
    }

    // Insert new component at the end
    _entity_to_index[entity_id] = new_index;
    _index_to_entity[new_index] = entity_id;
    if (_components.size() > new_index) {
      _components[new_index] = std::move(component);
    } else {
      _components.push_back(std::move(component));
    }
    _size++;
    return true;
  }

  // returns true if cloning was successful and false otherwise
  bool clone_component(uint32_t src_entity_id,
                      uint32_t dst_entity_id) override {
    if (src_entity_id >= _size) {
      return false;
    }

    size_t src_index = _entity_to_index[src_entity_id];
    T component_copy = _components[src_index];

    insert_data(dst_entity_id, std::move(component_copy));
    return true;
  }

  // Returns true there was an entity to remove and false otherwise
  bool remove_data(uint32_t entity_id) override {
    if (!has_data(entity_id)) {
      return false;
    }

    const size_t removed_idx = _entity_to_index[entity_id];
    const size_t last_idx = _size - 1;

    if (removed_idx != last_idx) {
      _components[removed_idx] = std::move(_components[last_idx]);
      uint32_t last_entity = _index_to_entity[last_idx];
      _entity_to_index[last_entity] = removed_idx;
      _index_to_entity[removed_idx] = last_entity;
    }

    _entity_to_index[entity_id] = std::numeric_limits<size_t>::max();
    _size--;
    return true;
  }

  // Returns nullptr if there is no such entity
  T *get_data(uint32_t entity_id) {
    if (!has_data(entity_id)) {
      return nullptr;
    }

    return &_components[_entity_to_index[entity_id]];
  }

private:
  std::vector<T> _components{};
  // Used for accessing components given entity id
  std::vector<size_t> _entity_to_index;
  // Used for keeping components vector dense
  std::vector<std::uint32_t> _index_to_entity;
  size_t _size = 0;
};
