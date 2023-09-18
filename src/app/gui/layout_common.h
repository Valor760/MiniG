#pragma once
#include "layout.h"

namespace MiniG::Gui
{
#define EXTERN_GETLAYOUT(X) \
		extern Layout Layout_##X; \
		inline Layout GetLayout_##X() { return Layout_##X; } \
		inline const char* LayoutName_##X = #X;

EXTERN_GETLAYOUT(MainMenu);
EXTERN_GETLAYOUT(SelectGame);

} /* namespace MiniG::Gui */