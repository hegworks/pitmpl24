#include "ICamera.h"
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
	if(key == FORWARD_KEY) m_isForwardKeyDown = true;
	if(key == BACKWARD_KEY) m_isBackwardKeyDown = true;
	if(key == LEFT_KEY) m_isLeftKeyDown = true;
	if(key == RIGHT_KEY) m_isRightKeyDown = true;
}

void Player::KeyUp(Key key)
{
	if(key == FORWARD_KEY) m_isForwardKeyDown = false;
	if(key == BACKWARD_KEY) m_isBackwardKeyDown = false;
	if(key == LEFT_KEY) m_isLeftKeyDown = false;
	if(key == RIGHT_KEY) m_isRightKeyDown = false;
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
	glm::vec3 movement = glm::vec3(0);
	if(m_isForwardKeyDown)
	{
		movement += m_iCamera->GetForward();
	}
	if(m_isBackwardKeyDown)
	{
		movement -= m_iCamera->GetForward();
	}
	if(m_isRightKeyDown)
	{
		movement += m_iCamera->GetRight();
	}
	if(m_isLeftKeyDown)
	{
		movement -= m_iCamera->GetRight();
	}
	movement.y = 0;
	if(glm::length(movement) > 0)
	{
		movement = glm::normalize(movement);
		m_transform->SetPosition(*m_transform->GetPosition() + movement * m_moveSpeed * deltaTime);

		// REF: these calculations for gradual rotation are from ChatGPT
		float targetAngle = atan2(movement.x, movement.z);
		float currentAngle = glm::radians(m_transform->GetRotation()->y);
		float angleDifference = targetAngle - currentAngle;

		if(angleDifference > glm::pi<float>()) angleDifference -= 2 * glm::pi<float>();
		if(angleDifference < -glm::pi<float>()) angleDifference += 2 * glm::pi<float>();

		float newAngle = currentAngle + glm::clamp(angleDifference, -m_rotationSpeed * deltaTime, m_rotationSpeed * deltaTime);
		glm::vec3 rotation = glm::vec3(0, glm::degrees(newAngle), 0);
		m_transform->SetRotation(rotation);
	}
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

