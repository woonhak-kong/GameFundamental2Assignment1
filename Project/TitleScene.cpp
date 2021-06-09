#include "TitleScene.h"

#include <iostream>

#include "Game.h"
#include "InputHandler.h"
#include "TextureManager.h"

TitleScene::TitleScene() :
	Scene(GameStateE::TITLE)
{
}

void TitleScene::HandleEvents()
{

}

void TitleScene::Update(float deltaTime)
{
	m_startButton->Update();
}

void TitleScene::Render()
{
	m_startButton->Render();

}

bool TitleScene::OnEnter()
{
	std::cout << "Title Scene on enter\n";
	m_startButton = new Button(TextureManager::GetInstance().START_BUTTON, { 300, 300, 300, 100 });
	m_startButton->AddClickListener([&]() -> void
		{
			Game::GetInstance().ChangeState(GameStateE::GAME);
		});

	return true;
}

bool TitleScene::OnExit()
{
	std::cout << "Title Scene on exit\n";
	delete m_startButton;
	return true;
}
