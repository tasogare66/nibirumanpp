#include "stdafx.h"

#include "imgui.h"
#include "ModeMng.h"
#include "ModeGame.h"
#include "Spawner.h"
#include "EnemyType.h"

#include "DwGui.h"

namespace dwgui {
  //scriptの再読み込み
  void setup_spawner_script() {
    if (auto* mg = ModeMng::inst().get_current_mode<ModeGame>()) {
      mg->get_spawner_w().reinit();
    }
  }
}

void DwGui::show_window()
{
  ImGui::Begin("game");
  this->show_window_internal();
  ImGui::End();
}

void DwGui::show_window_internal()
{
  ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
  if (ImGui::Button("Screenshot")) {
    m_screenshot_req = true;
  }

  // 生成スクリプトを無効に
  ImGui::Checkbox("DisableSpawn", &m_disable_spawn_script); // スクリプトをOFF
  {
    const char* items[] = {
#undef ENEMY_TYPE_DECL
#define ENEMY_TYPE_DECL(_id,_cls)	#_id,

#include "EnemyType.h"

#undef ENEMY_TYPE_DECL
    };
    ImGui::Combo("", &m_spawn_item_current, items, IM_ARRAYSIZE(items));
    ImGui::SameLine();
    m_spawn_item_req = ImGui::Button("Spawn");
  }
  // restart
  if (ImGui::Button("Restart")) {
    // 敵の全削除
    //PlayImpl::singleton().m_enemy_spawner->request_remove_all();
    // 敵生成スクリプトの再読み込み
    dwgui::setup_spawner_script();
  }
  // spawner script
  if (ImGui::Button("SetupSpawnerScript")) {
    // 敵生成スクリプトの再読み込み
    dwgui::setup_spawner_script();
  }
  ImGui::SameLine();
  if (ImGui::Button("RemoveEnemy")) {
    // 敵の全削除
	//PlayImpl::singleton().m_enemy_spawner->request_remove_all();
  }
//  PlayImpl::singleton().spawner_log_disp();
}