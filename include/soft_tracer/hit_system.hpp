#pragma once

#include "hit_result.hpp"
#include "interval.hpp"
#include "ray.hpp"

template <typename C>
HitResult hit_entities_with(const Ray &ray, const Interval &t_intervavl);
