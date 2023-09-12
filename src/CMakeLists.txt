cmake_minimum_required(VERSION 3.16)

set(EXEC ${TARGET_PROJECT})
project(${EXEC})

file(GLOB_RECURSE SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.h ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)
add_executable(${EXEC} ${SRC} ${HEADERS})

if(MSVC)
	target_compile_definitions(${EXEC} PUBLIC ${WINDOWS_DEFINES} ${COMMON_DEFINES})
	set_property(TARGET ${EXEC} PROPERTY CXX_STANDARD 20)
elseif(UNIX AND NOT APPLE)
	target_compile_definitions(${TARGET} PUBLIC ${LINUX_DEFINES} ${COMMON_DEFINES})
	set_property(TARGET ${EXEC} PROPERTY CXX_STANDARD 20)
endif()

add_library(GLAD ${LIB_GLAD_PATH})
add_library(imgui
		${EXTERNAL_DIR}/imgui/imgui.cpp
		${EXTERNAL_DIR}/imgui/imgui_tables.cpp
		${EXTERNAL_DIR}/imgui/imgui_widgets.cpp
		${EXTERNAL_DIR}/imgui/imgui_draw.cpp
		${EXTERNAL_DIR}/imgui/backends/imgui_impl_glfw.cpp
		${EXTERNAL_DIR}/imgui/backends/imgui_impl_opengl3.cpp
	)

set(COMMON_LIBS glfw3 imgui GLAD)
if(UNIX)
	set(OS_LIBS pthread GL dl)
else()
	set(OS_LIBS opengl32)
endif()

target_link_libraries(${EXEC} PUBLIC ${OS_LIBS} ${COMMON_LIBS})