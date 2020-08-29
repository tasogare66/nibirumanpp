-- -*- coding: utf-8 -*-
--package.path = './data/scr/lua-matrix/?.lua;' .. package.path
--local matrix = require "matrix"
local insert=table.insert
local remove=table.remove
local min,max,abs=math.min,math.max,math.abs
local sin,cos,atan2=math.sin,math.cos,math.atan2
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

------------------------------------------
Spawner.test_co = function(self,args)
  while true do
    if SCRSPW.num<10 then
      --SCRSPW:spawn(random(-170,170),random(-170,170),args.t)
      SCRSPW:spawn(args.t, {dirx=128})
    end
    wait_for_second(0.5)
  end
end

------------------------------------------
function Spawner:registration()
end

function Spawner:init()
  self:runco(Spawner.test_co, {t=EnemyType.GRUNT})
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
