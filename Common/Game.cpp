#include "Game.h"

#include "stb_image.h"

#include "Camera.h"
#include "CubeModel.h"
#include "GeneralOBJ.h"
#include "IGraphics.h"
#include "IInput.h"
#include "Input.h"
#include "ObjectModel.h"
#include "ShaderManager.h" // more of a generic system of graphics
#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
// keeping some things in global space...because....Brian does it this way :D you should consider better ways....AKA, you should consider better ways...do I have to spell it out?
std::vector<ObjectModel*> MyObjects; // on the basis that every object is derived from ObjectModel, we keep a list of things to draw.
std::vector<ObjectModel*> Cubes; // on the basis that every object is derived from ObjectModel, we keep a list of things to draw.
ShaderManager ShaderManager;

Game::Game(Input* input, IGraphics* graphics) :
	input(input),
	graphics(graphics)

{

}

Game::~Game()
{

}

void Game::KeyCallback(Key key, KeyAction action)
{
	// Implement your key callback logic here
	if(action == KeyAction::DOWN)
	{
		switch(key)
		{
			case Key::W:
				std::cout << "W down" << std::endl;
				break;
			case Key::S:
				std::cout << "S down" << std::endl;
				break;
			case Key::A:
				std::cout << "A down" << std::endl;
				break;
			case Key::D:
				std::cout << "D down" << std::endl;
				break;
			case Key::ESCAPE:
				std::cout << "ESCAPE down" << std::endl;
				Quit();
				break;
			default:
				break;
		}
	}
}


void Game::Start()
{
	InitializeOpenGLES();

	camera = new Camera();

	MyFiles* Handler = new MyFiles();
	ObjectModel* T2;   // so both types even though quite different use the same base to create them

	glm::vec3 Pos = glm::vec3(1.0); // set a position

	for(int i = -5; i < 5; i++)
	{
		for(int j = -5; j < 5; j++)
		{
			T2 = new CubeModel(Handler); // make a new cube
			Pos = glm::vec3(i * 4.1, 0.0f, j * 4.1); // set a position
			T2->Scales = glm::vec3(4);		 // a cube is actually quite large (screen size) so shrink it down
			T2->SetPosition(Pos);
			Cubes.push_back(T2); // store in the vector ready for the game loop to process
			T2->StoreGraphicClass(&ShaderManager); // make sure it knows the where the graphics data is, (for now it contains our attribute/uniform info)
			ShaderManager.AttachShader(T2); // set it up
		}
	}

	//reuse T2
	//T2 = new GeneralOBJ((char*)"../Common/Assets/Models/brian_01.obj", Handler); // make a new OBJ
	//Pos = glm::vec3(5.0f, 10.0f, 0.0f); // set a position
	//T2->SetPosition(Pos);
	//T2->Scales = glm::vec3(1.0f, 1.0f, 1.0f);

	//
	//MyObjects.push_back(T2); // store in the vector ready for the game loop to process
	//T2->StoreGraphicClass(&ShaderManager); // make sure it knows the where the graphics data is, (for now it contains our attribute/uniform info)
	//ShaderManager.AttachShader(T2); // set it up

	//reuse T2
	T2 = new GeneralOBJ((char*)"../Common/Assets/Models/Naked_Snake.obj", Handler); // make a new OBJ
	Pos = glm::vec3(0.0f, 3.0f, 0.0f); // set a position
	T2->SetPosition(Pos);
	T2->Scales = glm::vec3(0.4);

	MyObjects.push_back(T2); // store in the vector ready for the game loop to process
	T2->StoreGraphicClass(&ShaderManager); // make sure it knows the where the graphics data is, (for now it contains our attribute/uniform info)
	ShaderManager.AttachShader(T2); // set it up


	//reuse T2
	//T2 = new GeneralOBJ((char*)"../Common/Assets/Models/brian_02.obj", Handler); // make a new OBJ
	//Pos = glm::vec3(5.0f, -10.0f, 0.0f); // set a position
	//T2->SetPosition(Pos);
	//T2->Scales = glm::vec3(1.0f, 1.0f, 1.0f);


	//MyObjects.push_back(T2); // store in the vector ready for the game loop to process
	//T2->StoreGraphicClass(&ShaderManager); // make sure it knows the where the graphics data is, (for now it contains our attribute/uniform info)
	//ShaderManager.AttachShader(T2); // set it up



	//reuse T2
	//T2 = new GeneralOBJ((char*)"../Common/Assets/Models/brian_03.obj", Handler); // make a new OBJ
	//Pos = glm::vec3(-5.0f, 10.0f, 0.0f); // set a position
	//T2->SetPosition(Pos);
	//T2->Scales = glm::vec3(1.0f, 1.0f, 1.0f);


	//MyObjects.push_back(T2); // store in the vector ready for the game loop to process
	//T2->StoreGraphicClass(&ShaderManager); // make sure it knows the where the graphics data is, (for now it contains our attribute/uniform info)
	//ShaderManager.AttachShader(T2); // set it up


	// Timing
	auto startTime = std::chrono::system_clock::now();
	auto lastTime = startTime;

	float averageFPS{0};


	input->GetKeyboard()->SetKeyCallback(
		[this](Key key, KeyAction action) { KeyCallback(key, action); }
	);

	while(!quitting)
	{
		ProcessInput();
		auto time = std::chrono::system_clock::now();
		std::chrono::duration<float> delta = time - lastTime;

		gameDeltaTime = delta.count();

		camera->Update(gameDeltaTime);

		std::chrono::duration<float> elapsed = time - startTime;
		if(elapsed.count() > 0.25f && frameCount > 10)
		{
			averageFPS = static_cast<float>(frameCount) / elapsed.count();
			startTime = time;
			frameCount = 0;
		}
		// Setup the viewport
		ClearScreen();
		glViewport(0, 0, SCRWIDTH, SCRHEIGHT);





		for(size_t i = 0; i < MyObjects.size(); i++)
		{
			//update the object, in this demo, we just up its position and transform data but you should consider logic
			MyObjects[i]->Update(camera);
			// the draw routine is the responsbility of the object itself, thats not an ideal system, consider how to improve
			MyObjects[i]->Draw();
		}

		for(size_t i = 0; i < Cubes.size(); i++)
		{
			//update the object, in this demo, we just up its position and transform data but you should consider logic
			Cubes[i]->Update(camera);
			// the draw routine is the responsbility of the object itself, thats not an ideal system, consider how to improve
			Cubes[i]->Draw();
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

Input* Game::GetInput() const
{
	return input;
}

void Game::Quit()
{
	quitting = true;
}

//example of using the key and mouse
void Game::ProcessInput()
{
	Input* input = GetInput();
	IMouse* mouse = input->GetMouse();
	IKeyboard* keyboard = input->GetKeyboard();
	if(keyboard->GetKey(Key::W))
	{
		printf("we pressed W\n");
	}
	if(keyboard->GetKey(Key::S))
	{
		printf("we pressed S\n");
	}
	if(keyboard->GetKey(Key::A))
	{
		printf("we pressed A\n");
	}
	if(keyboard->GetKey(Key::D))
	{
		printf("we pressed D\n");
	}
	if(keyboard->GetKey(Key::ESCAPE))
	{
		printf("we pressed ESCAPE\n");
	}

	if(mouse->GetButtonDown(MouseButtons::LEFT))
	{
		printf("we pressed mouse left\n");
	}

	camera->MouseCallback(mouse->GetPosition().x, mouse->GetPosition().y);
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
	glViewport(0, 0, SCRWIDTH, SCRHEIGHT);
}

void Game::ClearScreen()
{
	glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
