-- -*- coding: utf-8 -*-
package.path = './rom/?.lua;' .. package.path
local matrix = require "vecmat"


--
-- 外から呼ばれる、更新関数
--
function update_baby()
  repeat

    coroutine.yield(0) -- c++へは、resume中は0を返す
  until false -- ずっと続ける
  return 1  -- c++へは、コルーチンが終了したら1を返す
end
