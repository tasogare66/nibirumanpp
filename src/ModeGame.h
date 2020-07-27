#include "Mode.h"
class ModeGame : public Mode {
public:
  ModeGame();
  ~ModeGame();
  void init() override;
  void dest() override;
  bool ctrl(float dt) override;
  void ctrl_post() override;
  void draw0(sf::RenderWindow& window) override;
  void draw1(sf::RenderWindow& window) override;
private:
};