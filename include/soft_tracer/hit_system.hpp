#pragma once

#include <vector>

#include "soft_tracer/entitiy.hpp"
#include "soft_tracer/hit_result.hpp"
#include "soft_tracer/ray.hpp"

template <typename C>
std::vector<HitResult> hit_entities_with(const Ray &ray, float t_min, float t_max);
