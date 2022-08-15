#pragma once
#include <memory>
#include "State.h"

//This class implements RAII which means the ctor and dtor manage the 
//initialization and shutdown respectively and avoid code clutter in 
//main.cpp. This class works but has not been fully tested for issues.

class App
{

public:

	App(State* initialState);
	~App();
	void Run();

private:

	bool Initialize();
	bool CreateObjects();
	void Shutdown();

	int deltaTime{ 0 };
	std::unique_ptr<State> appState;

};