cmake_minimum_required(VERSION 3.7)
project(Seden)
set (CMAKE_CXX_STANDARD 20)

find_package(OpenGL REQUIRED)

add_subdirectory (${CMAKE_CURRENT_SOURCE_DIR}/Seden/vendor)

file(GLOB_RECURSE SEDEN_SOURCE
	Seden/src/*.cpp
)

add_executable (seden_exe ${SEDEN_SOURCE})

target_include_directories (seden_exe PUBLIC
	Seden
)

target_include_directories (seden_exe PUBLIC
	${STB_INCLUDE}
	${GLFW_INCLUDE}
	${GLAD_INCLUDE}
	${GLM_INCLUDE}
	${IMGUI_INCLUDE}
	${IMGUI_INCLUDE}/backends
	${IMGUI_INCLUDE}/misc/cpp
	${IMGUIZMO_INCLUDE}
)
target_link_libraries (seden_exe PUBLIC 
	glfw
	glad
	imgui
	imguizmo
)