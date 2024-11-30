#include "IInputProcessor.h"
#include "Model.h"
#include "Player.h"
#include "Transform.h"

Player::Player(Model* model, ICamera* camera)
{
	m_model = model;
	m_iCamera = camera;

	m_transform = new Transform();
}

Player::~Player()
{
}

void Player::MouseCallback(double xPos, double yPos)
{
}

void Player::KeyDown(Key key)
{
}

void Player::KeyUp(Key key)
{
}

void Player::ProcessInput(IKeyboard* iKeyboard)
{
}

