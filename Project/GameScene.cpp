#include "GameScene.h"

#include <iostream>
#include <SDL_image.h>

#include "Game.h"
#include "InputHandler.h"
#include "Utilities.h"

GameScene::GameScene()
	: Scene(GameStateE::GAME)
	, m_keyStates(nullptr)
	, m_shouldClearEnemies(false)
	, m_shouldClearPlayerBullets(false)
	, m_shouldClearEnemyBullets(false)
	, m_canShoot(true)
	, m_pBGText(nullptr)
	, m_pSpriteTexture(nullptr)
	, m_pPlayer(nullptr)
	, m_enemySpawnTime(0)
	, m_enemySpawnMaxTime(1.5f)
	, m_pivot({ 0,0 })
	, m_pMusic(nullptr)
	, m_numOfKilling(0)
{
}

void GameScene::HandleEvents()
{
	SDL_Event event = InputHandler::GetInstance().GetCurrentEvent();

	switch (event.type)
	{
		case SDL_KEYUP:
			if (event.key.keysym.sym == SDLK_SPACE)
				//std::cout << " space " << std::endl;
			m_canShoot = true;
			break;
	}

}

void GameScene::Update(float deltaTime)
{
	if (Mix_PausedMusic())
		Mix_ResumeMusic();
	// Scroll the backgrounds.
	for (int i = 0; i < kNumberOfBackgrounds; i++)
	{
		m_bgArray[i].GetDestinationTransform()->x -= kBackgroundScrollSpeed * deltaTime;
	}

	// Check if they need to snap back.
	if (m_bgArray[1].GetDestinationTransform()->x <= 0)
	{
		m_bgArray[0].GetDestinationTransform()->x = 0;
		m_bgArray[1].GetDestinationTransform()->x = Game::kWidth;
	}

	// Player animation/movement.
	m_pPlayer->Animate(deltaTime); // Oh! We're telling the player to animate itself. This is good! Hint hint.

	if (KeyDown(SDL_SCANCODE_A) && m_pPlayer->GetDestinationTransform()->x > m_pPlayer->GetDestinationTransform()->h)
	{
		m_pPlayer->GetDestinationTransform()->x -= kPlayerSpeed * deltaTime;
	}
	else if (KeyDown(SDL_SCANCODE_D) && m_pPlayer->GetDestinationTransform()->x < Game::kWidth / 2)
	{
		m_pPlayer->GetDestinationTransform()->x += kPlayerSpeed * deltaTime;
	}
	if (KeyDown(SDL_SCANCODE_W) && m_pPlayer->GetDestinationTransform()->y > 0)
	{
		m_pPlayer->GetDestinationTransform()->y -= kPlayerSpeed * deltaTime;
	}
	else if (KeyDown(SDL_SCANCODE_S) && m_pPlayer->GetDestinationTransform()->y < Game::kHeight - m_pPlayer->GetDestinationTransform()->w)
	{
		m_pPlayer->GetDestinationTransform()->y += kPlayerSpeed * deltaTime;
	}

	if (KeyDown(SDL_SCANCODE_SPACE) && m_canShoot)
	{
		m_canShoot = false;
		m_playerBullets.push_back(
			new Bullet({ kPlayerBulletSourceXPosition, kPlayerBulletSourceYPosition, kPlayerBulletWidth, kPlayerBulletHeight },
				{ m_pPlayer->GetDestinationTransform()->x + kPlayerWidth - kPlayerBulletWidth, m_pPlayer->GetDestinationTransform()->y + kPlayerHeight / 2 - kPlayerBulletWidth, kPlayerBulletWidth, kPlayerBulletHeight },
				kPlayerBulletSpeed));
		Mix_PlayChannel(-1, m_sounds[1], 0);
	}

	// Enemy animation/movement.
	for (size_t i = 0; i < m_enemies.size(); i++)
	{
		m_enemies[i]->Update(deltaTime);
		if (m_enemies[i]->GetDestinationTransform()->x < -m_enemies[i]->GetDestinationTransform()->h)
		{
			delete m_enemies[i];
			m_enemies[i] = nullptr;
			m_shouldClearEnemies = true;
		}
	}
	if (m_shouldClearEnemies)
	{
		CleanVector<Enemy*>(m_enemies, m_shouldClearEnemies);
	}

	// Update enemy spawns.
	m_enemySpawnTime += deltaTime;
	if (m_enemySpawnTime > m_enemySpawnMaxTime)
	{
		// Randomizing enemy bullet spawn rate
		float bulletSpawnRate = 0.5f + (rand() % 3) / 2.0f;
		// Random starting y location
		float yEnemyLocation = (float)(kEnemyHeight + rand() % (Game::kHeight - kEnemyHeight));
		m_enemies.push_back(
			new Enemy({ kEnemySourceXPosition, kEnemySourceYPosition, kEnemyWidth, kEnemyHeight },
				{Game::kWidth, yEnemyLocation, kEnemyWidth, kEnemyHeight },
				&m_enemyBullets,
				m_sounds[0],
				bulletSpawnRate));

		m_enemySpawnTime = 0;
	}

	// Update the bullets. Player's first.
	for (size_t i = 0; i < m_playerBullets.size(); i++)
	{
		m_playerBullets[i]->Update(deltaTime);
		if (m_playerBullets[i]->GetDestinationTransform()->x > Game::kWidth)
		{
			delete m_playerBullets[i];
			m_playerBullets[i] = nullptr;
			m_shouldClearPlayerBullets = true;
		}
	}

	if (m_shouldClearPlayerBullets)
	{
		CleanVector<Bullet*>(m_playerBullets, m_shouldClearPlayerBullets);
	}

	// Now enemy bullets.
	for (size_t i = 0; i < m_enemyBullets.size(); i++)
	{
		m_enemyBullets[i]->Update(deltaTime);
		if (m_enemyBullets[i]->GetDestinationTransform()->x < -m_enemyBullets[i]->GetDestinationTransform()->w)
		{
			delete m_enemyBullets[i];
			m_enemyBullets[i] = nullptr;
			m_shouldClearEnemyBullets = true;
		}
	}

	if (m_shouldClearEnemyBullets)
	{
		CleanVector<Bullet*>(m_enemyBullets, m_shouldClearEnemyBullets);
	}

	if (KeyDown(SDL_SCANCODE_P))
	{
		if (Mix_PlayingMusic())
			Mix_PauseMusic();
		Game::GetInstance().PushState(GameStateE::PAUSE);

	}

	CheckCollision();



}

