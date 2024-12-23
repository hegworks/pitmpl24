#pragma once

#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>

namespace Uknitty
{

class InterfaceManager
{
#if 0
public:

#pragma region Input
	void ProcessMousePosition(double xPos, double yPos)
	{
		ProcessMousePositionCaller(m_inputAbles, xPos, yPos);
		ProcessMousePositionCaller(m_flowInputAbles, xPos, yPos);
		ProcessMousePositionCaller(m_flowInputRenderAbles, xPos, yPos);
	}

	void ProcessKeyboard(IKeyboard* keyboard)
	{
		ProcessKeyboardCaller(m_inputAbles, keyboard);
		ProcessKeyboardCaller(m_flowInputAbles, keyboard);
		ProcessKeyboardCaller(m_flowInputRenderAbles, keyboard);
	}

	void ProcessInput(IMouse* mouse, IKeyboard* keyboard)
	{
		ProcessInputCaller(m_inputAbles, mouse, keyboard);
		ProcessInputCaller(m_flowInputAbles, mouse, keyboard);
		ProcessInputCaller(m_flowInputRenderAbles, mouse, keyboard);
	}

	void KeyDown(Key key)
	{
		KeyDownCaller(m_inputAbles, key);
		KeyDownCaller(m_flowInputAbles, key);
		KeyDownCaller(m_flowInputRenderAbles, key);
	}

	void KeyUp(Key key)
	{
		KeyUpCaller(m_inputAbles, key);
		KeyUpCaller(m_flowInputAbles, key);
		KeyUpCaller(m_flowInputRenderAbles, key);
	}
#pragma endregion Input

#pragma region Render
	void Draw()
	{
		DrawCaller(m_renderAbles);
		DrawCaller(m_flowRenderAbles);
		DrawCaller(m_flowInputRenderAbles);
	}
#pragma endregion Render

#pragma region Flow
	void Awake()
	{
		AwakeCaller(m_flowAbles);
		AwakeCaller(m_flowInputAbles);
		AwakeCaller(m_flowRenderAbles);
		AwakeCaller(m_flowInputRenderAbles);
	}

	void Start()
	{
		StartCaller(m_flowAbles);
		StartCaller(m_flowInputAbles);
		StartCaller(m_flowRenderAbles);
		StartCaller(m_flowInputRenderAbles);
	}

	void Update(float deltaTime)
	{
		UpdateCaller(m_flowAbles, deltaTime);
		UpdateCaller(m_flowInputAbles, deltaTime);
		UpdateCaller(m_flowRenderAbles, deltaTime);
		UpdateCaller(m_flowInputRenderAbles, deltaTime);
	}

	void LateUpdate(float deltaTime)
	{
		LateUpdateCaller(m_flowAbles, deltaTime);
		LateUpdateCaller(m_flowInputAbles, deltaTime);
		LateUpdateCaller(m_flowRenderAbles, deltaTime);
		LateUpdateCaller(m_flowInputRenderAbles, deltaTime);
	}

	void FixedUpdate()
	{
		FixedUpdateCaller(m_flowAbles);
		FixedUpdateCaller(m_flowInputAbles);
		FixedUpdateCaller(m_flowRenderAbles);
		FixedUpdateCaller(m_flowInputRenderAbles);
	}

	void Destroy()
	{
		std::cout << "Destroying InterfaceManager" << std::endl;

		DestroyCaller(m_flowAbles);
		m_flowAbles.clear();

		DestroyCaller(m_flowInputAbles);
		m_flowInputAbles.clear();

		DestroyCaller(m_flowRenderAbles);
		m_flowRenderAbles.clear();

		DestroyCaller(m_flowInputRenderAbles);
		m_flowInputRenderAbles.clear();

		DestroyCaller(m_renderAbles);
		m_renderAbles.clear();

		DestroyCaller(m_inputAbles);
		m_inputAbles.clear();

		delete this;
	}
#pragma endregion Flow

#pragma region Add
	void AddFlow(Flow* flow)
	{
		m_flowAbles.push_back(flow);
	}

	void AddRender(Render* render)
	{
		m_renderAbles.push_back(render);
	}

	void AddInput(Input* input)
	{
		m_inputAbles.push_back(input);
	}

	void AddFlowInput(FlowInput* flowInput)
	{
		m_flowInputAbles.push_back(flowInput);
	}

	void AddFlowRender(FlowRender* flowRender)
	{
		m_flowRenderAbles.push_back(flowRender);
	}

