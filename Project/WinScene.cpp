#include "WinScene.h"

#include <iostream>

#include "Game.h"
#include "TextureManager.h"

WinScene::WinScene() :
	Scene(GameStateE::WIN)
{
}

void WinScene::HandleEvents()
{
}

void WinScene::Update(float deltaTime)
{
	m_menuButton->Update();
}

void WinScene::Render()
{
	TextureManager::GetInstance().Draw(TextureManager::GetInstance().WIN_IMG, 300, 100, 300, 100);
	m_menuButton->Render();
}

bool WinScene::OnEnter()
{
	std::cout << "Win Scene on enter\n";
	m_menuButton = new Button(TextureManager::GetInstance().MENU_BUTTON, { 300, 600, 300, 100 });
	m_menuButton->AddClickListener([&]() -> void
		{
			Game::GetInstance().ChangeState(GameStateE::TITLE);
		});
	return true;
}

bool WinScene::OnExit()
{
	std::cout << "Win Scene on exit\n";

	delete m_menuButton;
	return true;
}
