#include "InputHandler.h"

void InputHandler::Update()
{
	SDL_PollEvent(&m_event);
}

SDL_Event& InputHandler::GetCurrentEvent()
{
	return m_event;
}
