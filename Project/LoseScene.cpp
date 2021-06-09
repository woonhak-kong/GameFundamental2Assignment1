#include "LoseScene.h"

#include <iostream>

#include "Game.h"
#include "TextureManager.h"

LoseScene::LoseScene():
Scene(GameStateE::LOSE)
{
}

void LoseScene::HandleEvents()
{
}

void LoseScene::Update(float deltaTime)
{
	m_menuButton->Update();
}

void LoseScene::Render()
{
	TextureManager::GetInstance().Draw(TextureManager::GetInstance().DIED_IMG, 300, 100, 300, 100);
	m_menuButton->Render();
}

bool LoseScene::OnEnter()
{
	std::cout << "Lose Scene on enter\n";

	m_menuButton = new Button(TextureManager::GetInstance().MENU_BUTTON, { 300, 600, 300, 100 });
	m_menuButton->AddClickListener([&]() -> void
		{
			Game::GetInstance().ChangeState(GameStateE::TITLE);
		});

	return true;
}

bool LoseScene::OnExit()
{
	std::cout << "Lose Scene on exit\n";
	delete m_menuButton;
	return true;
}
