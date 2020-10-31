-- -*- coding: utf-8 -*-
package.path = './rom/?.lua;' .. package.path
local matrix = require "vecmat"

local yield=coroutine.yield

function upd_for_second(sec,func)
  while sec > 0 do
    yield(0) --続ける
    if(func)then func() end
    sec = sec - GAME.dt
  end
end

function get_dir(tgt)
  local dir = tgt-Vec2.new(boss.get_position())
  return dir:SetNormalize()
end

------------------------------------------

--
-- 外から呼ばれる、更新関数
--
function update_baby()
  repeat

    local tgt=GAME.decide_target_index()
    upd_for_second(5, function()
      local px,py=get_tgt_pos(tgt)
      boss.move_to(px,py,15)
      boss.use_arms(0, {t=0.2})
    end)

    local dir = get_dir(Vec2.new(get_tgt_pos()))
    upd_for_second(2)
    boss.add_vel_force(dir.x*3,dir.y*3)
    upd_for_second(0.3) -- wait
    --self.arms_timer=0
    upd_for_second(3,function()
      boss.move_to(0,0,50)
      boss.use_arms(0, {t=0.2})
    end)
    --self.arms_timer=0
    upd_for_second(5,function()
      local ofs=boss.elapsed//FRAME2SEC*2
      boss.use_arms(0,{t=0.16,num=35,ofs=ofs})
    end)

  until false -- ずっと続ける
  return 1  -- c++へは、コルーチンが終了したら1を返す
end
