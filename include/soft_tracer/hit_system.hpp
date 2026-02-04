#pragma once


#include "soft_tracer/hit_result.hpp"
#include "soft_tracer/ray.hpp"
#include "soft_tracer/interval.hpp"


template <typename C>
std::vector<HitResult> hit_entities_with(const Interval& t_intervavl);
