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

function ene_spawn(t,px,py,dir,prm0)
  local dirx,diry = 0,1
  prm0 = prm0 or 0
  if(dir)then dirx,diry=dir.x,dir.y end
  SCRSPW:spawn(t, {px=px, py=py, dirx=dirx, diry=diry, prm0=prm0})
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

Spawner.circle_co = function(self,args)
  --radius:160まで
  local end_wait=args.end_wait or 0
  local radius = args.radius--60  
  local num = args.num
  local deg_step=360/num
  local dm=args.dirt or 1
  for i=0,num-1 do
    local rad=math.rad(deg_step*i)
    local c=cos(rad)
    local s=sin(rad)
    local m=dm==0 and i%2*2-1 or dm
    ene_spawn(args.t,c*radius,s*radius,Vec2.new(c*m,s*m))
  end
  wait_for_second(end_wait)
end

Spawner.line_co = function(self,args)
  -- rot:rotation_y
  -- step_wait:1体あたりの待ち,default:0
  -- end_wait:全登録あとの待ち
  -- stid:開始のid, 0,1,-8
  local m = matrix_roty(args.rot)
  local stid=args.stid or 1
  local end_wait=args.end_wait or 0
  local step_wait=args.step_wait or 0
  for i=stid, 8 do
    local v = matrix{{i*20},{0},{1}}
    local pos = m*v
    ene_spawn(args.t,pos[1][1],pos[2][1]) --dirなし
    wait_for_second(step_wait)
  end
  wait_for_second(end_wait)
end
Spawner.cross_co = function(self,args)
  local rot = args.rot or randi_range(0,1)*45
  local lst={
    { Spawner.line_co, { t=args.t, rot=rot, step_wait=0, stid=-8 }, 0},
    { Spawner.line_co, { t=args.t, rot=rot+90, step_wait=0 }, 0},
    { Spawner.line_co, { t=args.t, rot=rot-90, step_wait=0 }, 5},
  }
  self:runlst(lst)
end

Spawner.line_intercept_co = function(self,args)
  local end_wait=args.end_wait or 0
  local m = matrix_roty(args.rot)
  local y = args.y
  local width=9
  local r=GAME.LvRadius-8
  local len=sqrt(r*r-y*y)
  local num=len//width
  local ene_args=table.clone(args.args)
  local dir = m*matrix{{ene_args.dir.x},{ene_args.dir.y},{1}}
  ene_args.dir.x,ene_args.dir.y=dir[1][1],dir[2][1]
  for i=-num, num do
    local v = matrix{{i*width},{y},{1}}
    local pos = m*v
    ene_spawn(args.t,pos[1][1],pos[2][1],ene_args.dir)
  end
  wait_for_second(end_wait)
end
Spawner.square_co = function(self,args)
  --y:切片
  --end_wait:終わり待ち
  local ud=1+randi_range(0,1)*(-2)
  for i=0,3 do
    self:runco(Spawner.line_intercept_co, { t=args.t, args={dir=Vec2.new(0,ud)}, y=args.y, rot=90*i+args.rot, end_wait=args.end_wait })
  end
end

function Spawner:line_onr_co(args)
  local m=matrix_roty(args.rot)
  local w=9
  for i=args.sti,args.edi do
    local v = matrix{{i*w},{0},{1}}
    local pos=m*v
    ene_spawn(args.t,pos[1][1],pos[2][1],nil,args.rdir)
  end
end
function Spawner:radial_co(args)
  local num=16
  for i=0,num-1 do
    self:runco(Spawner.line_onr_co,{t=args.t,rot=360/num*i,sti=14,edi=18,rdir=-1+i%2*2})
  end
  wait_for_second(args.end_wait or 0)
end

Spawner.chase_co=function(self,args)
  --num:数
  --spd:追尾速
  local ply_id=GAME.decide_target_index()
  local p=Vec2.new(0,0)
  local dir=Vec2.new()
  for i=1,args.num do
    wait_for_second(args.step_wait,function()
      local px,py=GAME.get_target_position(ply_id)
      dir=Vec2.new(px,py)-p
      dir:SetNormalize()
      p:Add(dir*args.spd*Spawner_dt)
    end)
    ene_spawn(args.t,p.x,p.y,dir)
  end
end

------------------------------------------
function Spawner:registration()
  local tbl = {
---[[
    { Spawner.spiral_co, { t=EnemyType.GRUNT } },
    { Spawner.spiral_co, { t=EnemyType.SNAKE } },
    { Spawner.cross_co, { t=EnemyType.GRUNT } },
    { Spawner.cross_co, { t=EnemyType.SNAKE } },
    { Spawner.circle_co, { t=EnemyType.GRUNT, radius=120,end_wait=4,num=40 } },
    { Spawner.circle_co, { t=EnemyType.SNAKE, radius=120,end_wait=4,num=40 } },
    { Spawner.circle_co, { t=EnemyType.ARROW, radius=85,end_wait=4,num=59,dirt=randi_range(0,2)-1 } },
    { Spawner.random_co, { t=EnemyType.GRUNT, str=60, edr=160, num=75, end_wait=4 }, },
    { Spawner.random_co, { t=EnemyType.GRUNT, edr=60, num=25, end_wait=4 }, },
    { Spawner.random_co, { t=EnemyType.SNAKE, str=60, edr=160, num=75, end_wait=4 }, },
    { Spawner.random_co, { t=EnemyType.SNAKE, edr=60, num=25, end_wait=4 }, },
    { Spawner.random_co, { t=EnemyType.HULK, str=80, edr=160, num=10, end_wait=4 }, },
    { Spawner.square_co, { t=EnemyType.ARROW, y=115, rot=randi_range(0,1)*45, end_wait=4 }, },
    { Spawner.square_co, { t=EnemyType.ARROW2, y=115, rot=randi_range(0,1)*45, end_wait=4 }, },
    { Spawner.chase_co, { t=EnemyType.GRUNT, num=50, step_wait=0.4, spd=35 } },
    { Spawner.chase_co, { t=EnemyType.SNAKE, num=50, step_wait=0.4, spd=35 } },
    { Spawner.chase_co, { t=EnemyType.ARROW, num=50, step_wait=0.4, spd=35 } },
    { Spawner.radial_co, {t=EnemyType.SPHE,end_wait=4} },
--]]
  }

  if SCRSPW.num >= 200 then return end
  if self:num()<1 and #tbl>0 then
    local lot = randi_range(1,#tbl)
    local c=tbl[lot]
    self:runco(c[1],c[2])
  end
	if SCRSPW.ttl >= self.human_step*700 then
		self:runco(Spawner.random_co, {t=EnemyType.HUMAN,num=3})
		self.human_step=self.human_step+1
	end
end

function Spawner:init()
  --self:runco(Spawner.test_co, {t=EnemyType.SNAKE})
  --self:runco(Spawner.random_co, {t=EnemyType.GRUNT, str=60, edr=160, num=75, end_wait=4 })
  --self:runco(Spawner.spiral_co, { t=EnemyType.GRUNT })
  --self:runco(Spawner.circle_co, { t=EnemyType.GRUNT, radius=120,end_wait=4,num=40 })
  --self:runco(Spawner.cross_co, { t=EnemyType.GRUNT })
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
function Spawner:runlst(lst)
  for i=1,#lst do
    local c=lst[i]
    self:runco(c[1],c[2])
    wait_for_second(c[3] or 0)
  end
end
function Spawner:num()
  return #self.lst
end

Spawner.new = function()
  local inst = setmetatable({
    lst={},
    human_step=1,
    boss_step=1,
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
