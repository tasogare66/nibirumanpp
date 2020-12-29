-- -*- coding: utf-8 -*-
package.path = './rom/?.lua;' .. package.path
local matrix = require "vecmat"

local yield=coroutine.yield

function upd_for_second(sec,func)
  local elp=0
  while sec > 0 do
    yield(0) --続ける
    if(func)then
       if func(elp) then --true返したら終了
        sec=0
      end
    end
    sec = sec - GAME.dt
    elp = elp + GAME.dt
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
-- baby
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

-- worm
function update_worm()
  local radius = 120
  local radius_s = 180
  local deg = 0
  local sign=1
  local add_deg = function(v)
    deg = math.fmod(deg+v,360)
    return math.rad(deg) --radian返す
  end
  local move_to_opposite = function()
    local oprad=add_deg(180)
    upd_for_second(10, function()
      local m = matrix_roty(oprad)
      local v = matrix { {-radius},{0},{1}, }
      local pos = m*v
      return boss.move_to(pos[1][1],pos[2][1],60)<1
    end)
  end

  repeat
    sign = (randi(1)==1) and 1 or -1
    upd_for_second(20, function()
      local rad=add_deg(0.4*sign)
      local r = radius+math.sin(rad*6)*30
      local m = matrix_roty(rad)
      local v = matrix { {-r},{0},{1}, }
      local pos = m*v
      boss.move_to(pos[1][1],pos[2][1],100)
      boss.use_arms(0,{t=1.0})
    end)
    move_to_opposite() --反対へ
    --
    sign = (randi(1)==1) and 1 or -1
    upd_for_second(15, function()
      local rad=add_deg(0.7*sign)
      local m = matrix_roty(rad)
      local v = matrix { {-150},{0},{1}, }
      local pos = m*v
      boss.move_to(pos[1][1],pos[2][1],200)
      boss.use_arms(1,{t=0.8})
    end)
    move_to_opposite() --反対へ

  until false -- ずっと続ける
  return 1  -- c++へは、コルーチンが終了したら1を返す
end

-- urchin
function update_urchin()
  repeat
    local sign=(randi(1)==1) and 1 or -1
    local dur=30
    upd_for_second(dur, function(elp)
      local rad=math.rad(elp*sign*90)
      local r = lerp(GAME.LvRadius-10,0,elp/dur)
      local m = matrix_roty(rad)
      local v = matrix { {-r},{0},{1}, }
      local pos = m*v
      boss.move_to(pos[1][1],pos[2][1],180)
    end)

    boss.set_stiffness(0.9);
    boss.set_rot_speed(30,10);
    upd_for_second(30, function()
      boss.move_to(0,0,15)
      boss.use_arms(0, {t=0.2})
    end)
    boss.set_stiffness(0.2);
    boss.set_rot_speed(0,3);

    for i=1, 3 do
      local dir = get_dir(Vec2.new(get_tgt_pos()))
      upd_for_second(1)
      boss.add_vel_force(dir.x*5,dir.y*5)
    end
 
  until false -- ずっと続ける
  return 1  -- c++へは、コルーチンが終了したら1を返す
end
