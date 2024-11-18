#include "Game.h"



#include "stb_image.h"

#include "Input.h"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <iostream>
#include "ObjectModel.h"
#include "GeneralOBJ.h"
#include "CubeModel.h"
#include "IGraphics.h"
#include "Graphics.h" // more of a generic system of graphics
#include "IInput.h"
// keeping some things in global space...because....Brian does it this way :D you should consider better ways....AKA, you should consider better ways...do I have to spell it out?
std::vector<ObjectModel*> MyObjects; // on the basis that every object is derived from ObjectModel, we keep a list of things to draw.
Graphics Graphics;

Game::Game(const Input* const input, IGraphics* graphics) :
	input(input),
	graphics(graphics)

{

}

Game::~Game()
{

}


void Game::Start()
{
	InitializeOpenGLES();


	MyFiles* Handler = new MyFiles();
	ObjectModel* T2;   // so both types even though quite different use the same base to create them

	T2 = new CubeModel(Handler); // make a new cube
	glm::vec3 Pos = glm::vec3(-10.0f, 0.0f, 0.0f); // set a position
	T2->Scales = glm::vec3(0.2f, 0.2f, 0.2f);		 // a cube is actually quite large (screen size) so shrink it down
	T2->SetPosition(Pos);

	MyObjects.push_back(T2); // store in the vector ready for the game loop to process
	T2->StoreGraphicClass(&Graphics); // make sure it knows the where the graphics data is, (for now it contains our attribute/uniform info)
	Graphics.Init(T2); // set it up
//reuse T2
	T2 = new GeneralOBJ((char*)"../Common/Assets/Models/brian_01.obj", Handler); // make a new OBJ
	Pos = glm::vec3(5.0f, 10.0f, 0.0f); // set a position
	T2->SetPosition(Pos);
	T2->Scales = glm::vec3(1.0f, 1.0f, 1.0f);

	
	MyObjects.push_back(T2); // store in the vector ready for the game loop to process
	T2->StoreGraphicClass(&Graphics); // make sure it knows the where the graphics data is, (for now it contains our attribute/uniform info)
	Graphics.Init(T2); // set it up



	//reuse T2
	T2 = new GeneralOBJ((char*)"../Common/Assets/Models/brian_02.obj", Handler); // make a new OBJ
	Pos = glm::vec3(5.0f, -10.0f, 0.0f); // set a position
	T2->SetPosition(Pos);
	T2->Scales = glm::vec3(1.0f, 1.0f, 1.0f);


	MyObjects.push_back(T2); // store in the vector ready for the game loop to process
	T2->StoreGraphicClass(&Graphics); // make sure it knows the where the graphics data is, (for now it contains our attribute/uniform info)
	Graphics.Init(T2); // set it up



	//reuse T2
	T2 = new GeneralOBJ((char*)"../Common/Assets/Models/brian_03.obj", Handler); // make a new OBJ
	Pos = glm::vec3(-5.0f, 10.0f, 0.0f); // set a position
	T2->SetPosition(Pos);
	T2->Scales = glm::vec3(1.0f, 1.0f, 1.0f);


	MyObjects.push_back(T2); // store in the vector ready for the game loop to process
	T2->StoreGraphicClass(&Graphics); // make sure it knows the where the graphics data is, (for now it contains our attribute/uniform info)
	Graphics.Init(T2); // set it up


	// Timing
	auto startTime = std::chrono::system_clock::now();
	auto lastTime = startTime;

	float averageFPS{ 0 };
	

	while(!quitting)
	{
		ProcessInput();
		auto time = std::chrono::system_clock::now();
		std::chrono::duration<float> delta = time - lastTime;

		gameDeltaTime = delta.count();

		std::chrono::duration<float> elapsed = time - startTime;
		if(elapsed.count() > 0.25f && frameCount > 10)
		{
			averageFPS = static_cast<float>(frameCount) / elapsed.count();
			startTime = time;
			frameCount = 0;
		}
		// Setup the viewport
		ClearScreen();
		glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);


		


		for (size_t i = 0; i < MyObjects.size(); i++)
		{
			//update the object, in this demo, we just up its position and transform data but you should consider logic
			MyObjects[i]->Update();
			// the draw routine is the responsbility of the object itself, thats not an ideal system, consider how to improve
			MyObjects[i]->Draw();
		}


		glFlush();
		graphics->SwapBuffer();
		lastTime = time;
		++frameCount;
	}

	graphics->Quit();
}


btDiscreteDynamicsWorld* Game::World() const
{
	return world;
}

const Input& Game::GetInput() const
{
	return *input;
}

void Game::Quit()
{
	quitting = true;
}


//example of using the key and mouse
void Game::ProcessInput()
{
	const Input& input = GetInput();
	const IMouse& mouse = GetInput().GetMouse();
	{
		if (input.GetKeyboard().GetKey(Key::W))
		{
			printf("we pressed W\n");
		}
		if (input.GetKeyboard().GetKey(Key::S))
		{
			printf("we pressed S\n");
		}
		if (input.GetKeyboard().GetKey(Key::A))
		{
			printf("we pressed A\n");
		}
		if (input.GetKeyboard().GetKey(Key::D))
		{
			printf("we pressed D\n");
		}

		if (input.GetMouse().GetButtonDown(MouseButtons::LEFT))
		{
			printf("we pressed mouse left\n");
		}

	
	}



}



void Game::InitializeOpenGLES()
{

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glDepthRangef(0.0f, 1.0f);
	glClearDepthf(1.0f);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquation(GL_FUNC_ADD);

	glCullFace(GL_BACK);
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
}

void Game::ClearScreen()
{
	glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
