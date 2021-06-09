#include "Game.h"
#include "Utilities.h"
#include <iostream>

#include "GameScene.h"
#include "InputHandler.h"
#include "LoseScene.h"
#include "PauseScene.h"
#include "TextureManager.h"
#include "TitleScene.h"
#include "WinScene.h"

Game::Game()
	: m_running(false)
	, m_pWindow(nullptr)
	, m_pRenderer(nullptr)

{

}

int Game::Init(const char* title, int xPos, int yPos)
{
	std::cout << "Init Game ..." << std::endl;
	srand((unsigned)time(nullptr));

	int errorCode = SDL_Init(SDL_INIT_EVERYTHING);
	if (errorCode == 0)
	{
		std::cout << "SDL_Init() succeeded!" << std::endl;
	}
	else
	{
		std::cout << "SDL_Init() failed. Error code: " << errorCode << ": " << SDL_GetError() << std::endl;
		system("pause");
		return errorCode;
	}

	m_pWindow = SDL_CreateWindow(title, xPos, yPos, kWidth, kHeight, 0);
	if (m_pWindow != nullptr)
	{
		std::cout << "SDL_CreateWindow() succeeded!" << std::endl;
	}
	else
	{
		std::cout << "SDL_CreateWindow() failed. Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		system("pause");
		return -1;
	}

	m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, SDL_RENDERER_ACCELERATED);
	TextureManager::GetInstance().SetRenderer(m_pRenderer);
	if (m_pRenderer != nullptr)
	{
		std::cout << "SDL_CreateRenderer() succeeded!" << std::endl;
	}
	else
	{
		std::cout << "SDL_CreateRenderer() failed. Error: " << SDL_GetError() << std::endl;
		SDL_DestroyWindow(m_pWindow);
		SDL_Quit();
		system("pause");
		return -1;
	}

	errorCode = Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024);
	if (errorCode == 0)
	{
		std::cout << "Mix_OpenAudio() succeeded!" << std::endl;
	}
	else
	{
		std::cout << "Mix_OpenAudio() failed. Error: " << Mix_GetError() << std::endl;
		SDL_DestroyWindow(m_pWindow);
		SDL_Quit();
		system("pause");
		return -1;
	}
	int audioFlags = MIX_INIT_MP3;
	errorCode = Mix_Init(audioFlags);
	if (errorCode != audioFlags)
	{
		std::cout << "Mix_OpenAudio() failed. Error: " << Mix_GetError() << std::endl;
		SDL_DestroyWindow(m_pWindow);
		SDL_Quit();
		system("pause");
		return -1;
	}
	std::cout << "Initialization successful!" << std::endl;

	m_running = true;

	// added by Woonhak
	Start();
	return 0;
}

bool Game::IsRunning()
{
	return m_running;
}

void Game::HandleEvents()
{
	InputHandler::GetInstance().Update();

	// added by Woonhak.
	m_pStateMachine->HandleEvent();

	SDL_Event event = InputHandler::GetInstance().GetCurrentEvent();

	switch (event.type)
	{
		case SDL_QUIT: // User pressed window's 'x' button.
			QuitGame();
			break;
		case SDL_KEYDOWN: // Try SDL_KEYUP instead.
			if (event.key.keysym.sym == SDLK_ESCAPE)
				QuitGame();
			break;

	}




}



/* Update is SUPER way too long on purpose! Part of the Assignment 1, if you use
   this program as a start project is to chop up Update and figure out where each
   part of the code is supposed to go. A practice in OOP is to have objects handle
   their own behaviour and this is a big hint for you. */
void Game::Update(float deltaTime)
{


	// added by Woonhak.
	m_pStateMachine->Update(deltaTime);
}

void Game::Render()
{
	SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 255);
	SDL_RenderClear(m_pRenderer); // Clear the screen with the draw color.




	// added by Woonhak.
	m_pStateMachine->Render();

	SDL_RenderPresent(m_pRenderer);
}

void Game::Clean()
{
	std::cout << "Clean ..." << std::endl;
	Mix_CloseAudio();
	Mix_Quit();

	TextureManager::GetInstance().Clean();
	m_pStateMachine->Clean();

	SDL_DestroyRenderer(m_pRenderer);
	SDL_DestroyWindow(m_pWindow);
	SDL_Quit();
}


void Game::Start()
{
	TextureManager::GetInstance().Load(TextureManager::GetInstance().START_BUTTON, "Assets/Images/PlayButton.png");
	TextureManager::GetInstance().Load(TextureManager::GetInstance().MENU_BUTTON, "Assets/Images/MenuButton.png");
	TextureManager::GetInstance().Load(TextureManager::GetInstance().RESUME_BUTTON, "Assets/Images/ResumeButton.png");
	TextureManager::GetInstance().Load(TextureManager::GetInstance().EXIT_BUTTON, "Assets/Images/ExitButton.png");
	TextureManager::GetInstance().Load(TextureManager::GetInstance().DIED_IMG, "Assets/Images/Died.png");
	TextureManager::GetInstance().Load(TextureManager::GetInstance().WIN_IMG, "Assets/Images/Win.png");
	m_pStateMachine = new GameStateMachine();
	PushState(GameStateE::TITLE);


}

void Game::ChangeState(GameStateE newState)
{
	switch (newState)
	{
		case GameStateE::TITLE:
			m_pStateMachine->ChangeState(new TitleScene());
			break;
		case GameStateE::GAME:
			m_pStateMachine->ChangeState(new GameScene());
			break;
		case GameStateE::PAUSE:
			m_pStateMachine->ChangeState(new PauseScene());
			break;
		case GameStateE::WIN:
			m_pStateMachine->ChangeState(new WinScene());
			break;
		case GameStateE::LOSE:
			m_pStateMachine->ChangeState(new LoseScene());
			break;

		default:
			break;
	}


}

void Game::PushState(GameStateE pushState)
{
	switch (pushState)
	{
		case GameStateE::TITLE:
			m_pStateMachine->PushState(new TitleScene());
			break;
		case GameStateE::GAME:
			m_pStateMachine->PushState(new GameScene());
			break;
		case GameStateE::PAUSE:
			m_pStateMachine->PushState(new PauseScene());
			break;
		case GameStateE::WIN:
			m_pStateMachine->PushState(new WinScene());
			break;
		case GameStateE::LOSE:
			m_pStateMachine->PushState(new LoseScene());
			break;

		default:
			break;
	}
}

void Game::PopState()
{
	m_pStateMachine->PopState();
}
