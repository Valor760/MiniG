#include "app/app.h"

int main()
{
	MiniG::MainApp app;
	if(!app.Init())
	{
		LOG_ERROR("App initialization failed!");
		return 1;
	}
	app.Run();
	return 0;
}