void GameScene::Render()
{
	// Render stuff. Background first.
	for (int i = 0; i < kNumberOfBackgrounds; i++)
	{
		SDL_RenderCopyF(Game::GetInstance().GetRenderer(), m_pBGText, m_bgArray[i].GetSourceTransform(), m_bgArray[i].GetDestinationTransform());
	}

	// Player.
	SDL_RenderCopyExF(Game::GetInstance().GetRenderer(),
		m_pSpriteTexture,
		m_pPlayer->GetSourceTransform(),
		m_pPlayer->GetDestinationTransform(),
		m_pPlayer->GetAngle(),
		&m_pivot,
		SDL_FLIP_NONE);

	// Player bullets.
	for (size_t i = 0; i < m_playerBullets.size(); i++)
	{
		SDL_RenderCopyExF(Game::GetInstance().GetRenderer(), m_pSpriteTexture, m_playerBullets[i]->GetSourceTransform(), m_playerBullets[i]->GetDestinationTransform(), 90, &m_pivot, SDL_FLIP_NONE);
	}

	// Enemies.
	for (size_t i = 0; i < m_enemies.size(); i++)
	{
		SDL_RenderCopyExF(Game::GetInstance().GetRenderer(), m_pSpriteTexture, m_enemies[i]->GetSourceTransform(), m_enemies[i]->GetDestinationTransform(), -90, &m_pivot, SDL_FLIP_NONE);
	}

	// Enemy bullets.
	for (size_t i = 0; i < m_enemyBullets.size(); i++)
	{
		SDL_RenderCopyF(Game::GetInstance().GetRenderer(), m_pSpriteTexture, m_enemyBullets[i]->GetSourceTransform(), m_enemyBullets[i]->GetDestinationTransform());
	}
}

