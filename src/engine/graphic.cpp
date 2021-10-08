#include <engine/graphic.h>



namespace mushgine {
	
	void GraphicManager::Init()
	{
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		this->window = glfwCreateWindow(1024, 1024 - 50, "Name", NULL, NULL);
		if (window == NULL)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return;
		}
		glfwMakeContextCurrent(this->window);

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init();
		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
	}

	float timer = 0.0f;
	void GraphicManager::Update(float dt)
	{
		timer += dt;
		//while (!glfwWindowShouldClose(this->window))


		glfwPollEvents();
		//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// feed inputs to dear imgui, start new frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// render your GUI
		ImGui::Begin("Demo window");
		ImGui::Button("Mushroom Engine say Hello!");
		char msg = (timer);
		ImGui::Text(&msg);
		ImGui::End();

		// Render dear imgui into screen
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(this->window);
	}

	void GraphicManager::Destroy()
	{
		glfwTerminate();
	}
}