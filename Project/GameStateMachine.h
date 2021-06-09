#pragma once
#ifndef __GAME_STATE_MACHINE__
#define __GAME_STATE_MACHINE__
#include "GameStateE.h"
#include <vector>

#include "Scene.h"

class GameStateMachine
{
public:
	GameStateMachine() = default;


	void HandleEvent();
	void Update(float deltaTime);
	void Render();
	void ChangeState(Scene* scene);
	void PushState(Scene* state);
	void PopState();

	void ExcuteDeleting();
	void Clean();


private:

	std::vector<Scene*> m_gameScene;
	std::vector<Scene*> m_deletingSceneQueue;

};


#endif // __GAME_STATE_MACHINE__
