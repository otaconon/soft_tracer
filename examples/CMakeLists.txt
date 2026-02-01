add_executable(onon_soft_tracer_examples simple_sphere.cpp)

target_link_libraries(onon_soft_tracer_examples PRIVATE onon::soft_tracer SDL3::SDL3)