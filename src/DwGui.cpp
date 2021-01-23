#include "stdafx.h"

#if DEBUG
#include "imgui.h"
#include "ModeMng.h"
#include "ModeGame.h"
#include "Spawner.h"
#include "EnemyType.h"
#include "Sound.h"
#include "ObjLst.h"
#include "SaveData.h"

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
  this->show_options_window();
  ImGui::End();
}

bool DwGui::get_disable_spawn_scritp() const {
  return SaveDataMng::deb_conf().m_no_spawn_script;
}
int DwGui::check_spawn_req() {
  int ret = -1;
  if (m_spawn_item_req) ret = SaveDataMng::deb_conf().m_spawn_item_no;
  m_spawn_item_req = false;
  return ret;
}

void DwGui::show_window_internal()
{
  ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
  if (ImGui::Button("Screenshot")) {
    m_screenshot_req = true;
  }

  // 生成スクリプトを無効に
  ImGui::Checkbox("DisableSpawn", &SaveDataMng::deb_conf_w().m_no_spawn_script); // スクリプトをOFF
  {
    const char* items[] = {
#undef ENEMY_TYPE_DECL
#define ENEMY_TYPE_DECL(_id,_cls,_edid)	#_id,

#include "EnemyType.h"

#undef ENEMY_TYPE_DECL
    };
    ImGui::Combo("", &SaveDataMng::deb_conf_w().m_spawn_item_no, items, IM_ARRAYSIZE(items));
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
    ObjLst::inst().del_all_enemies();
  }
//  PlayImpl::singleton().spawner_log_disp();
}

void DwGui::show_options_window()
{
  if (ImGui::CollapsingHeader("Options"))
  {
    auto& snd = Sound::inst();
    auto& save_data = SaveDataMng::inst().get_data_w();
    if (ImGui::SliderFloat("sfx volume", snd.get_sfx_volume_pt(), 0.0f, 1.0f, "vol = %.3f")) {
      snd.apply_sfx_volume();
      save_data.set_vol_sfx(snd.get_sfx_volume());
    }
    if (ImGui::SliderFloat("music volume", snd.get_music_volume_pt(), 0.0f, 1.0f, "vol = %.3f")) {
      snd.apply_music_volume();
      save_data.set_vol_music(snd.get_music_volume());
    }

    //deb conf
    ImGui::Checkbox("BgGuideDisp", &SaveDataMng::deb_conf_w().m_bg_guide_disp); ImGui::SameLine();
    ImGui::Checkbox("NoBossScript", &SaveDataMng::deb_conf_w().m_no_boss_script);

    if (ImGui::Button("Save")) SaveDataMng::inst().write_save_data();
    ImGui::SameLine();
    if (ImGui::Button("ResetDebConf")) SaveDataMng::inst().reset_deb_conf();
  }
}
#endif