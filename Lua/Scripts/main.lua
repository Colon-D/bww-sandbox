local UEHelpers = require("UEHelpers")

-- force jump on keybind
local function force_jump()
	local player = UEHelpers:GetPlayerController().Pawn
	-- CanJump needs dll to be forced to work with all costumes
	if player ~= nil and player.CanJump() then
		player.Jump()
	end
end

-- read config file (config_filepath and on_config_loaded set by C++)
local config = { Key = Key }
local f, err = loadfile(config_filepath, "t", config)
if f then
	f()
else
	print("###############################")
	print("##   CONFIG FAILED TO LOAD   ##")
	print("## Sandbox mod will not work ##")
	print("## Error:                    ##")
	print(err)
	print("###############################")
	return
end

on_config_loaded(config)

for _, key in ipairs(config.force_jump_keys) do
	RegisterKeyBind(key, force_jump)
end
