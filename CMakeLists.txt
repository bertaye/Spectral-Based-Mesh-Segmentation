set(CMAKE_GENERATOR_PLATFORM Win32)
cmake_minimum_required(VERSION 3.10)

# Define your project name
project(Spectral_Based_Mesh_Segmentation)

# Set C++ Standard
set(CMAKE_CXX_STANDARD 17)

add_definitions(-DGLEW_NO_GLU)

# Add all source files to a variable
set(SOURCES
    Source\ Files/Camera.cpp
    Source\ Files/Mesh.cpp
    Source\ Files/MeshRenderer.cpp
    Source\ Files/Shader.cpp
    Source\ Files/Window.cpp
    Source\ Files/main.cpp
)

# Add all header files to a variable
set(HEADERS
    Header\ Files/Camera.h
    Header\ Files/Mesh.h
    Header\ Files/MeshRenderer.h
    Header\ Files/Shader.h
    Header\ Files/STriangle.h
    Header\ Files/SVertex.h
    Header\ Files/Window.h
)

# Include directories for the compiler to find
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Header\ Files)
include_directories("C:/External Libs")
include_directories("C:/External Libs/Eigen")
include_directories("C:/External Libs/GLEW/include")
include_directories("C:/External Libs/GLFW/include")
include_directories("C:/External Libs/GLM")
include_directories("C:/External Libs/Spectra/include")

# If GLEW, GLFW, Eigen, GLM, Spectra libraries are compiled and exist as *.lib files in their directories.
# add them to a variable
set(LIBS
    "C:/External Libs/GLEW/lib/Release/Win32/glew32.lib"
    "C:/External Libs/GLFW/lib-vc2022/glfw3.lib" # Adjust this as per your actual Win32 library location
    "opengl32.lib"
    # Add other libraries as required
)

# Create an executable with your project name
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Link the libraries with your executable
target_link_libraries(${PROJECT_NAME} ${LIBS})

# Copy glew32.dll to the output directory after building
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_if_different
                   "C:/External Libs/GLEW/bin/Release/Win32/glew32.dll"
                   $<TARGET_FILE_DIR:${PROJECT_NAME}>)
