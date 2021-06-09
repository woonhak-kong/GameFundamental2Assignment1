#include "GameStateMachine.h"

#include <iostream>

void GameStateMachine::HandleEvent()
{
	if (!m_gameScene.empty())
	{
		m_gameScene.back()->HandleEvents();
	}
}

void GameStateMachine::Update(float deltaTime)
{
	if (!m_gameScene.empty())
	{
		m_gameScene.back()->Update(deltaTime);
	}
	ExcuteDeleting();
}

void GameStateMachine::Render()
{
	if (!m_gameScene.empty())
	{
		//m_gameScene.back()->Render();
		for (auto& element : m_gameScene)
		{
			element->Render();
		}
	}
}

void GameStateMachine::ChangeState(Scene* scene)
{
	if (!m_gameScene.empty())
	{
		if (m_gameScene.back()->GetCurrentState() != scene->GetCurrentState())
		{
			if (m_gameScene.back()->OnExit())
			{
				m_deletingSceneQueue.push_back(m_gameScene.back());
				m_gameScene.pop_back();
			}
			else
			{
				std::cout << "Error!! - ChangeState\n";
			}
			m_gameScene.push_back(scene);
			m_gameScene.back()->OnEnter();

		}
		else
		{
			delete scene;
		}
	}
}

void GameStateMachine::PushState(Scene* state)
{
	m_gameScene.push_back(state);
	m_gameScene.back()->OnEnter();
}

void GameStateMachine::PopState()
{
	if (!m_gameScene.empty())
	{
		m_gameScene.back()->OnExit();
		m_deletingSceneQueue.push_back(m_gameScene.back());
		m_gameScene.pop_back();
	}
}

void GameStateMachine::ExcuteDeleting()
{
	if (!m_deletingSceneQueue.empty())
	{
		delete m_deletingSceneQueue.back();
		m_deletingSceneQueue.pop_back();
	}
}

void GameStateMachine::Clean()
{
	for (auto element : m_gameScene)
	{
		element->OnExit();
		delete element;

	}
	for (auto element : m_deletingSceneQueue)
	{
		delete element;
	}
	m_gameScene.clear();
	m_deletingSceneQueue.clear();
}
