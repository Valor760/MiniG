#include "app/app.h"

int main()
{
	if(!MiniG::MainApp::Init())
	{
		LOG_ERROR("App initialization failed!");
		return 1;
	}
	MiniG::MainApp::Run();
	return 0;
}