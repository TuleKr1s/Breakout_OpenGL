#ifndef GAME_H
#define GAME_H

enum GameState {
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};

class Game {
public:

	Game(unsigned int width, unsigned int heigth);
	~Game();

	GameState m_state;
	bool m_keys[1024];
	unsigned int m_width, m_height;

	void init();

	void processInput(float dt);
	void update(float dt);
	void render();

};




#endif
