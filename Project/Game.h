#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <vector>

#include "GameStateMachine.h"
#include "Scene.h"
#include "Sprites.h"

class Game
{
public:
	static const int kWidth = 1024;
	static const int kHeight = 768;

	static Game& GetInstance()
	{
		static Game instance;
		return instance;
	}


	int Init(const char* title, int xPos, int yPos);
	bool IsRunning();
	void HandleEvents();
	void Update(float deltaTime);
	void Render();
	void Clean();
	void QuitGame() { m_running = false; }



	SDL_Window* GetWindow() { return m_pWindow; }
	SDL_Renderer* GetRenderer() { return m_pRenderer; }

	// added by Woonhak

	void Start();

	void ChangeState(GameStateE newState);
	void PushState(GameStateE pushState);
	void PopState();


private:

	Game();

	bool m_running;
	SDL_Window* m_pWindow;
	SDL_Renderer* m_pRenderer;



	// added by Woonhak
	GameStateMachine* m_pStateMachine;
	//Scene* m_currentScene;
};
