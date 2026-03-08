#pragma once
#include <ctime>
#include <iostream>
#include "FrameWorkManager.h"

int main()
{
	FrameworkManager framework;
	//Setup and initialazation provide with window size
	if (!framework.Init(1200, 800)) return -1;

	//Loop
	framework.Run();
	//Clear and end
	framework.Shutdown();

	return 0;
}

