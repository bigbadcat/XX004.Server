local t_prof =
{
	[1]={1, {"stb*100","hit*700","pry*200","pryd*500","crt*200","crtd*2000","enr*10","stbr*1","spd*500"}},
	[2]={2, {"stb*100","hit*650","pry*200","pryd*450","crt*250","crtd*2000","enr*10","stbr*1","spd*500"}},
}

local keys = {"id", "init_attr"}
local mt = Common.GetKeyMetaTable(keys)
Common.SetConfigMetaTable(t_prof, mt)

return t_prof
