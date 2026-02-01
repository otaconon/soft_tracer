#pragma once

#include "soft_tracer/entity_manager.hpp"

class S_EntityManager {
public:
  static EntityManager &get_instance() {
    static EntityManager instance;
    return instance;
  }

  S_EntityManager(const S_EntityManager &) = delete;
  void operator=(const S_EntityManager &) = delete;

private:
  S_EntityManager() {}
  ~S_EntityManager() {}
};
