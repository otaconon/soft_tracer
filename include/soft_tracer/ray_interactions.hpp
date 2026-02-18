#pragma once

#include "hit_result.hpp"
#include "ray.hpp"
#include "soft_tracer/material.hpp"

bool scatter_dielectric(Ray &ray, HitResult &hit_result, Material &material);
bool scatter_lambertian(Ray &ray, HitResult &hit_result, Material &material);
bool scatter_metallic(Ray &ray, HitResult &hit_result, Material &material);
