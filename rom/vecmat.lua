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


function randf_range(l,r)
  return l+GAME.randomf()*(r-l)
end
function randi(max)
  return GAME.randomi(max)
end
function randi_range(l,r)
  return l+GAME.randomi(r-l);
end
