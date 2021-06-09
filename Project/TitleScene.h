#pragma once
#ifndef __TITLE_SCENE__
#define __TITLE_SCENE__
#include "Scene.h"
#include "Sprites.h"

class TitleScene : public Scene
{
public:
	TitleScene();
	virtual ~TitleScene() = default;

	void HandleEvents() override;
	void Update(float deltaTime) override;
	void Render() override;
	bool OnEnter() override;
	bool OnExit() override;


private:
	Button* m_startButton;

};

#endif //__TITLE_SCENE__