LittlerootTown_MapScripts::
	.byte 0


SpringwoodPond_EventScript_SpringrockPathSign::
	lockall
	msgbox SpringwoodPond_EventScript_SpringrockPathSign_Text_0
	releaseall
	return


SpringwoodPond_EventScript_PlayersHouse::
	lockall
	msgbox SpringwoodPond_EventScript_PlayersHouse_Text_0
	releaseall
	return


SpringwoodPond_EventScript_SpringwoodPondSign::
	lockall
	msgbox SpringwoodPond_EventScript_SpringwoodPondSign_Text_0
	releaseall
	return


SpringwoodPond_EventScript_BellasHouse::
	lockall
	msgbox SpringwoodPond_EventScript_BellasHouse_Text_0
	releaseall
	return


SpringwoodPond_EventScript_SakurasHouse::
	lockall
	msgbox SpringwoodPond_EventScript_SakurasHouse_Text_0
	releaseall
	return


SpringwoodPond_EventScript_SakurasLab::
	lockall
	msgbox SpringwoodPond_EventScript_SakurasLab_Text_0
	releaseall
	return


SpringwoodPond_EventScript_WaterMan::
	lockall
	faceplayer
	msgbox SpringwoodPond_EventScript_WaterMan_Text_0
	releaseall
	return


SpringwoodPond_EventScript_Skitty::
	lockall
	msgbox SpringwoodPond_EventScript_Skitty_Text_0
	releaseall
	return


SpringwoodPond_EventScript_WomanOnBench::
	lockall
	faceplayer
	msgbox SpringwoodPond_EventScript_WomanOnBench_Text_0
	releaseall
	return


SpringwoodPond_EventScript_ManOnBench::
	lockall
	faceplayer
	msgbox SpringwoodPond_EventScript_ManOnBench_Text_0
	releaseall
	return


SpringwoodPond_EventScript_WomanInFlowers::
	lockall
	faceplayer
	msgbox SpringwoodPond_EventScript_WomanInFlowers_Text_0
	releaseall
	return


SpringwoodPond_EventScript_Scientist::
	lockall
	faceplayer
	msgbox SpringwoodPond_EventScript_Scientist_Text_0
	releaseall
	return


SpringwoodPond_EventScript_CipherPeon1::
	lockall
	faceplayer
	msgbox SpringwoodPond_EventScript_CipherPeon1_Text_0
	releaseall
	return


SpringwoodPond_EventScript_CipherPeon2::
	lockall
	faceplayer
	msgbox SpringwoodPond_EventScript_CipherPeon2_Text_0
	releaseall
	return


SpringwoodPond_EventScript_FatMan::
	lockall
	faceplayer
	goto_if_unset FLAG_SYS_POKEMON_GET, SpringwoodPond_EventScript_FatMan_2
	goto_if_set FLAG_SAKURAS_LAB_CHAOS, SpringwoodPond_EventScript_FatMan_3
	msgbox SpringwoodPond_EventScript_FatMan_Text_2
SpringwoodPond_EventScript_FatMan_1:
	releaseall
	return

SpringwoodPond_EventScript_FatMan_2:
	msgbox SpringwoodPond_EventScript_FatMan_Text_0
	goto SpringwoodPond_EventScript_FatMan_1

SpringwoodPond_EventScript_FatMan_3:
	msgbox SpringwoodPond_EventScript_FatMan_Text_1
	goto SpringwoodPond_EventScript_FatMan_1


SpringwoodPond_EventScript_SakuraNeedsYou::
	lockall
	applymovement 2, Common_Movement_JumpInPlaceDown
	waitmovement 0
	applymovement 2, Common_Movement_JumpInPlaceDown
	waitmovement 0
	msgbox SpringwoodPond_EventScript_SakuraNeedsYou_Text_0
	closemessage
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkUp
	waitmovement 0
	msgbox SpringwoodPond_EventScript_SakuraNeedsYou_Text_1
	releaseall
	return


SpringwoodPond_EventScript_SpringrockPathSign_Text_0:
	.string "Springrock Path$"

