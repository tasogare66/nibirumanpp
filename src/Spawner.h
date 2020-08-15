#pragma once

class Spawner {
public:
  Spawner() = default;
  ~Spawner() = default;
  void init();
  void exec(float dt);
private:
};