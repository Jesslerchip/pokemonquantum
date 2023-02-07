OldaleTown_House1_MapScripts::
	.byte 0


TaprootTown_House1_EventScript_Man::
	lockall
	faceplayer
	msgbox TaprootTown_House1_EventScript_Man_Text_0
	releaseall
	return


TaprootTown_House1_EventScript_Man_Text_0:
	.string "I love Taproot Town, but I'd like to get\n"
	.string "out and journey across Rika.\p"
	.string "I just don't like the cold, so I don't\n"
	.string "have much of an incentive to leave!$"