bool GameScene::OnEnter()
{
	std::cout << "Game Scene on enter\n";
	m_keyStates = SDL_GetKeyboardState(nullptr);

	m_pBGText = IMG_LoadTexture(Game::GetInstance().GetRenderer(), "Assets/Images/background.png");
	m_pSpriteTexture = IMG_LoadTexture(Game::GetInstance().GetRenderer(), "Assets/Images/sprites.png");

	m_bgArray[0] = { {0, 0, Game::kWidth, Game::kHeight}, {0, 0, Game::kWidth, Game::kHeight} };
	m_bgArray[1] = { {0, 0, Game::kWidth, Game::kHeight}, {Game::kWidth, 0, Game::kWidth, Game::kHeight} };

	m_pMusic = Mix_LoadMUS("Assets/Audio/game.mp3"); // Load the music track.
	if (m_pMusic == nullptr)
	{
		std::cout << "Failed to load music: " << Mix_GetError() << std::endl;
	}

	m_sounds.reserve(3);
	Mix_Chunk* sound = Mix_LoadWAV("Assets/Audio/enemy.wav");
	if (sound == nullptr)
	{
		std::cout << "Failed to load sound: " << Mix_GetError() << std::endl;
	}
	m_sounds.push_back(sound);

	sound = Mix_LoadWAV("Assets/Audio/laser.wav");
	if (sound == nullptr)
	{
		std::cout << "Failed to load sound: " << Mix_GetError() << std::endl;
	}
	m_sounds.push_back(sound);

	sound = Mix_LoadWAV("Assets/Audio/explode.wav");
	if (sound == nullptr)
	{
		std::cout << "Failed to load sound: " << Mix_GetError() << std::endl;
	}
	m_sounds.push_back(sound);

	m_pPlayer = new Player({ kPlayerSourceXPosition, kPlayerSourceYPosition, kPlayerWidth, kPlayerHeight }, // Image Source
		{ Game::kWidth / 4 , Game::kHeight / 2 - kPlayerHeight / 2,kPlayerWidth,kPlayerHeight }); // Window Destination

	Mix_PlayMusic(m_pMusic, -1); // Play. -1 = looping.
	Mix_VolumeMusic(16); // 0-MIX_MAX_VOLUME (128).


	return true;
}

bool GameScene::OnExit()
{
	std::cout << "Game Scene on exit\n";

	// Delete Player
	delete m_pPlayer;
	m_pPlayer = nullptr;

	// Destroy Enemies
	for (size_t i = 0; i < m_enemies.size(); i++)
	{
		delete m_enemies[i];
		m_enemies[i] = nullptr;
	}
	m_enemies.clear();

	// Destroy Player Bullets
	for (size_t i = 0; i < m_playerBullets.size(); i++)
	{
		delete m_playerBullets[i];
		m_playerBullets[i] = nullptr;
	}
	m_playerBullets.clear();

	// Destroy Enemy Bullets
	for (size_t i = 0; i < m_enemyBullets.size(); i++)
	{
		delete m_enemyBullets[i];
		m_enemyBullets[i] = nullptr;
	}
	m_enemyBullets.clear();

	// Clean sounds up
	for (size_t i = 0; i < m_sounds.size(); i++)
	{
		Mix_FreeChunk(m_sounds[i]);
	}
	m_sounds.clear();

	Mix_FreeMusic(m_pMusic);

	return true;
}

bool GameScene::KeyDown(SDL_Scancode key)
{
	if (m_keyStates)
	{
		return m_keyStates[key] == 1;
	}
	return false;
}

