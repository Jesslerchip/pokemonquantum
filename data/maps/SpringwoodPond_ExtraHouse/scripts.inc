SpringwoodPond_ExtraHouse_MapScripts::
	.byte 0


SpringwoodPond_ExtraHouse_EventScript_SakurasWife::
	lock
	faceplayer
	msgbox SpringwoodPond_ExtraHouse_EventScript_SakurasWife_Text_0
	release
	return


SpringwoodPond_ExtraHouse_EventScript_SakurasWife_Text_0:
	.string "Oh, are you looking for my wife? She's\n"
	.string "usually in her Research Institute\l"
	.string "doing some kind of important work. I do\l"
	.string "wish she'd take a break once in a while,\l"
	.string "but she can't help being passionate.$"
