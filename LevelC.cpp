#include "LevelC.h"
#include "Utility.h"

#define LEVEL_WIDTH 31
#define LEVEL_HEIGHT 8

#define VICTORY_TEXT_LEN 7

#define NUMBER_OF_ENEMIES 4

// 183

unsigned int LEVELC_DATA[] =
{
    0, 0, 0, 0, 0, 0, 0, 0, 23, 0, 0, 0, 0, 23, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 297, 0, 0, 0, 0, 0, 23, 0, 0, 0, 0, 5, 0, 0, 0, 0, 24, 25, 26, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 297, 0, 0, 0, 0, 0, 5, 0, 0, 0, 24, 25, 26, 0, 0, 0, 44, 45, 46, 0, 0, 0, 0, 0, 0, 0, 0, 0, 295,
    0, 0, 297, 0, 0, 0, 0, 24, 25, 26, 0, 0, 44, 45, 46, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 295,
    0, 236, 237, 0, 0, 0, 0, 44, 45, 46, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 82, 295,
    297, 58, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 275, 276, 0,
    0, 276, 276, 271, 272, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 275, 277, 0, 0, 0, 275, 277, 0, 0, 0, 275, 276, 0, 0, 0,
    0, 0, 0, 0, 0, 276, 276, 276, 277, 0, 0, 0, 0, 0, 0, 0, 295, 297, 0, 0, 0, 295, 297, 0, 0, 275, 0, 0, 0, 0, 0,
};

LevelC::~LevelC()
{
    //delete[] m_state.enemies;
    delete    m_state.player;
    delete    m_state.map;
    Mix_FreeChunk(m_state.jump_sfx);
    Mix_FreeMusic(m_state.bgm);
}

void LevelC::initialise()
{
    m_state.next_scene_id = -1;

    GLuint map_texture_id = Utility::load_texture("assets/monochrome_tilemap.png");
    m_state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, LEVELC_DATA, map_texture_id, 1.0f, 20, 20);

    // ————— PLAYER SET-UP ————— //
    m_state.player = new Entity();
    m_state.player->set_entity_type(PLAYER);
    m_state.player->set_position(glm::vec3(1.0f, -5.0f, 0.0f));
    m_state.player->set_movement(glm::vec3(0.0f));
    m_state.player->set_speed(2.5f);
    m_state.player->set_acceleration(glm::vec3(0.0f, -9.81f, 0.0f));
    m_state.player->m_texture_id = Utility::load_texture("assets/monochrome_tilemap_transparent.png");

    m_state.player->m_walking[m_state.player->LEFT] = new int[4] { 241, 242, 243, 244 };
    m_state.player->m_walking[m_state.player->RIGHT] = new int[4] { 241, 242, 243, 244 };

    m_state.player->m_animation_indices = m_state.player->m_walking[m_state.player->RIGHT];
    m_state.player->m_animation_time = 0.0f;
    m_state.player->m_animation_frames = 4;
    m_state.player->m_animation_index = 0;
    m_state.player->m_animation_cols = 20;
    m_state.player->m_animation_rows = 20;

    // Jumping
    m_state.player->m_jumping_power = 6.5f;

    // ————— ENEMY SET-UP ————— //

    for (int i = 0; i < NUMBER_OF_ENEMIES; i++) {
        m_state.enemies[i] = new Entity();
        m_state.enemies[i]->set_entity_type(ENEMY);

        m_state.enemies[i]->set_movement(glm::vec3(0.0f));
        m_state.enemies[i]->set_speed(0.5f);
        m_state.enemies[i]->set_acceleration(glm::vec3(0.0f, -9.81f, 0.0f));
        m_state.enemies[i]->m_texture_id = Utility::load_texture("assets/monochrome_tilemap_transparent.png");

        m_state.enemies[i]->m_animation_time = 0.0f;
        m_state.enemies[i]->m_animation_frames = 4;
        m_state.enemies[i]->m_animation_index = 0;
        m_state.enemies[i]->m_animation_cols = 20;
        m_state.enemies[i]->m_animation_rows = 20;
    }

    m_state.enemies[0]->set_ai_type(PATROL);
    m_state.enemies[0]->set_position(glm::vec3(9.0f, -3.0f, 0.0f));
    m_state.enemies[0]->m_walking[m_state.enemies[0]->DOWN] = new int[4] { 321, 322, 323, 324 };
    m_state.enemies[0]->m_animation_indices = m_state.enemies[0]->m_walking[m_state.enemies[0]->DOWN];

    m_state.enemies[1]->set_ai_type(PATROL);
    m_state.enemies[1]->set_ai_state(WALKING);
    m_state.enemies[1]->set_position(glm::vec3(14.0f, -2.0f, 0.0f));
    m_state.enemies[1]->m_walking[m_state.enemies[1]->DOWN] = new int[4] { 341, 342, 343, 344 };
    m_state.enemies[1]->m_animation_indices = m_state.enemies[1]->m_walking[m_state.enemies[1]->DOWN];

    m_state.enemies[2]->set_ai_type(PATROL);
    m_state.enemies[2]->set_ai_state(WALKING);
    m_state.enemies[2]->set_position(glm::vec3(17.0f, -5.0f, 0.0f));
    m_state.enemies[2]->m_walking[m_state.enemies[2]->DOWN] = new int[4] { 361, 362, 363, 364 };
    m_state.enemies[2]->m_animation_indices = m_state.enemies[2]->m_walking[m_state.enemies[2]->DOWN];

    m_state.enemies[3]->set_ai_type(PATROL);
    m_state.enemies[3]->set_ai_state(WALKING);
    m_state.enemies[3]->set_position(glm::vec3(22.0f, -5.0f, 0.0f));
    m_state.enemies[3]->m_walking[m_state.enemies[3]->DOWN] = new int[4] { 361, 362, 363, 364 };
    m_state.enemies[3]->m_animation_indices = m_state.enemies[3]->m_walking[m_state.enemies[3]->DOWN];


    // ————— VICTORY TEXT ————— //
    int win_text_array[] = { 89, 79, 85, 9, 87, 73, 78 };
    float next_pos = 25.0f;
    for (int i = 0; i < VICTORY_TEXT_LEN; i++) {
        m_state.victorytext[i] = new Entity();
        m_state.victorytext[i]->set_position(glm::vec3(++next_pos, -1.0f, 0.0f));

        m_state.victorytext[i]->m_animation_time = 0.0f;
        m_state.victorytext[i]->m_animation_frames = 4;
        m_state.victorytext[i]->m_animation_index = 0;
        m_state.victorytext[i]->m_animation_cols = 16;
        m_state.victorytext[i]->m_animation_rows = 16;

        m_state.victorytext[i]->m_animation_indices = new int[1];
        m_state.victorytext[i]->m_animation_indices[0] = win_text_array[i];
    }

    // ————— LOSE TEXT ————— //
    int lose_text_array[] = { 89, 79, 85, 9, 76, 79, 83, 69 };
    for (int i = 0; i < LOSE_TEXT_LEN; i++) {
        m_state.losetext[i] = new Entity();

        m_state.losetext[i]->m_animation_time = 0.0f;
        m_state.losetext[i]->m_animation_frames = 4;
        m_state.losetext[i]->m_animation_index = 0;
        m_state.losetext[i]->m_animation_cols = 16;
        m_state.losetext[i]->m_animation_rows = 16;

        m_state.losetext[i]->m_animation_indices = new int[1];
        m_state.losetext[i]->m_animation_indices[0] = lose_text_array[i];
    }

        /**
    BGM and SFX
    */
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);

    m_state.bgm = Mix_LoadMUS("assets/music.mp3");
    Mix_PlayMusic(m_state.bgm, -1);
    Mix_VolumeMusic(MIX_MAX_VOLUME / 2.0);

    m_state.jump_sfx = Mix_LoadWAV("assets/jump_sound.wav");
}

