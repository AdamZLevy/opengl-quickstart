cmake_minimum_required(VERSION 3.17)
project(opengl-quickstart)

find_package(OpenGL REQUIRED)

set(CMAKE_CXX_STANDARD 20)

# define DEBUG depending on run type
add_compile_options("$<$<CONFIG:DEBUG>:-DDEBUG>")

# definitions
add_definitions(
        -DGLEW_STATIC
)

# libraries setup
set(GLFW_PATH lib/glfw-3.3.2)
set(GLEW_PATH lib/glew-2.1.0)
set(IMGUI_PATH lib/imgui-docking)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(${GLFW_PATH})

include_directories(
        ${GLFW_PATH}/include/GLFW
        ${GLEW_PATH}/include
        ${IMGUI_PATH}
        lib
)

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(OPENGL_LIBRARY
            ${OPENGL_LIBRARY}
            -lGL-lGL -lGLU -lXrandr -lXext -lX11 -lrt
            ${CMAKE_DL_LIBS}
            ${GLFW_LIBRARIES}
            )
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(OPENGL_LIBRARY
            ${OPENGL_LIBRARY}
            ${CMAKE_DL_LIBS}
            ${GLFW_LIBRARIES}
            )
endif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")

set(GLEW_SOURCE
        ${GLEW_PATH}/src/glew.c
        src/opengl-renderer/Texture.cpp src/opengl-renderer/Texture.h src/ui/UI.cpp src/ui/UI.h src/ui/Widget.cpp src/ui/Widget.h src/ui/widgets/Info.cpp src/ui/widgets/Info.h)


add_library(GLEW STATIC
        ${GLEW_SOURCE}
        ${GLEW_INCLUDE}
        )


target_link_libraries(GLEW
        ${OPENGL_LIBRARY}
        ${EXTRA_LIBS}
        )

set(ALL_LIBS
        ${OPENGL_LIBRARY}
        glfw
        GLEW
        )

file(GLOB IMGUI_SRC
        "${IMGUI_PATH}/*.h"
        "${IMGUI_PATH}/*.cpp"
        )

file(GLOB STB_IMAGE
        "lib/stb_image.h"
        "lib/stb_image.cpp"
        )

# project files setup
set(OPENGL_RENDERER src/opengl-renderer/Renderer.cpp src/opengl-renderer/Renderer.h src/opengl-renderer/VertexBuffer.cpp src/opengl-renderer/VertexBuffer.h src/opengl-renderer/IndexBuffer.cpp src/opengl-renderer/IndexBuffer.h src/opengl-renderer/VertexArray.cpp src/opengl-renderer/VertexArray.h src/opengl-renderer/VertexBufferLayout.cpp src/opengl-renderer/VertexBufferLayout.h src/opengl-renderer/Shader.cpp src/opengl-renderer/Shader.h)

# target setup
add_executable(opengl-quickstart main.cpp ${IMGUI_SRC} ${STB_IMAGE} ${OPENGL_RENDERER})

target_link_libraries(opengl-quickstart ${ALL_LIBS})


add_compile_options("$<$<CONFIG:DEBUG>:-DDEBUG>")


add_custom_command(TARGET opengl-quickstart POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E rm -rf $<TARGET_FILE_DIR:opengl-quickstart>/shader)
add_custom_command(TARGET opengl-quickstart POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E rm -rf $<TARGET_FILE_DIR:opengl-quickstart>/resources)

add_custom_command(TARGET opengl-quickstart POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/shader $<TARGET_FILE_DIR:opengl-quickstart>/shader)
add_custom_command(TARGET opengl-quickstart POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/resources $<TARGET_FILE_DIR:opengl-quickstart>/resources)
