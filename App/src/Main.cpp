#include "MainLayer.h"
#include "Core/Application.h"

int main()
{
	Core::ApplicationSpecification appSpec;
	appSpec.Name = "Test Application";
	appSpec.WindowSpec.Width = 800;
	appSpec.WindowSpec.Height = 800;
	appSpec.WindowSpec.VSync = true;
	appSpec.WindowSpec.Multisampling = true;

	Core::Application app(appSpec);
	app.PushLayer<MainLayer>();
	app.Run();
	
	return 0;
}