#pragma once

#include "hit_result.hpp"
#include "ray.hpp"
#include "soft_tracer/material.hpp"

[[nodiscard]] bool scatter_dielectric(Ray &ray, const HitResult &hit_result,
                                      const Material &material);
[[nodiscard]] bool scatter_lambertian(Ray &ray, const HitResult &hit_result,
                                      const Material &material);
[[nodiscard]] bool scatter_metallic(Ray &ray, const HitResult &hit_result,
                                    const Material &material);
