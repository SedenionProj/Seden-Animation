cmake_minimum_required(VERSION 3.7)
project(Seden)
set (CMAKE_CXX_STANDARD 17)
set(VAR OFF)
find_package(OpenGL REQUIRED)

add_subdirectory (${CMAKE_CURRENT_SOURCE_DIR}/Seden/vendor)

add_executable (seden_exe Seden/src/test.cpp)

target_include_directories (seden_exe PUBLIC 
	${STB_INCLUDE}
	${GLFW_INCLUDE}
	${GLAD_INCLUDE}
	${GLM_INCLUDE}
	${IMGUI_INCLUDE}
	${IMGUI_INCLUDE}/backends
	${IMGUIZMO_INCLUDE}
)
target_link_libraries (seden_exe PUBLIC 
	glfw
	glad
	imgui
	imguizmo
)