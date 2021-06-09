#pragma once
#include <SDL_rect.h>

#include "Scene.h"
#include "Sprites.h"

class PauseScene : public Scene
{
public:
	PauseScene();
	virtual ~PauseScene() = default;

	void HandleEvents() override;
	void Update(float deltaTime) override;
	void Render() override;
	bool OnEnter() override;
	bool OnExit() override;


private:
	SDL_Rect m_rect;
	Button* m_resumeButton;
};

