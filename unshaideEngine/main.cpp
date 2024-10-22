#include "Engine.h"
#include <iostream>

int main() {
	cout << "unshaide's OpenGL Engine" << endl;

	Engine engine;

	if (!engine.init()) return -1;
	if (!engine.bufferModes()) return -1;
	if (!engine.useShaders()) return -1;
	if (!engine.loadTextures()) return -1;

	engine.gameLoop();
	
	return 0;
}