SpringwoodPond_EventScript_PlayersHouse_Text_0:
	.string "{PLAYER}'s House$"

SpringwoodPond_EventScript_SpringwoodPondSign_Text_0:
	.string "Springwood Pond\p"
	.string "Home of the famous Prof. Sakura and her\n"
	.string "Research Institute!$"

SpringwoodPond_EventScript_BellasHouse_Text_0:
	.string "Bella's House$"

SpringwoodPond_EventScript_SakurasHouse_Text_0:
	.string "Sakura's House$"

SpringwoodPond_EventScript_SakurasLab_Text_0:
	.string "Prof. Sakura's Research Institute\p"
	.string "Accredited research facility for\n"
	.string "Pokémon science and technology$"

SpringwoodPond_EventScript_WaterMan_Text_0:
	.string "I'm studying the water quality of the\n"
	.string "pond. It's a very important job.\p"
	.string "I come here every day to take notes on\n"
	.string "the Pokémon that live here to help\l"
	.string "Prof. Sakura better understand the\l"
	.string "life cycle of Water Pokémon.$"

SpringwoodPond_EventScript_Skitty_Text_0:
	.string "Skitty: Prrr…\p"
	.string "The Skitty is sleeping. How cute!$"

SpringwoodPond_EventScript_WomanOnBench_Text_0:
	.string "I love Springwood Pond. Everything\n"
	.string "about this place is absolutely\l"
	.string "beautiful!\p"
	.string "It's a long ways from Iridium Bluff, but\n"
	.string "the peaceful scenery is worth it.$"

SpringwoodPond_EventScript_ManOnBench_Text_0:
	.string "Did you know Prof. Sakura was once a\n"
	.string "Pokémon Trainer?\p"
	.string "It's true! She even traveled with\n"
	.string "Champion Steven, from the Hoenn\l"
	.string "region!$"

SpringwoodPond_EventScript_WomanInFlowers_Text_0:
	.string "I'm doing a study on the way Pokémon\n"
	.string "interact with their environments.\p"
	.string "Prof. Sakura suggested flowers like\n"
	.string "these are a vital part of a Grass\l"
	.string "Pokémon's diet.\p"
	.string "I'm putting that to the test!$"

SpringwoodPond_EventScript_Scientist_Text_0:
	.string "Hey, have you heard about the Rika\n"
	.string "League?\p"
	.string "Who am I kidding, of course you have!\n"
	.string "It's only the latest and greatest\l"
	.string "Pokémon League with the strongest\l"
	.string "Trainers in the world!$"

SpringwoodPond_EventScript_CipherPeon1_Text_0:
	.string "Huh?\p"
	.string "Hey, we're trying to scheme here.\n"
	.string "Skedaddle!$"

SpringwoodPond_EventScript_CipherPeon2_Text_0:
	.string "Hey, punk! This doesn't concern you!\p"
	.string "Butt out, or else!$"

SpringwoodPond_EventScript_FatMan_Text_0:
	.string "Yo, {PLAYER}! Prof. Sakura was looking\n"
	.string "for you.\p"
	.string "Don't keep her waiting!$"

SpringwoodPond_EventScript_FatMan_Text_1:
	.string "Yo, {PLAYER}! I just saw some guy\n"
	.string "make off with a hunk of metal.\p"
	.string "Seemed pretty important. They went\n"
	.string "down Route 1, if you're looking for\l"
	.string "them.$"

SpringwoodPond_EventScript_FatMan_Text_2:
	.string "Yo, {PLAYER}! Pokémon Trainers in Rika\n"
	.string "use all sorts of gadgets to teach\l"
	.string "Pokémon moves and unlock special\l"
	.string "powers.\p"
	.string "The power of science is amazing!$"

SpringwoodPond_EventScript_SakuraNeedsYou_Text_0:
	.string "Yo, {PLAYER}! Where are you going?$"

SpringwoodPond_EventScript_SakuraNeedsYou_Text_1:
	.string "I think Prof. Sakura needed your help.\n"
	.string "You should head into the Research\l"
	.string "Institute and talk to her.$"
