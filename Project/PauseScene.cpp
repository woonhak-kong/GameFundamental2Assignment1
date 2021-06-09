#include "PauseScene.h"

#include <iostream>

#include "Game.h"
#include "TextureManager.h"

PauseScene::PauseScene() :
Scene(GameStateE::PAUSE)
{
}

void PauseScene::HandleEvents()
{
}

void PauseScene::Update(float deltaTime)
{
	m_resumeButton->Update();
}

void PauseScene::Render()
{
	SDL_SetRenderDrawColor(Game::GetInstance().GetRenderer(), 0, 255, 0, 100);
	SDL_SetRenderDrawBlendMode(Game::GetInstance().GetRenderer(), SDL_BLENDMODE_BLEND);

	SDL_RenderFillRect(Game::GetInstance().GetRenderer(), &m_rect);
	m_resumeButton->Render();

}

bool PauseScene::OnEnter()
{
	std::cout << "Pause Scene on enter\n";

	m_rect = { Game::kWidth / 10 , Game::kHeight / 10 , Game::kWidth / 10 * 8 , Game::kHeight / 10 * 8 };

	m_resumeButton = new Button(TextureManager::GetInstance().RESUME_BUTTON, { 300, 300, 300, 100 });
	m_resumeButton->AddClickListener([&]() -> void
		{
			Game::GetInstance().PopState();
		});


	return true;
}

bool PauseScene::OnExit()
{
	std::cout << "Pause Scene on exit\n";
	delete m_resumeButton;
	return true;
}
