#pragma once
#include "layout.h"

namespace MiniG::Gui
{
#define EXTERN_GETLAYOUT(X) \
		extern Layout GetLayout_##X(); \
		inline const char* LayoutName_##X = #X;

EXTERN_GETLAYOUT(MainMenu);

} /* namespace MiniG::Gui */