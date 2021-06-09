#pragma once
#ifndef __TEXTURE_MANAGER__
#define __TEXTURE_MANAGER__
#include <string>
#include <map>
#include <SDL_render.h>

class TextureManager
{
public:
	~TextureManager() =default;

	const std::string START_BUTTON = "startbutton";
	const std::string MENU_BUTTON = "menubutton";
	const std::string RESUME_BUTTON = "resumebutton";
	const std::string EXIT_BUTTON = "exitbutton";
	const std::string DIED_IMG = "died";
	const std::string WIN_IMG = "win";

	static TextureManager& GetInstance()
	{
		static TextureManager instance;
		return instance;
	}

	void SetRenderer(SDL_Renderer* renderer);
	void Load(std::string id, std::string fileName);
	void Draw(std::string id, int x, int y, double angle = 0.0, SDL_RendererFlip parFlip = SDL_FLIP_NONE);
	void Draw(std::string id, int x, int y, int destW, int destH, double angle = 0.0, SDL_RendererFlip parFlip = SDL_FLIP_NONE);

	void Clean();


private:

	TextureManager();

	std::map <std::string, SDL_Texture*> m_textureMap;
	SDL_Renderer* m_pRenderer;


};



#endif // __TEXTURE_MANAGER__