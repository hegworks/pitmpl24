#include "GeneralCameraCInput.h"

#include "GeneralCamera.h"
#include <iostream>
#include <stdexcept>

namespace Uknitty
{

GeneralCameraCInput::GeneralCameraCInput(GeneralCamera* owner)
{
	if(!owner)
	{
		throw std::runtime_error("owner is nullptr");
	}

	m_owner = owner;
}

void GeneralCameraCInput::ProcessMousePosition(double xPos, double yPos)
{
	auto m_cameraType = m_owner->GetCameraType();
	switch(m_cameraType)
	{
		case GeneralCamera::CameraType::TOP_DOWN_FOLLOW:
			break;
		case GeneralCamera::CameraType::THIRD_PERSON_FOLLOW:
		case GeneralCamera::CameraType::FIRST_PERSON:
		{
			if(m_isFirstMouse) // initially set to true
			{
				m_lastX = xPos;
				m_lastY = yPos;
				m_isFirstMouse = false;
			}

			double xOffset = xPos - m_lastX;
			double yOffset = m_lastY - yPos; // reversed since y-coordinates range from bottom to top
			m_lastX = xPos;
			m_lastY = yPos;

			xOffset *= MOUSE_SENSITIVITY;
			yOffset *= MOUSE_SENSITIVITY;

			m_owner->SetYaw(m_owner->GetYaw() + xOffset);
			m_owner->SetPitch(m_owner->GetPitch() + yOffset);

			switch(m_cameraType)
			{
				case GeneralCamera::CameraType::THIRD_PERSON_FOLLOW:
					if(m_owner->GetPitch() > m_owner->PITCH_MAX_THIRD_PERSON) m_owner->SetPitch(m_owner->PITCH_MAX_THIRD_PERSON);
					if(m_owner->GetPitch() < m_owner->PITCH_MIN_THIRD_PERSON) m_owner->SetPitch(m_owner->PITCH_MIN_THIRD_PERSON);
					break;
				case GeneralCamera::CameraType::FIRST_PERSON:
					if(m_owner->GetPitch() > m_owner->PITCH_MAX_FIRST_PERSON) m_owner->SetPitch(m_owner->PITCH_MAX_FIRST_PERSON);
					if(m_owner->GetPitch() < m_owner->PITCH_MIN_FIRST_PERSON) m_owner->SetPitch(m_owner->PITCH_MIN_FIRST_PERSON);
					break;
				case GeneralCamera::CameraType::TOP_DOWN_FOLLOW:
				default:
					throw std::runtime_error("unknown camera type");
			}
		}
		break;
		default:
			throw std::runtime_error("unknown camera type");
	}
}

void GeneralCameraCInput::OnKeyDown(Key key)
{
	std::cout << "GeneralCameraCInput::OnKeyDown" << std::endl;
	if(key == CAMERA_TYPE_SWITCH_KEY)
	{
		int nextCameraType = ((int)m_owner->GetCameraType() + 1) % m_owner->TOTAL_CAMERA_TYPES;
		m_owner->SetCameraType(static_cast<Uknitty::GeneralCamera::CameraType>(nextCameraType));
	}
}

} // namespace Uknitty
