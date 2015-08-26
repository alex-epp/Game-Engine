#include "GameEngine.h"

GameEngine::GameEngine()
{
	init();
}
GameEngine::~GameEngine()
{
	if (created)
		quit();
}

static string GetLastErrorString()
{
	DWORD err = GetLastError();
	if (err == 0)
	{
		return string();
	}

	LPSTR msgBuffer = nullptr;
	size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&msgBuffer, 0, NULL);

	string message(msgBuffer, size);

	LocalFree(msgBuffer);

	return message;
}

bool GameEngine::init()
{
	/*this->hInstance = hInstance;

	// Create a window class
	WNDCLASS wc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = TEXT("glWindow");
	wc.lpszMenuName = 0;
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	
	// Register the class
	RegisterClass(&wc);

	// Create a window rect

	int x = Constants::FULLSCREEN ? 0 : 50;
	int y = Constants::FULLSCREEN ? 0 : 50;

	RECT rect;
	SetRect(&rect, x, y, x+Constants::WINDOW_WIDTH, y+Constants::WINDOW_HEIGHT);
	// Adjust it
	if(!Constants::FULLSCREEN) AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	// Create the window
	hwnd = CreateWindow(TEXT("glWindow"),
		TEXT(Constants::WINDOW_TITLE),
		Constants::FULLSCREEN ? WS_POPUP : WS_OVERLAPPEDWINDOW,
		rect.left, rect.top,
		rect.right - rect.left,
		rect.bottom - rect.top,
		NULL, NULL,
		hInstance, NULL);

	if (hwnd == NULL) // Check that the window was created successfully
	{
		LOG_ERR("Failed to create window: ", GetLastErrorString());
		return false;
	}

	// Get the window DC
	hdc = GetDC(hwnd);

	// Show the window
	ShowWindow(hwnd, nCmdShow);

	// Create pixel format descriptor
	PIXELFORMATDESCRIPTOR pfd = { 0 };
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_DRAW_TO_WINDOW;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24;
	pfd.cDepthBits = 32;

	// Get a pixel format as close as possible to pfd
	int chosenPixelFormat = ChoosePixelFormat(hdc, &pfd);
	if (chosenPixelFormat == 0) // Check that the pixel format is valid
	{
		LOG_ERR("ChoosePixelFormat() failed");
		return false;
	}

	// Set the pixel format
	int result = SetPixelFormat(hdc, chosenPixelFormat, &pfd);
	if (result == NULL) // Check that the operation succeeded
	{
		LOG_ERR("SetPixelFormat() failed");
		return false;
	}

	// Create an OpenGL rendering context
	hglrc = wglCreateContext(hdc);

	// Connect the render context
	wglMakeCurrent(hdc, hglrc);*/

	// Initialize glfw
	if (!glfwInit())
	{
		LOG_ERR("Could not initialize glfw");
		return false;
	}
	// Multisampling
	glfwWindowHint(GLFW_SAMPLES, 8);

	GLFWmonitor* moniter = Constants::FULLSCREEN ? glfwGetPrimaryMonitor() : NULL;
	
	window = glfwCreateWindow(Constants::WINDOW_WIDTH, Constants::WINDOW_HEIGHT, Constants::WINDOW_TITLE, moniter, NULL);
	if (!window)
	{
		LOG_ERR("Could not create glfw window");
		return false;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	// Initialize GLEW
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		LOG_ERR("Could not initialize GLEW: ", glewGetErrorString(err));
		return false;
	}
	LOG("GLEW Version: ", GLEW_VERSION);
	LOG("OpenGL Version: ", glGetString(GL_VERSION));
	LOG("Vendor: ", glGetString(GL_VENDOR));
	LOG("Renderer: ", glGetString(GL_RENDERER));
	LOG("GLSL Version: ", glGetString(GL_SHADING_LANGUAGE_VERSION));

	if (!GLEW_VERSION_4_3)
		LOG_WARN("OpenGL 4.3 is not supported");
		

	// Initialize OpenGL
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);

	glViewport(0, 0, Constants::WINDOW_WIDTH, Constants::WINDOW_HEIGHT);
	glClearColor(0.f, 0.f, 0.f, 1.f);
	//glClearColor(1, 1, 1, 1);

	// Create Input singleton
	Input& inst = Input::getInstance(); // The first time getInstance() is called
	inst.init(this);
	glfwSetKeyCallback(window, Input::keyCallback);
	glfwSetWindowSizeCallback(window, Input::resizeCallback);

	// Create per-frame UBO
	glGenBuffers(1, &frameDataUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, frameDataUBO);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(FrameData), NULL, GL_STREAM_DRAW);
	glBindBufferRange(GL_UNIFORM_BUFFER, Constants::PERFRAME_UNIFORM_INDEX, frameDataUBO, 0, sizeof(FrameData));

	// Some other book-keeping
	frameData.numLights = 0;
	created = true;

	return true;
}

