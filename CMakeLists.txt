cmake_minimum_required (VERSION 3.0)
project (Tutorials)

find_Package(OpenGL REQUIRED)

add_subdirectory(external)

include_directories(
    external/AntTweakBar-1.16/include/
    external/glfw-3.1.2/include/
    external/glm-0.9.7.1/
    external/glew/-1.13.0/include/
    external/assimp-3.0.1270/include/
    external/bullet-2.81-rev2613/src/
    .
)

set(ALL_LIBS
    ${OPENGL_LIBRARY}
    glfw
    GLEW_1130
)

add_definitions (
    -DTW_STATIC
    -DTW_NO_LIB_PRAGMA
    -DTW_NO_DIRECT3D
    -DGLEW_STATIC
    -D_DRT_SECURE_NO_WARNINGS
)

# Tutorial 1
add_executable(tutorial01_first_window
    tutorial01_first_window/tutorial01.cpp
)
target_link_libraries(tutorial01_first_window
	${ALL_LIBS}
)

# Tutorial 2
add_executable(tutorial02_red_triangle
        tutorial02_red_triangle/tutorial02.cpp
        common/shader.cpp
        common/shader.hpp
)
target_link_libraries(tutorial02_red_triangle
        ${ALL_LIBS}
)

# Tutorial 3
add_executable(tutorial03_matrices 
    tutorial03_matrices/tutorial03.cpp
	common/shader.cpp
	common/shader.hpp
)

target_link_libraries(tutorial03_matrices
	${ALL_LIBS}
)

# Tutorial 4
add_executable(tutorial04_colored_cube
    tutorial04_colored_cube/tutorial04.cpp
    common/shader.cpp
    common/shader.hpp
)

target_link_libraries(tutorial04_colored_cube
    ${ALL_LIBS}
)