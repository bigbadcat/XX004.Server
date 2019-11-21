local t_const =
{
	[1]={1, "power_rate", 1000, 0, 0, ""},
}

local keys = {"id", "name", "value1", "value2", "value3", "value_str"}
local mt = Common.GetKeyMetaTable(keys)
Common.SetConfigMetaTable(t_const, mt)

return t_const