void LevelC::update(float delta_time, int& m_player_health)
{
    m_state.player->update(delta_time, m_state.player, NULL, 0, m_state.map);

    for (int i = 0; i < VICTORY_TEXT_LEN; i++) {
        m_state.victorytext[i]->update(delta_time, m_state.victorytext[i], NULL, 0, m_state.map);
    }

    if (m_state.player->get_position().y == -4.0f && m_state.player->get_position().x == 29.0f) {
        m_state.player->deactivate();
        for (int i = 0; i < VICTORY_TEXT_LEN; i++) {
            m_state.victorytext[i]->m_texture_id = Utility::load_texture("assets/font1.png");
        }
    }

    for (int i = 0; i < LOSE_TEXT_LEN; i++) {
        m_state.losetext[i]->update(delta_time, m_state.losetext[i], NULL, 0, m_state.map);
    }

    if (m_player_health == 0)
    {
        m_state.player->deactivate();
        float next_pos = m_state.player->get_position().x;
        for (int i = 0; i < LOSE_TEXT_LEN; i++) {
            m_state.losetext[i]->set_position(glm::vec3(++next_pos, -1.0f, 0.0f));
            m_state.losetext[i]->m_texture_id = Utility::load_texture("assets/font1.png");
        }
    }

    if (m_state.player->get_position().y < -10.0f)
    {
        --m_player_health;
        m_state.player->set_position(glm::vec3(1.0f, -5.0f, 0.0f));
    }

    m_state.enemies[0]->ai_activate(m_state.player, 475);
    m_state.enemies[1]->ai_activate(m_state.player, 475);
    m_state.enemies[2]->ai_activate(m_state.player, 275);
    m_state.enemies[3]->ai_activate(m_state.player, 275);

    for (int i = 0; i < NUMBER_OF_ENEMIES; i++) {
        m_state.enemies[i]->update(delta_time, m_state.enemies[i], NULL, 0, m_state.map);
        if (m_state.player->check_collision(m_state.enemies[i]))
        {
            --m_player_health;
            m_state.player->set_position(glm::vec3(1.0f, -5.0f, 0.0f));
        }
    }
}

void LevelC::render(ShaderProgram* program)
{
    m_state.map->render(program);
    m_state.player->render(program);
    for (int i = 0; i < VICTORY_TEXT_LEN; i++) {
        m_state.victorytext[i]->render(program);
    }
    for (int i = 0; i < LOSE_TEXT_LEN; i++) {
        m_state.losetext[i]->render(program);
    }
    for (int i = 0; i < NUMBER_OF_ENEMIES; i++) {
        m_state.enemies[i]->render(program);
    }
}