-- -*- coding: utf-8 -*-
--package.path = './data/scr/lua-matrix/?.lua;' .. package.path
--local matrix = require "matrix"

--local a = 0
--spawner = Spawner();

local Spawner={}
local Spawner_dt=0
Spawner.__index=Spawner

function Spawner:registration()
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

--
-- 外から呼ばれる、敵生成関数
--
function spawn_exec()
  local cnt=0
  repeat
    --
    cnt=cnt+1
    coroutine.yield(0)
  until false

  proj.log("spawn finish.")
  return 1  -- c++へは、コルーチンが終了したら1を返す
end
