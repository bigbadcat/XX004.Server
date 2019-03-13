local t_prof =
{
	[1]={1, 500, 200, 50},
	[2]={2, 400, 150, 50},
	[3]={3, 300, 100, 20},
	[4]={4, 400, 200, 30},
}

local keys = {"id", "hp", "att", "def"}
local mt = Common.GetKeyMetaTable(keys)
Common.SetConfigMetaTable(t_prof, mt)

return t_prof
