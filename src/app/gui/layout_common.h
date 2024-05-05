#pragma once
#include "layout.h"

namespace MiniG::Gui
{
#define MAKE_LAYOUT(X) Layout Layout_##X

#define EXTERN_GETLAYOUT(X) \
		extern Layout Layout_##X; \
		inline Layout GetLayout_##X() { return Layout_##X; } \
		inline const char* LayoutName_##X = #X;

EXTERN_GETLAYOUT(MainMenu);
EXTERN_GETLAYOUT(SelectGame);
EXTERN_GETLAYOUT(Tetris);
EXTERN_GETLAYOUT(Snake);

/* Functions to init values, that can't be initialized during dynamic init */
void Init_LayoutMainMenu();
void Init_LayoutGameSelection();

} /* namespace MiniG::Gui */