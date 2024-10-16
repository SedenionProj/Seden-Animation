# stb
set (STB_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/stb PARENT_SCOPE)

# glfw
set (GLFW_LIBRARY_TYPE STATIC CACHE STRING "" FORCE)
set (GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set (GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set (GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set (GLFW_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/glfw/include PARENT_SCOPE)
add_subdirectory (glfw)

# glad
set (GLAD_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/glad/include PARENT_SCOPE)
add_subdirectory(glad)

# glm
set (GLM_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/glm PARENT_SCOPE)
add_subdirectory(glm)

# imgui
set (IMGUI_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/imgui PARENT_SCOPE)
file(GLOB IMGUI_SOURCE
   imgui/*.cpp
   imgui/backends/imgui_impl_glfw.h
   imgui/backends/imgui_impl_glfw.cpp
   imgui/backends/imgui_impl_opengl3.h
   imgui/backends/imgui_impl_opengl3.cpp
   imgui/backends/imgui_impl_opengl3_loader.cpp
)

add_library(imgui STATIC ${IMGUI_SOURCE})
target_include_directories(imgui PUBLIC imgui)
target_link_libraries(imgui PRIVATE glfw)

# imguizmo
set (IMGUIZMO_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/imguizmo PARENT_SCOPE)
file(GLOB IMGUIZMO_SOURCE
	imguizmo/*.cpp
)
add_library(imguizmo STATIC ${IMGUIZMO_SOURCE})
target_include_directories(imguizmo PUBLIC imgui)