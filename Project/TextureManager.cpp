#include "TextureManager.h"

#include <iostream>
#include <SDL_image.h>
#include <SDL_render.h>

TextureManager::TextureManager() :
	m_pRenderer(nullptr)
{


}



void TextureManager::SetRenderer(SDL_Renderer* renderer)
{
	m_pRenderer = renderer;
}

void TextureManager::Load(std::string id, std::string fileName)
{
	m_textureMap[id] = IMG_LoadTexture(m_pRenderer, fileName.c_str());
	std::cout << SDL_GetError() << std::endl;
}

void TextureManager::Draw(std::string id, int x, int y, double angle, SDL_RendererFlip parFlip)
{

	SDL_Rect srcRect;
	SDL_Rect destRect;
	SDL_QueryTexture(m_textureMap[id], NULL, NULL, &srcRect.w, &srcRect.h);

	srcRect.x = 0;
	srcRect.y = 0;
	destRect.w = srcRect.w;
	destRect.h = srcRect.h;
	destRect.x = x;
	destRect.y = y;

	SDL_RenderCopyEx(m_pRenderer, m_textureMap[id], &srcRect, &destRect, angle, 0, parFlip);

	//SDL_RenderCopy(m_pRenderer, m_textureMap[id], &srcRect, &destRect);
}

void TextureManager::Draw(std::string id, int x, int y, int destW, int destH, double angle, SDL_RendererFlip parFlip)
{
	SDL_Rect srcRect;
	SDL_Rect destRect;
	SDL_QueryTexture(m_textureMap[id], NULL, NULL, &srcRect.w, &srcRect.h);

	srcRect.x = 0;
	srcRect.y = 0;
	destRect.x = x;
	destRect.y = y;
	destRect.w = destW;
	destRect.h = destH;

	SDL_RenderCopyEx(m_pRenderer, m_textureMap[id], &srcRect, &destRect, angle, 0, parFlip);

}

void TextureManager::Clean()
{
	for (auto element : m_textureMap)
	{
		SDL_DestroyTexture(element.second);
	}

	m_pRenderer = nullptr;
}


