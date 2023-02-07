OldaleTown_House2_MapScripts::
	.byte 0


TaprootTown_House2_EventScript_BugCatcher::
	lockall
	faceplayer
	msgbox TaprootTown_House2_EventScript_BugCatcher_Text_0
	releaseall
	return


TaprootTown_House2_EventScript_AromaLady::
	lockall
	faceplayer
	msgbox TaprootTown_House2_EventScript_AromaLady_Text_0
	releaseall
	return


TaprootTown_House2_EventScript_BugCatcher_Text_0:
	.string "I love Bug Pokémon. They're super cool!\p"
	.string "There are a lot of them on Route 3, so I\n"
	.string "go looking for them every day.$"

TaprootTown_House2_EventScript_AromaLady_Text_0:
	.string "I really like Grass Pokémon.\p"
	.string "They're very easy to find out Route 4,\n"
	.string "so I go out there and catch some any\l"
	.string "chance I get.$"
