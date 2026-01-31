file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_library(onon_soft_tracer STATIC ${SOURCES})
add_library(onon::soft_tracer ALIAS onon_soft_tracer)

target_include_directories(onon_soft_tracer PUBLIC ${CMAKE_SOURCE_DIR}/include/)
target_link_libraries(onon_soft_tracer PUBLIC glm::glm SDL3::SDL3)

target_include_directories(onon_soft_tracer PUBLIC
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
