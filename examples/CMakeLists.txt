add_executable(three_spheres_example three_spheres.cpp)
target_link_libraries(three_spheres_example PRIVATE onon::soft_tracer SDL3::SDL3)

add_executable(random_spheres_example random_spheres.cpp)
target_link_libraries(random_spheres_example PRIVATE onon::soft_tracer SDL3::SDL3)
