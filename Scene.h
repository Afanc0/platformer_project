#pragma once
#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL_mixer.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "Utility.h"
#include "Entity.h"
#include "Map.h"

#define VICTORY_TEXT_LEN 7
#define LOSE_TEXT_LEN 8

#define NUMBER_OF_ENEMIES 4

struct GameState
{
    Map* map;
    Entity* player;
    Entity* enemies[NUMBER_OF_ENEMIES];
    Entity* victorytext[VICTORY_TEXT_LEN];
    Entity* losetext[LOSE_TEXT_LEN];


    Mix_Music* bgm;
    Mix_Chunk* jump_sfx;

    int next_scene_id;
};

class Scene {
public:
    int m_number_of_enemies = 1;

    GameState m_state;

    virtual void initialise() = 0;
    virtual void update(float delta_time, int& m_player_health) = 0;
    virtual void render(ShaderProgram* program) = 0;

    GameState const get_state() const { return m_state; }
};