local t_character =
{
	[1001]={1001, 1, 500, 200, 50},
	[1002]={1002, 2, 400, 150, 50},
	[1003]={1003, 6, 300, 100, 20},
	[1004]={1004, 6, 400, 200, 30},
	[1005]={1005, 3, 200, 120, 20},
	[1006]={1006, 4, 500, 200, 50},
	[1007]={1007, 3, 300, 150, 50},
	[1008]={1008, 5, 1000, 250, 100},
	[1009]={1009, 3, 400, 200, 50},
}

local keys = {"id", "type", "hp", "att", "def"}
local mt = Common.GetKeyMetaTable(keys)
Common.SetConfigMetaTable(t_character, mt)

return t_character
