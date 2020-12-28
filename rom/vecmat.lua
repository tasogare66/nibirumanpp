-- Vec2
local sqrt = math.sqrt
local rawset = rawset
local rawget = rawget

Vec2 = {}
setmetatable(Vec2, Vec2)

local fields = {}
Vec2.__index = function(t,k)
	local var = rawget(Vec2, k)
	if var == nil then
		var = rawget(fields, k)
		if var ~= nil then
			return var(t)
		end
	end
	return var
end

function Vec2.new(x, y)
	local v = {x = x or 0, y = y or 0}
	setmetatable(v, Vec2)
	return v
end

function Vec2:Set(x,y)
	self.x = x or 0
	self.y = y or 0
end

function Vec2:Get()
	return self.x, self.y
end

function Vec2:SqrMagnitude()
	return self.x * self.x + self.y * self.y
end

function Vec2:Clone()
	return Vec2.new(self.x, self.y)
end

function Vec2:Normalize()
	local v = self:Clone()
	return v:SetNormalize()
end

function Vec2:SetNormalize()
	local num = self:Magnitude()
	if num == 1 then
		return self
		elseif num > 1e-05 then
				self:Div(num)
		else    
				self:Set(0,0)
	end 
	return self
end

function Vec2.Dot(lhs, rhs)
	return lhs.x * rhs.x + lhs.y * rhs.y
end
function Vec2.Cross(a,b)
	return a.x*b.y - a.y*b.x
end
function Vec2.Angle(a,b)
	local am = a:Magnitude()
	local bm = b:Magnitude()
	return math.acos(clamp(Vec2.Dot(a,b)/(am * bm),-1,1))
end

function Vec2.Magnitude(v2)
	return sqrt(v2.x * v2.x + v2.y * v2.y)
end

function Vec2:Div(d)
	self.x = self.x / d
	self.y = self.y / d
	return self
end

function Vec2:Mul(d)
	self.x = self.x * d
	self.y = self.y * d
	return self
end

function Vec2:Add(b)
	self.x = self.x + b.x
	self.y = self.y + b.y
	return self
end

function Vec2:Sub(b)
	self.x = self.x - b.x
	self.y = self.y - b.y
	return
end

function Vec2:SetRotate(ang)
	local c=cos(ang)
	local s=sin(ang)
	local x,y=self.x,self.y
	self.x = x*c-y*s
	self.y = y*c+x*s
	return self
end
function Vec2:Rotate(ang)
	local v = self:Clone()
	return v:SetRotate(ang)
end

Vec2.__tostring = function(self)
	return string.format("[%f,%f]", self.x, self.y)
end

Vec2.__div = function(va, d)
	return Vec2.new(va.x / d, va.y / d)
end

Vec2.__mul = function(va, d)
	return Vec2.new(va.x * d, va.y * d)
end

Vec2.__add = function(va, vb)
	return Vec2.new(va.x + vb.x, va.y + vb.y)
end

Vec2.__sub = function(va, vb)
	return Vec2.new(va.x - vb.x, va.y - vb.y)
end

Vec2.__unm = function(va)
	return Vec2.new(-va.x, -va.y)
end

Vec2.__eq = function(va,vb)
	return va.x == vb.x and va.y == vb.y
end

fields.up     = function() return Vec2.new(0,1) end
fields.right  = function() return Vec2.new(1,0) end
fields.zero   = function() return Vec2.new(0,0) end
fields.one    = function() return Vec2.new(1,1) end

fields.magnitude    = Vec2.Magnitude
fields.normalized   = Vec2.Normalize
fields.sqrMagnitude = Vec2.SqrMagnitude

-- matrix
local matrix = {_TYPE='module', _NAME='matrix', _VERSION='0.2.11.20120416'}

local matrix_meta = {}

function matrix:new( rows, columns, value )
	if type( rows ) == "table" then
		if type(rows[1]) ~= "table" then -- expect a vector
			return setmetatable( {{rows[1]},{rows[2]},{rows[3]}},matrix_meta )
		end
		return setmetatable( rows,matrix_meta )
	end
	local mtx = {}
	local value = value or 0
	if columns == "I" then
		for i = 1,rows do
			mtx[i] = {}
			for j = 1,rows do
				if i == j then
					mtx[i][j] = 1
				else
					mtx[i][j] = 0
				end
			end
		end
	else
		for i = 1,rows do
			mtx[i] = {}
			for j = 1,columns do
				mtx[i][j] = value
			end
		end
	end
	return setmetatable( mtx,matrix_meta )
end

setmetatable( matrix, { __call = function( ... ) return matrix.new( ... ) end } )

function matrix.add( m1, m2 )
	local mtx = {}
	for i = 1,#m1 do
		local m3i = {}
		mtx[i] = m3i
		for j = 1,#m1[1] do
			m3i[j] = m1[i][j] + m2[i][j]
		end
	end
	return setmetatable( mtx, matrix_meta )
end

function matrix.sub( m1, m2 )
	local mtx = {}
	for i = 1,#m1 do
		local m3i = {}
		mtx[i] = m3i
		for j = 1,#m1[1] do
			m3i[j] = m1[i][j] - m2[i][j]
		end
	end
	return setmetatable( mtx, matrix_meta )
end

function matrix.mul( m1, m2 )
	local mtx = {}
	for i = 1,#m1 do
		mtx[i] = {}
		for j = 1,#m2[1] do
			local num = m1[i][1] * m2[1][j]
			for n = 2,#m1[1] do
				num = num + m1[i][n] * m2[n][j]
			end
			mtx[i][j] = num
		end
	end
	return setmetatable( mtx, matrix_meta )
