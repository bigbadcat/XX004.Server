------------------------------------------------------------
-- Copyright (c) 2018-2088, By XuXiang all rights reserved.
--
-- FileName: Common.lua
-- Summary: 公共函数。
--
-- Author: XuXiang
-- Date: 2019-01-24 15:44
------------------------------------------------------------

--公共函数集
Common = {}

--获取键值对应的原表
Common.GetKeyMetaTable = function (keys)
	local mt = {}
	mt.KeyToIndex = {}
	for i=1,#keys do
		mt.KeyToIndex[keys[i]] = i
	end
	mt.__index = function (table, key)
		local index = mt.KeyToIndex[key]
		return rawget(table, index)
	end
	return mt
end

--设置配置的元表
Common.SetConfigMetaTable = function (cfgs, mt)
	for i,v in pairs(cfgs) do
		setmetatable(v, mt)
	end
end