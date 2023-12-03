#include "Scene.h"

class LevelB : public Scene {
public:
    int ENEMY_COUNT = 1;

    ~LevelB();

    void initialise() override;
    void update(float delta_time, int& m_player_health) override;
    void render(ShaderProgram* program) override;
};