end

function matrix.mulnum( m1, num )
	local mtx = {}
	for i = 1,#m1 do
		mtx[i] = {}
		for j = 1,#m1[1] do
			mtx[i][j] = m1[i][j] * num
		end
	end
	return setmetatable( mtx, matrix_meta )
end

function matrix.divnum( m1, num )
	local mtx = {}
	for i = 1,#m1 do
		local mtxi = {}
		mtx[i] = mtxi
		for j = 1,#m1[1] do
			mtxi[j] = m1[i][j] / num
		end
	end
	return setmetatable( mtx, matrix_meta )
end

function matrix.type( mtx )
	local e = mtx[1][1]
	if type(e) == "table" then
		if e.type then
			return e:type()
		end
		return "tensor"
	end
	return "number"
end
	
function matrix.rows( mtx )
	return #mtx
end
function matrix.columns( mtx )
	return #mtx[1]
end
function matrix.size( mtx )
	return #mtx,#mtx[1]
end

function matrix.getelement( mtx,i,j )
	if mtx[i] and mtx[i][j] then
		return mtx[i][j]
	end
end

function matrix.setelement( mtx,i,j,value )
	if matrix.getelement( mtx,i,j ) then
		-- check if value type is number
		mtx[i][j] = value
		return 1
	end
end

function matrix.replace( m1, func, ... )
	local mtx = {}
	for i = 1,#m1 do
		local m1i = m1[i]
		local mtxi = {}
		for j = 1,#m1i do
			mtxi[j] = func( m1i[j], ... )
		end
		mtx[i] = mtxi
	end
	return setmetatable( mtx, matrix_meta )
end

matrix_meta.__add = function( ... )
	return matrix.add( ... )
end

matrix_meta.__sub = function( ... )
	return matrix.sub( ... )
end

matrix_meta.__mul = function( m1,m2 )
	if getmetatable( m1 ) ~= matrix_meta then
		return matrix.mulnum( m2,m1 )
	elseif getmetatable( m2 ) ~= matrix_meta then
		return matrix.mulnum( m1,m2 )
	end
	return matrix.mul( m1,m2 )
end

matrix_meta.__unm = function( mtx )
	return matrix.mulnum( mtx,-1 )
end

matrix_meta.__eq = function( m1, m2 )
	-- check same type
	if matrix.type( m1 ) ~= matrix.type( m2 ) then
		return false
	end
	-- check same size
	if #m1 ~= #m2 or #m1[1] ~= #m2[1] then
		return false
	end
	-- check elements equal
	for i = 1,#m1 do
		for j = 1,#m1[1] do
			if m1[i][j] ~= m2[i][j] then
				return false
			end
		end
	end
	return true
end

matrix_meta.__index = {}
for k,v in pairs( matrix ) do
	matrix_meta.__index[k] = v
end

local symbol_meta = {}; symbol_meta.__index = symbol_meta
local symbol = symbol_meta

function symbol_meta.new(o)
	return setmetatable({tostring(o)}, symbol_meta)
end
symbol_meta.to = symbol_meta.new

setmetatable( symbol_meta, { __call = function( _,s ) return symbol_meta.to( s ) end } )

function symbol_meta.tostring( e,fstr )
	return string.format( fstr,e[1] )
end

function symbol_meta:type()
	if getmetatable(self) == symbol_meta then
		return "symbol"
	end
end

function symbol_meta:gsub(from, to)
	return symbol.to( string.gsub( self[1],from,to ) )
end

function symbol_meta.makereplacer( ... )
	local tosub = {}
	local args = {...}
	for i = 1,#args,2 do
		tosub[args[i]] = args[i+1]
		end
	local function func( a ) return tosub[a] or a end
	return function(sym)
		return symbol.to( string.gsub( sym[1], "%a", func ) )
	end
end

function symbol_meta.abs(a)
	return symbol.to("(" .. a[1] .. "):abs()")
end

function symbol_meta.sqrt(a)
	return symbol.to("(" .. a[1] .. "):sqrt()")
end

function symbol_meta.__add(a,b)
	return symbol.to(a .. "+" .. b)
end

function symbol_meta.__sub(a,b)
	return symbol.to(a .. "-" .. b)
end

function symbol_meta.__mul(a,b)
	return symbol.to("(" .. a .. ")*(" .. b .. ")")
end

function symbol_meta.__eq(a,b)
	return a[1] == b[1]
end

function symbol_meta.__tostring(a)
	return a[1]
end

function symbol_meta.__concat(a,b)
	return tostring(a) .. tostring(b)
end

matrix.symbol = symbol


-- utility
FRAME2SEC=1/60

function matrix_roty(rad)
  -- identity matrix with size 4x4
  local m = matrix (3,"I")
  local c = math.cos(rad)
  local s = math.sin(rad)
  m[1][1] = c
  m[1][2] = -s
  m[2][1] = s
  m[2][2] = c
  return m
end

function matrix_roty_deg(deg)
  local rad = math.rad(deg)
  return matrix_roty(rad)
end

function randf_range(l,r)
  return l+GAME.randomf()*(r-l)
end
function randi(max)
  return GAME.randomi(max)
end
function randi_range(l,r)
  return l+GAME.randomi(r-l);
end

function get_tgt_pos(id)
  id = id or GAME.decide_target_index()
  return GAME.get_target_position(id) --px,py
end

function lerp(a,b,t) return a*(1-t) + b*t end

function table.clone(org)
  local copy={}
  for orig_key, orig_value in pairs(org) do
    copy[orig_key] = orig_value
  end
  return copy
end


return matrix