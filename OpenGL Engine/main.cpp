//#include <Windows.h>
//#include <chrono>
//#include <GL/freeglut.h>
//#include <gtc/matrix_transform.hpp>
//#include <iostream>
//#include "Model.h"
//#include "Constants.h"
#include "GameEngine.h"

//using namespace std;

int main(int argc, char* argv[])
{
	GameEngine engine;
	engine.mainLoop();

	return 0;
}

/*
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	
}*/