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
  local loop_cnt=0
  repeat
    local sign=(randi(1)==1) and 1 or -1
    local dur=30
    upd_for_second(dur, function(elp)
      local rad=math.rad(elp*sign*90)
      local r = lerp(GAME.LvRadius-10,0,elp/dur)
      local m = matrix_roty(rad)
      local v = matrix { {-r},{0},{1}, }
      local pos = m*v
      if elp>1 and elp<25 then boss.use_arms(2,{t=0.3}) end
      boss.move_to(pos[1][1],pos[2][1],180)
    end)

    local arms_flag = false
    local arms_type=loop_cnt%3
    boss.set_stiffness(0.9);
    boss.set_rot_speed(27,10);
    upd_for_second(20, function(elp)
      boss.move_to(0,0,30)
      if elp<1 then return end
      if arms_type==1 then
        if not arms_flag then --just once
          boss.use_arms(3,{t=0}) --radial spawn
          arms_flag = true
        end
      elseif arms_type==2 then
        boss.use_arms(0, {t=0.45}) --arrow
      else 
        boss.use_arms(1,{t=0.2}) --normal
      end
    end)
    boss.set_stiffness(0.2);
    boss.set_rot_speed(0,3);

    for i=1, 3 do
      local dir = get_dir(Vec2.new(get_tgt_pos()))
      upd_for_second(1)
      boss.add_vel_force(dir.x*(4-i),dir.y*(4-i))
    end

    loop_cnt=loop_cnt+1
  until false -- ずっと続ける
  return 1  -- c++へは、コルーチンが終了したら1を返す
end

-- cog
cog = {}
cog.find_tgt = function()
  local n=GAME.get_target_num()-1
  local tgts={};
  for i=0,n do
    local p = Vec2.new(get_tgt_pos(i))
    if p:SqrMagnitude() > 10000.0 then
      table.insert(tgts,i)
    end
  end
  if (#tgts>=1) then
    return tgts[randi_range(1,#tgts)]
  end
  return nil
end
function update_cog()
  repeat
    boss.set_rot_speed(15,10);
    local dur=20
    local check_interval=4
    local interval=check_interval
    local tgt=nil

    local formation_tbl={dur=4}
    boss.set_formation(0,formation_tbl)
    upd_for_second(dur, function(elp)
      interval=interval-elp
      if interval < 0 then
        --change target
        tgt = cog.find_tgt()
        interval=check_interval
      end

      if tgt then
        local px,py=get_tgt_pos(tgt)
        boss.move_to(px,py,80)
      else
        local rad=math.rad(elp*90)
        local r = lerp(GAME.LvRadius-70,0,elp/dur)
        local m = matrix_roty(rad)
        local v = matrix { {-r},{0},{1}, }
        local pos = m*v
        boss.move_to(pos[1][1],pos[2][1],180)
      end

      boss.use_arms(0, {t=0.30})
    end)

    boss.set_rot_speed(60,4);
    boss.set_formation(1,formation_tbl)
    upd_for_second(dur, function(elp)
      boss.use_arms(0, {t=0.30})
    end)

    boss.set_rot_speed(110,4);
    boss.set_formation(2,formation_tbl)
    upd_for_second(dur, function(elp)
      boss.use_arms(0, {t=0.25})
    end)

    boss.set_rot_speed(60,4);
    boss.set_formation(0,formation_tbl)
    upd_for_second(5, function(elp)
      boss.use_arms(0, {t=0.30}) --arrow
    end)

  until false -- ずっと続ける
  return 1  -- c++へは、コルーチンが終了したら1を返す
end