#pragma once
#include <chrono>
#include "Logger.h"
using namespace std;
class FPSCounter
{
public:
	FPSCounter();
	~FPSCounter();

	static void update()
	{
		static int frameCount = 0;
		static auto start = chrono::steady_clock::now();

		frameCount++;
		auto diff = chrono::steady_clock::now() - start;
		if (diff >= 1s)
		{
			LOG("Ms/frame - ", chrono::duration_cast<chrono::milliseconds>(diff).count() / static_cast<float>(frameCount));
			LOG("Frames/sec - ", frameCount * 1000.f / chrono::duration_cast<chrono::milliseconds>(diff).count());
			frameCount = 0;
			start = chrono::steady_clock::now();
		}
	}
};

