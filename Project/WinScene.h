#pragma once
#include "Scene.h"
#include "Sprites.h"

class WinScene : public Scene
{
public:
	WinScene();
	virtual ~WinScene() = default;
	void HandleEvents() override;
	void Update(float deltaTime) override;
	void Render() override;
	bool OnEnter() override;
	bool OnExit() override;


private:
	Button* m_menuButton;
};

