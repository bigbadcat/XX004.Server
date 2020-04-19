local t_attr =
{
	[1]={1, "hp", true, "", 500},
	[2]={2, "eng", true, "", 1000},
	[3]={3, "stb", true, "", 1000},
	[4]={4, "att", false, "", 2000},
	[5]={5, "def", false, "", 1000},
	[6]={6, "hpr", false, "", 500},
	[7]={7, "enr", false, "", 1000},
	[8]={8, "stbr", false, "", 1000},
	[9]={9, "spd", false, "", 0},
	[10]={10, "hit", false, "", 1000},
	[11]={11, "pry", false, "", 1000},
	[12]={12, "pryd", false, "", 1000},
	[13]={13, "crt", false, "", 1000},
	[14]={14, "crtd", false, "", 1000},
	[15]={15, "acrt", false, "", 1000},
	[16]={16, "acrtd", false, "", 1000},
	[17]={17, "suck", false, "", 1000},
	[18]={18, "htb", false, "", 1000},
	[19]={19, "hta", false, "", 1000},
	[20]={20, "hts", false, "", 1000},
	[21]={21, "hp_r", false, "hp", 0},
	[22]={22, "eng_r", false, "eng", 0},
	[23]={23, "stb_r", false, "stb", 0},
	[24]={24, "att_r", false, "att", 0},
	[25]={25, "def_r", false, "def", 0},
	[26]={26, "hpr_r", false, "hpr", 0},
	[27]={27, "enr_r", false, "enr", 0},
	[28]={28, "stbr_r", false, "stbr", 0},
	[29]={29, "spd_r", false, "spd", 0},
}

local keys = {"type", "key", "have_runtime", "add_target", "power_rate"}
local mt = Common.GetKeyMetaTable(keys)
Common.SetConfigMetaTable(t_attr, mt)

return t_attr