void GameScene::CheckCollision()
{
	// Player vs. Enemy.
	SDL_Rect playerCollisionRect = { (int)m_pPlayer->GetDestinationTransform()->x - kPlayerHeight, (int)m_pPlayer->GetDestinationTransform()->y, kPlayerHeight, kPlayerWidth };
	for (size_t i = 0; i < m_enemies.size(); i++)
	{
		SDL_Rect enemyCollisionRect = { (int)m_enemies[i]->GetDestinationTransform()->x, (int)m_enemies[i]->GetDestinationTransform()->y - kEnemyWidth, kEnemyHeight, kEnemyWidth };
		if (SDL_HasIntersection(&playerCollisionRect, &enemyCollisionRect))
		{
			// Game over!
			std::cout << "Player goes boom!" << std::endl;
			Mix_PlayChannel(-1, m_sounds[2], 0);
			delete m_enemies[i];
			m_enemies[i] = nullptr;
			m_shouldClearEnemies = true;
			Game::GetInstance().ChangeState(GameStateE::LOSE);
		}
	}

	if (m_shouldClearEnemies)
	{
		CleanVector<Enemy*>(m_enemies, m_shouldClearEnemies);
	}

	// Player bullets vs. Enemies.
	for (size_t i = 0; i < m_playerBullets.size(); i++)
	{
		SDL_Rect bulletCollisionRect = { (int)m_playerBullets[i]->GetDestinationTransform()->x - kPlayerBulletHeight, (int)m_playerBullets[i]->GetDestinationTransform()->y, kPlayerBulletHeight, kPlayerBulletWidth };
		for (size_t j = 0; j < m_enemies.size(); j++)
		{
			if (m_enemies[j] != nullptr)
			{
				SDL_Rect enemyCollisionRect = { (int)m_enemies[j]->GetDestinationTransform()->x, (int)m_enemies[j]->GetDestinationTransform()->y - kEnemyWidth, kEnemyHeight, kEnemyWidth };
				if (SDL_HasIntersection(&bulletCollisionRect, &enemyCollisionRect))
				{
					Mix_PlayChannel(-1, m_sounds[2], 0);
					delete m_enemies[j];
					m_enemies[j] = nullptr;
					delete m_playerBullets[i];
					m_playerBullets[i] = nullptr;
					m_shouldClearEnemies = true;
					m_shouldClearPlayerBullets = true;
					m_numOfKilling++;

					if (m_numOfKilling > 3)
					{
						Game::GetInstance().ChangeState(GameStateE::WIN);
					}
					break;
				}
			}
		}
	}

	if (m_shouldClearEnemies)
	{
		CleanVector<Enemy*>(m_enemies, m_shouldClearEnemies);
	}

	if (m_shouldClearPlayerBullets)
	{
		CleanVector<Bullet*>(m_playerBullets, m_shouldClearPlayerBullets);
	}

	// Enemy bullets vs. player.
	for (size_t i = 0; i < m_enemyBullets.size(); i++)
	{
		SDL_Rect enemyBulletCollisionRect = { (int)m_enemyBullets[i]->GetDestinationTransform()->x, (int)m_enemyBullets[i]->GetDestinationTransform()->y, (int)m_enemyBullets[i]->GetDestinationTransform()->w, (int)m_enemyBullets[i]->GetDestinationTransform()->h };
		if (SDL_HasIntersection(&playerCollisionRect, &enemyBulletCollisionRect))
		{
			// Game over!
			std::cout << "Player goes boom!" << std::endl;
			Mix_PlayChannel(-1, m_sounds[2], 0);
			delete m_enemyBullets[i];
			m_enemyBullets[i] = nullptr;
			m_shouldClearEnemyBullets = true;
			Game::GetInstance().ChangeState(GameStateE::LOSE);
			break;
		}
	}

	if (m_shouldClearEnemyBullets)
	{
		CleanVector<Bullet*>(m_enemyBullets, m_shouldClearEnemyBullets);
	}
}