local t_attr =
{
	[1]={1, "hp", true, 0, 500},
	[2]={2, "eng", true, 0, 1000},
	[3]={3, "stb", true, 0, 1000},
	[4]={4, "att", false, 0, 2000},
	[5]={5, "def", false, 0, 1000},
	[6]={6, "hit", false, 0, 1000},
	[7]={7, "pry", false, 0, 1000},
	[8]={8, "pryd", false, 0, 1000},
	[9]={9, "crt", false, 0, 1000},
	[10]={10, "crtd", false, 0, 1000},
	[11]={11, "acrt", false, 0, 1000},
	[12]={12, "acrtd", false, 0, 1000},
	[13]={13, "hpr", false, 0, 500},
	[14]={14, "enr", false, 0, 1000},
	[15]={15, "stbr", false, 0, 1000},
	[16]={16, "suck", false, 0, 1000},
	[17]={17, "htb", false, 0, 1000},
	[18]={18, "hta", false, 0, 1000},
	[19]={19, "hts", false, 0, 1000},
	[20]={20, "spd", false, 0, 0},
	[21]={21, "hp_r", false, 1, 0},
	[22]={22, "eng_r", false, 2, 0},
	[23]={23, "stb_r", false, 3, 0},
	[24]={24, "att_r", false, 4, 0},
	[25]={25, "def_r", false, 5, 0},
	[26]={26, "spd_r", false, 20, 0},
}

local keys = {"type", "key", "have_runtime", "add_target", "power_rate"}
local mt = Common.GetKeyMetaTable(keys)
Common.SetConfigMetaTable(t_attr, mt)

return t_attr
