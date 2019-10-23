local t_const =
{
}

local keys = {"id", "name", "value1", "value2", "value3", "value_str"}
local mt = Common.GetKeyMetaTable(keys)
Common.SetConfigMetaTable(t_const, mt)

return t_const
