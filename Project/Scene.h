#pragma once
#ifndef __SCENE__
#define __SCENE__
#include "GameStateE.h"

class Scene
{
public:

	Scene(GameStateE state) : m_currentState(state) {}
	virtual ~Scene() = default;

	virtual void HandleEvents() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Render() = 0;
	virtual bool OnEnter() = 0;
	virtual bool OnExit() = 0;

	GameStateE GetCurrentState() const
	{
		return m_currentState;
	}

private:

	GameStateE m_currentState;
};




#endif //__SCENE__