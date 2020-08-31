-- -*- coding: utf-8 -*-
package.path = './rom/?.lua;' .. package.path
local matrix = require "vecmat"

local insert=table.insert
local remove=table.remove
local min,max,abs=math.min,math.max,math.abs
local sin,cos,atan2=math.sin,math.cos,math.atan2
local sqrt = math.sqrt
local yield=coroutine.yield

SCRSPW = ScrSpawner()

local Spawner={}
local Spawner_dt=0
Spawner.__index=Spawner

function wait_for_second(sec,func)
  while sec > 0 do
    yield(1)
    if(func)then func() end
    sec = sec - Spawner_dt
  end
end

function ene_spawn(t,px,py,dir)
  local dirx,diry = 0,1
  if(dir)then dirx,diry=dir.x,dir.y end
  SCRSPW:spawn(t, {px=px, py=py, dirx=dirx, diry=diry})
end

------------------------------------------
Spawner.test_co = function(self,args)
  while true do
    if SCRSPW.num<10 then
      ene_spawn(args.t, randf_range(-170,170), randf_range(-170,170))
    end
    wait_for_second(0.5)
  end
end

function random_circle(num,st_r,ed_r,func)
  local r0=st_r/ed_r
  for i=1,num do
    local r = sqrt(randf_range(r0,1))*ed_r
    local theta = randf_range(-math.pi, math.pi)
    func(r*math.cos(theta),r*math.sin(theta))
  end
end
Spawner.random_co = function(self,args)
  --str:start_radius
  --edr:end_radius
  --num:数
  local r_max=GAME.LvRadius-8
  local st_r=args.str or 0
  local ed_r=args.edr or r_max
  ed_r = math.min(ed_r, r_max)
  local end_wait=args.end_wait or 0
  random_circle(args.num,st_r,ed_r,function(x,y)
    ene_spawn(args.t,x,y)
  end)
  wait_for_second(end_wait)
end

Spawner.spiral_co = function(self,args)
  local radius = 160
  for i=0, 110 do
    radius = radius - 1.5
    local m = matrix_roty(15*i)
    local v = matrix { {-radius},{2},{1}, }
    local pos = m*v
    ene_spawn(args.t,pos[1][1],pos[2][1])
    wait_for_second(0.06)
  end
end

------------------------------------------
function Spawner:registration()
end

function Spawner:init()
  --self:runco(Spawner.test_co, {t=EnemyType.GRUNT})
  --self:runco(Spawner.random_co, {t=EnemyType.GRUNT, str=60, edr=160, num=75, end_wait=4 })
  self:runco(Spawner.spiral_co, { t=EnemyType.GRUNT })
end
function Spawner:exec(dt)
  self:registration()

  Spawner_dt=dt
  for _,val in pairs(self.lst) do
    _,val.resume_flag = coroutine.resume(val.co,self,val.args)
  end
  --終了分を削除
  for i=#self.lst,1,-1 do
    local flg=self.lst[i].resume_flag
    if flg ~= 1 then -- 1(resume中)でないと終了
      if flg ~= nil then
        proj.error(flg)
      end
      table.remove(self.lst, i)
    end
  end
end

function Spawner:create_co(func,args)
  return {
    co=coroutine.create(func),
    args=args,
    resume_flag=1,
  }
end
function Spawner:runco(func,args)
  insert(self.lst,#self.lst+1,Spawner:create_co(func,args))
end
function Spawner:num()
  return #self.lst
end

Spawner.new = function()
  local inst = setmetatable({
    lst={},
  },Spawner)
  return inst
end

--
-- 外から呼ばれる、敵生成関数
--
function spawn_exec()
  local spawner = Spawner.new()
  spawner:init()

  repeat
    spawner:exec(GAME.dt)
    coroutine.yield(0)
  until false

  proj.log("spawn finish.")
  return 1  -- c++へは、コルーチンが終了したら1を返す
end