void GameEngine::quit()
{
	/*// Unmake OpenGL rendering context
	wglMakeCurrent(NULL, NULL);
	// Delete it
	wglDeleteContext(hglrc);

	// Release the window DC
	ReleaseDC(hwnd, hdc);*/

	// Quit glfw
	glfwDestroyWindow(window);
	glfwTerminate();
}

void GameEngine::attachFrameData(const Model& model)
{
	model.attachUniformBlock(Constants::PERFRAME_UNIFORM_NAME, Constants::PERFRAME_UNIFORM_INDEX);
}

void GameEngine::updateFrameData()
{
	glBindBuffer(GL_UNIFORM_BUFFER, frameDataUBO);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(FrameData), &frameData);
}

void GameEngine::updateMatrices(mat4 projection, mat4 view)
{
	frameData.projection = projection;
	frameData.view = view;
}

void GameEngine::addLight(Light light)
{
	frameData.lights[frameData.numLights++] = light;
}

void GameEngine::resizeWindow(int width, int height)
{
	Constants::WINDOW_WIDTH = width;
	Constants::WINDOW_HEIGHT = height;
	glViewport(0, 0, width, height);

	frameData.projection = glm::perspective(Constants::FOV, Constants::WINDOW_WIDTH / (float)Constants::WINDOW_HEIGHT, Constants::NEAR_PLANE, Constants::FAR_PLANE);
}

void GameEngine::mainLoop()
{
	Model model;
	model.loadFromFile("crytek-sponza/", "sponza2.obj");
	attachFrameData(model);
	//model.scaleTo(vec3(10, 10, 10));
	//model.translateTo(vec3(0, -50, 0));
	model.rotateTo(3.14159/2.0, vec3(0, 1, 0));


	vec3 camOrigin = vec3(0, 600, -800);

	vec3 eyePos = camOrigin;
	vec3 eyeDir = vec3(0, 600, 1300);
	mat4 projection = glm::perspective(Constants::FOV, Constants::WINDOW_WIDTH / (float)Constants::WINDOW_HEIGHT, Constants::NEAR_PLANE, Constants::FAR_PLANE);
	mat4 view = glm::lookAt(eyePos, eyeDir, glm::vec3(0, 1, 0));
	updateMatrices(projection, view);

	Light l;
	l.ambient = vec4(1, 0.8, 0.6, .3);
	l.diffuse = vec4(1, 1, .8, 1);
	l.specular = vec4(1, 1, 1, .1);
	l.position = eyePos;
	l.radius = 500;
	addLight(l);

	Light l2;
	l2.ambient = vec4(1, 1, 1, .7);
	l2.diffuse = vec4(1, 1, 1, .7);
	l2.specular = vec4(1, 1, 1, .7);
	l2.position = vec3(0, 1000, 0);
	l2.radius = 2000;
	addLight(l2);

	float ang = 0.1f;
	
	/*
	MSG msg;
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{

			ang += 0.01f;
			model.translateTo(vec3(0, 0, 0));
			model.rotateTo(3.14159f, vec3(0, 0, 1));
			model.rotateBy(ang, vec3(0, 1, 0));

			// Updating and rendering
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			model.render(VPmatrix);
			SwapBuffers(hdc);
		}
	}
	*/


	auto start = GetTickCount();

	while (!glfwWindowShouldClose(window))
	{
		ang += 0.01f;
		frameData.lights[0].position.z = (cos(ang)+1.0) * 1000 + camOrigin.z;
		//frameData.lights[0].position.y = (cos(ang) + 1.0) * -200 + camOrigin.y;
		frameData.lights[0].position.x = cos(ang) * 160 + camOrigin.x;

		frameData.view = glm::lookAt(frameData.lights[0].position, eyeDir, glm::vec3(0, 1, 0));

		frameData.time = GetTickCount() - start;
		updateFrameData();

		// Updating and rendering
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		model.render();
		glfwSwapBuffers(window);

		glfwPollEvents();
	}
}

/*
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	switch (message)
	{
	case WM_CREATE:
		return 0;
	case WM_PAINT:
		{
			HDC hdc;
			PAINTSTRUCT ps;
			hdc = BeginPaint(hwnd, &ps);
			EndPaint(hwnd, &ps);
		}
		return 0;
	case WM_KEYDOWN:
		switch (wparam)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		}
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hwnd, message, wparam, lparam);
	}
}*/