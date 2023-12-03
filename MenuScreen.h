#include "Scene.h"

class MenuScreen : public Scene {
public:
    int ENEMY_COUNT = 1;

    ~MenuScreen();
    
    void initialise() override;
    void update(float delta_time, int& m_player_health) override;
    void render(ShaderProgram* program) override;
};