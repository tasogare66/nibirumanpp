#include "stdafx.h"

#include "imgui.h"
#include "ModeMng.h"
#include "ModeGame.h"
#include "Spawner.h"

#include "DwGui.h"

#if DEBUG
namespace dwgui {
  //script�̍ēǂݍ���
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
  // �����X�N���v�g�𖳌���
//  ImGui::Checkbox("DisableScript", PlayImpl::singleton().m_enemy_spawner->pt_disable_script()); // �X�N���v�g��OFF
  // restart
  if (ImGui::Button("Restart")) {
    // �G�̑S�폜
    //PlayImpl::singleton().m_enemy_spawner->request_remove_all();
    // �G�����X�N���v�g�̍ēǂݍ���
    dwgui::setup_spawner_script();
  }
  // spawner script
  if (ImGui::Button("SetupSpawnerScript")) {
    // �G�����X�N���v�g�̍ēǂݍ���
    dwgui::setup_spawner_script();
  }
  ImGui::SameLine();
  if (ImGui::Button("RemoveEnemy")) {
    // �G�̑S�폜
	//PlayImpl::singleton().m_enemy_spawner->request_remove_all();
  }
//  PlayImpl::singleton().spawner_log_disp();
}
#endif