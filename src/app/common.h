#pragma once

/* COMMON INCLUDES */
/* C/C++ INCLUDES */
#include <stdint.h>
#include <string>
#include <iostream>
#include <memory>
#include <filesystem>
#include <thread>

/* PROJECT INCLUDES */
#include "log/log.h"

/* EXTERNAL INCLUDES */
#include "imgui/imgui.h"

/* COMMON DEFINES */
#define ASSETS_DIR		"assets"

#define MINIG_UNUSED(x) ((void)(x))

/* OTHER STUFF */
namespace fs = std::filesystem;