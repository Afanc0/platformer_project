#include "MenuScreen.h"
#include "Utility.h"

#define LEVEL_WIDTH 25
#define LEVEL_HEIGHT 6

unsigned int LEVELD_DATA[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 80, 114, 101, 115, 115, 0, 69, 110, 116, 101, 114, 0, 116, 111, 0, 83, 116, 97, 114, 116, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

MenuScreen::~MenuScreen()
{
    delete[] m_state.enemies;
    delete    m_state.player;
    delete    m_state.map;
}

void MenuScreen::initialise()
{
    m_state.next_scene_id = 0;

    GLuint map_texture_id = Utility::load_texture("assets/font1.png");
    m_state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, LEVELD_DATA, map_texture_id, 0.43f, 16, 16);

    // ————— PLAYER SET-UP ————— //
    m_state.player = new Entity();
    m_state.player->set_entity_type(PLAYER);
    m_state.player->set_position(glm::vec3(5.0f, -5.0f, 0.0f));
    m_state.player->set_movement(glm::vec3(0.0f));
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
    m_state.player->m_jumping_power = 5.0f;

}

void MenuScreen::update(float delta_time, int& m_player_health)
{
    m_state.player->update(delta_time, m_state.player, NULL, 0, m_state.map);

    if (m_state.player->get_position().y < -10.0f) m_state.next_scene_id = 1;
}

void MenuScreen::render(ShaderProgram* program)
{
    m_state.map->render(program);
    m_state.player->render(program);
}