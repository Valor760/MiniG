#pragma once

/* COMMON INCLUDES */
/* C/C++ INCLUDES */
#include <stdint.h>
#include <string>
#include <iostream>
#include <memory>
#include <filesystem>
#include <vector>

/* PROJECT INCLUDES */
#include "log/log.h"

/* EXTERNAL INCLUDES */
#include "imgui/imgui.h"

/* COMMON DEFINES */
#define ASSETS_DIR		"assets"

#define MINIG_UNUSED(x) ((void)(x))

/* OTHER STUFF */
namespace fs = std::filesystem;

template <class T>
struct MGVec2
{
	T x; T y;
	T operator[] (int idx)
	{
		assert(idx == 0 || idx == 1);
		return idx ? x : y;
	}

	bool operator==(const MGVec2& right) const
	{
		return this->x == right.x && this->y == right.y;
	}

	MGVec2 operator-(MGVec2 right)
	{
		return {x - right.x, y - right.y};
	}
};

static inline ImVec4 Vec4Norm(const ImVec4& vec4, int norm)
{
	return ImVec4(
			vec4.x / norm,
			vec4.y / norm,
			vec4.z / norm,
			vec4.w / norm
		);
}