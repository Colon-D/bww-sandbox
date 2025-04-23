local UEHelpers = require("UEHelpers")

local function force_jump()
	local player = UEHelpers:GetPlayerController().Pawn
	-- CanJump needs dll to be forced to work with all costumes
	if player ~= nil and player.CanJump() then
		player.Jump()
	end
end

-- ############################
-- #### SET KEY BINDS HERE ####
-- ############################
-- Available keys: https://docs.ue4ss.com/dev/lua-api/table-definitions/key.html
RegisterKeyBind(Key.SPACE,              force_jump) -- unbind jump in settings
RegisterKeyBind(Key.CAPS_LOCK,          force_jump)
RegisterKeyBind(Key.RIGHT_MOUSE_BUTTON, force_jump)
