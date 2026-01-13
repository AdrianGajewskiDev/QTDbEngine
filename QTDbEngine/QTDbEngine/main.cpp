#include "pch.h"
#include <iostream>

const std::string ENGINE_VERSION = "0.0.1";

int main(int argc, char* argv[])
{
	Engine engine(ENGINE_VERSION);

	engine.Initialize();
}
