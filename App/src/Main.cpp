#include "Core/Application.h"

#include "MainLayer.h"

int main()
{
	Core::ApplicationSpecification appSpec;
	appSpec.Name = "Test Application";
	appSpec.WindowSpec.Width = 800;
	appSpec.WindowSpec.Height = 800;

	Core::Application app(appSpec);
	app.PushLayer<MainLayer>();
	app.Run();

    return 0;
}