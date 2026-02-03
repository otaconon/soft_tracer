#pragma once

#include <vector>

#include "soft_tracer/hit_result.hpp"
#include "soft_tracer/ray.hpp"
#include "soft_tracer/interval.hpp"

template <typename C>
HitResult hit_entities_with(const Ray &ray, const Interval& t_intervavl);
