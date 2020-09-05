#include "stdafx.h"

#include "imgui.h"
#include "ModeMng.h"
#include "ModeGame.h"
#include "Spawner.h"

#include "DwGui.h"

#if DEBUG
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
  // 生成スクリプトを無効に
//  ImGui::Checkbox("DisableScript", PlayImpl::singleton().m_enemy_spawner->pt_disable_script()); // スクリプトをOFF
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
#endif