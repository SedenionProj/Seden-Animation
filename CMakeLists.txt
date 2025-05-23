cmake_minimum_required(VERSION 3.7)
project(Seden)
set (CMAKE_CXX_STANDARD 20)

add_compile_definitions(SEDEN_VERBOSE)

add_subdirectory (${CMAKE_CURRENT_SOURCE_DIR}/Seden/vendor)

file(GLOB_RECURSE SEDEN_SOURCE
	Seden/src/*.cpp
)

file(COPY "${CMAKE_SOURCE_DIR}/Seden/assets" DESTINATION "${CMAKE_BINARY_DIR}")

if (MSVC)
	add_definitions(/wd4244 /wd4267 /wd4305)
endif ()

add_library (seden_lib ${SEDEN_SOURCE})

target_precompile_headers(seden_lib PRIVATE ${CMAKE_SOURCE_DIR}/Seden/src/pch.h)

target_include_directories (seden_lib PUBLIC
	Seden
)

target_include_directories (seden_lib PUBLIC
	${STB_INCLUDE}
	${ENTT_INCLUDE}
	${GLFW_INCLUDE}
	${GLAD_INCLUDE}
	${GLM_INCLUDE}
	${IMGUI_INCLUDE}
	${IMGUI_INCLUDE}/backends
	${IMGUI_INCLUDE}/misc/cpp
	${IMGUIZMO_INCLUDE}
)
target_link_libraries (seden_lib PUBLIC 
	glfw
	glad
	imgui
	imguizmo
)

# testApp

file(GLOB_RECURSE TESTAPP_SOURCE
	testApp/*.cpp
)

add_executable (testApp ${TESTAPP_SOURCE})

target_precompile_headers(testApp PRIVATE ${CMAKE_SOURCE_DIR}/Seden/src/pch.h)

target_link_libraries (testApp PRIVATE
	seden_lib
)