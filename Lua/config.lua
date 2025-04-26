-- Available keys: https://docs.ue4ss.com/dev/lua-api/table-definitions/key.html
force_jump_keys = {
	Key.SPACE, -- unbind jump in settings
	Key.CAPS_LOCK,
	Key.RIGHT_MOUSE_BUTTON,
}

infinite_costumes = true
unlock_all_costumes = false

volume_multipliers = {
	-- Music
	music = 1.0,
	-- Sound Effects
	se = 1.0,
	-- Voices
	voice = 1.0,
	
	-- Hidden. I'm unsure how these work. Probably multiplied with each other?

	Master = 1.0,

	music_field    = 1.0,
	music_menu     = 1.0,
	music_boss     = 1.0,
	music_cutscene = 1.0,
	music_movie    = 1.0,
	music_jingle   = 1.0,

	se_ingame   = 1.0,
	se_player   = 1.0,
	se_ability  = 1.0,
	se_tim      = 1.0,
	se_enemy    = 1.0,
	se_boss     = 1.0,
	se_npc      = 1.0,
	se_map      = 1.0,
	se_bg       = 1.0,
	se_spot     = 1.0,
	se_gimmick  = 1.0,
	se_system   = 1.0,
	se_cutscene = 1.0,
	se_movie    = 1.0,

	voice_player      = 1.0,
	voice_player_boy  = 1.0,
	voice_player_girl = 1.0,
	voice_tim         = 1.0,
	voice_balan       = 1.0,
	voice_enemy       = 1.0,
	voice_boss        = 1.0,
	voice_npc         = 1.0,
	voice_lance       = 1.0,
	voice_middleboss  = 1.0,

	vibration = 1.0
}
