//#include "MainLayer.h"
#include "TestLayer.h"

#include "Core/Application.h"

int main()
{
	Core::ApplicationSpecification appSpec;
	appSpec.Name = "Test Application";
	appSpec.WindowSpec.Width = 800;
	appSpec.WindowSpec.Height = 800;
	appSpec.WindowSpec.VSync = true;
	appSpec.WindowSpec.Multisampling = false;

	Core::Application app(appSpec);
	//app.PushLayer<MainLayer>();
	app.PushLayer<TestLayer>();
	app.Run();
	
	return 0;
}