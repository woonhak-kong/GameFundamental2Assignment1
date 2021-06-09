#pragma once
#ifndef __INPUT_HANDLER__
#define __INPUT_HANDLER__
#include <SDL_events.h>

class InputHandler
{
public:
	static InputHandler& GetInstance()
	{
		static InputHandler instance;
		return instance;
	}


	void Update();

	SDL_Event& GetCurrentEvent();
	//void OnMouseMove(SDL_Event& event);
	//void OnMouseButtonUp(SDL_Event& event);
	//void OnMouseButtonDown(SDL_Event& event);




private:
	InputHandler() = default;
	SDL_Event m_event;

};

#endif // __INPUT_HANDLER__