	void AddFlowInputRender(FlowInputRender* flowInputRender)
	{
		m_flowInputRenderAbles.push_back(flowInputRender);
	}
#pragma endregion Add

#pragma region Remove
	void RemoveFlow(Flow* flow)
	{
		m_flowAbles.erase(std::remove(m_flowAbles.begin(), m_flowAbles.end(), flow), m_flowAbles.end());
	}

	void RemoveRender(Render* render)
	{
		m_renderAbles.erase(std::remove(m_renderAbles.begin(), m_renderAbles.end(), render), m_renderAbles.end());
	}

	void RemoveInput(Input* input)
	{
		m_inputAbles.erase(std::remove(m_inputAbles.begin(), m_inputAbles.end(), input), m_inputAbles.end());
	}

	void RemoveFlowInput(FlowInput* flowInput)
	{
		m_flowInputAbles.erase(std::remove(m_flowInputAbles.begin(), m_flowInputAbles.end(), flowInput), m_flowInputAbles.end());
	}

	void RemoveFlowRender(FlowRender* flowRender)
	{
		m_flowRenderAbles.erase(std::remove(m_flowRenderAbles.begin(), m_flowRenderAbles.end(), flowRender), m_flowRenderAbles.end());
	}

	void RemoveFlowInputRender(FlowInputRender* flowInputRender)
	{
		m_flowInputRenderAbles.erase(std::remove(m_flowInputRenderAbles.begin(), m_flowInputRenderAbles.end(), flowInputRender), m_flowInputRenderAbles.end());
	}
#pragma endregion Remove

private:
	std::vector<Input*> m_inputAbles;
	std::vector<Flow*> m_flowAbles;
	std::vector<Render*> m_renderAbles;
	std::vector<FlowInput*> m_flowInputAbles;
	std::vector<FlowRender*> m_flowRenderAbles;
	std::vector<FlowInputRender*> m_flowInputRenderAbles;

	template <typename T>
	void ProcessInputCaller(std::vector<T*>& inputAbles, IMouse* mouse, IKeyboard* keyboard)
	{
		for(T* i : inputAbles)
		{
			i->ProcessMousePosition(mouse->GetPosition().x, mouse->GetPosition().y);
			i->ProcessKeyboard(keyboard);
		}
	}

	template <typename T>
	void ProcessMousePositionCaller(std::vector<T*>& inputAbles, double xPos, double yPos)
	{
		for(T* i : inputAbles)
		{
			i->ProcessMousePosition(xPos, yPos);
		}
	}

	template <typename T>
	void ProcessKeyboardCaller(std::vector<T*>& inputAbles, IKeyboard* keyboard)
	{
		for(T* i : inputAbles)
		{
			i->ProcessKeyboard(keyboard);
		}
	}

	template <typename T>
	void AwakeCaller(std::vector<T*>& flowAble)
	{
		for(T* i : flowAble)
		{
			i->Awake();
		}
	}

	template <typename T>
	void StartCaller(std::vector<T*>& flowAble)
	{
		for(T* i : flowAble)
		{
			i->Start();
		}
	}

	template <typename T>
	void UpdateCaller(std::vector<T*>& flowAble, float deltaTime)
	{
		for(T* i : flowAble)
		{
			i->Update(deltaTime);
		}
	}

	template <typename T>
	void LateUpdateCaller(std::vector<T*>& flowAble, float deltaTime)
	{
		for(T* i : flowAble)
		{
			i->LateUpdate(deltaTime);
		}
	}

	template <typename T>
	void DrawCaller(std::vector<T*>& renderAble)
	{
		for(T* i : renderAble)
		{
			i->Draw();
		}
	}

	template <typename T>
	void KeyDownCaller(std::vector<T*>& inputAble, Key key)
	{
		for(T* i : inputAble)
		{
			i->KeyDown(key);
		}
	}

	template <typename T>
	void KeyUpCaller(std::vector<T*>& inputAble, Key key)
	{
		for(T* i : inputAble)
		{
			i->KeyUp(key);
		}
	}

	template <typename T>
	void FixedUpdateCaller(std::vector<T*>& flowAble)
	{
		for(T* i : flowAble)
		{
			i->FixedUpdate();
		}
	}

	template <typename T>
	void DestroyCaller(std::vector<T*>& flowAble)
	{
		for(T* i : flowAble)
		{
			i->Destroy();
		}
	}
#endif
};

} // namespace uknitty