#include "stdafx.h"

#include "imgui.h"
#include "ModeMng.h"
#include "ModeGame.h"
#include "Spawner.h"
#include "EnemyType.h"

#include "DwGui.h"

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
  ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
  if (ImGui::Button("Screenshot")) {
    m_screenshot_req = true;
  }

  // �����X�N���v�g�𖳌���
  ImGui::Checkbox("DisableSpawn", &m_disable_spawn_script); // �X�N���v�g��OFF
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