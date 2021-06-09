#pragma once
#include "Scene.h"
#include "Sprites.h"

class LoseScene : public Scene
{
public:
	LoseScene();
	virtual ~LoseScene() = default;
	void HandleEvents() override;
	void Update(float deltaTime) override;
	void Render() override;
	bool OnEnter() override;
	bool OnExit() override;

private:

	Button* m_menuButton;
};

