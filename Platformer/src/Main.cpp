#include "Core/Application.h"

#include "MainLayer.h"

int main()
{
	Core::ApplicationSpecification appSpec;
	appSpec.Name = "Test Application";
	appSpec.WindowSpec.Width = 960;
	appSpec.WindowSpec.Height = 540;
	appSpec.WindowSpec.VSync = true;
	appSpec.WindowSpec.Multisampling = false;

	Core::Application app(appSpec);
	app.PushLayer<Platformer::MainLayer>();
	app.Run();
	
	return 0;
}