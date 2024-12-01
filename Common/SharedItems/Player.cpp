#include "IInputProcessor.h"
#include "Model.h"
#include "Player.h"
#include "Transform.h"

Player::Player(Uknitty::Model* model, Uknitty::ICamera* camera)
{
	m_model = model;
	m_iCamera = camera;

	m_transform = new Uknitty::Transform();
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

void Player::Awake()
{
}

void Player::Start()
{
}

void Player::Update(float deltaTime)
{
}

void Player::LateUpdate(float deltaTime)
{
}

void Player::FixedUpdate()
{
}

void Player::Render()
{
}

