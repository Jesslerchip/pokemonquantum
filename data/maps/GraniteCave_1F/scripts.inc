GraniteCave_1F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, GraniteCave_1F_MapScripts_MAP_SCRIPT_ON_TRANSITION
	.byte 0

GraniteCave_1F_MapScripts_MAP_SCRIPT_ON_TRANSITION:
	call Common_EventScript_SetupRivalGfxId
	end

MtDraco_1F_EventScript_Bella::
	lockall
	msgbox MtDraco_1F_EventScript_Bella_Text_0
	releaseall
	return


MtDraco_1F_EventScript_Skitty::
	lockall
	playmoncry SPECIES_SKITTY, 0
	msgbox MtDraco_1F_EventScript_Skitty_Text_0
	releaseall
	return


MtDraco_1F_EventScript_Poochyena::
	lockall
	playmoncry SPECIES_POOCHYENA, 0
	msgbox MtDraco_1F_EventScript_Poochyena_Text_0
	releaseall
	return


MtDraco_1F_EventScript_PeonRival::
	lockall
	msgbox MtDraco_1F_EventScript_PeonRival_Text_0
	releaseall
	return


MtDraco_1F_EventScript_PeonBattle::
	lockall
	faceplayer
	playbgm MUS_ENCOUNTER_MAGMA, 0
	msgbox MtDraco_1F_EventScript_PeonBattle_Text_0
	trainerbattle_no_intro TRAINER_GRUNT_MAGMA_HIDEOUT_1, MtDraco_1F_TextPeonDefeated
	goto MtDraco_1F_EventScript_AfterPeonBattle
	return


MtDraco_1F_EventScript_AfterPeonBattle::
	lockall
	faceplayer
	playbgm MUS_ENCOUNTER_MAGMA, 0
	msgbox MtDraco_1F_EventScript_AfterPeonBattle_Text_0
	closemessage
	fadescreen FADE_TO_BLACK
	playse SE_EXIT
	fadedefaultbgm
	removeobject 1
	removeobject 3
	removeobject 5
	removeobject 6
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkInPlaceLeft
	waitmovement 0
	fadescreen FADE_FROM_BLACK
	addobject 4
	applymovement 4, MtDraco_1F_Movement_BellaApproach
	waitmovement 0
	msgbox MtDraco_1F_EventScript_AfterPeonBattle_Text_1
	completesubquest QUEST_HELPING_THE_PROFESSOR, SUB_QUEST_3
	questmenu QUEST_MENU_SET_REWARD, QUEST_HELPING_THE_PROFESSOR
	questmenu QUEST_MENU_BUFFER_QUEST_NAME, QUEST_HELPING_THE_PROFESSOR
	call QuestUpdated
	fadescreen FADE_TO_BLACK
	playse SE_EXIT
	setflag FLAG_HIDE_MT_DRACO_1F_OBJECTS
	clearflag FLAG_SAKURAS_LAB_CHAOS
	setvar VAR_LITTLEROOT_INTRO_STATE, 5
	removeobject 4
	fadescreen FADE_FROM_BLACK
	releaseall
	return


MtDraco_1F_Movement_BellaApproach:
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_up
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_in_place_fast_right
	step_end

MtDraco_1F_EventScript_Bella_Text_0:
	.string "Bella: {PLAYER}! You made it!\p"
	.string "I'll hold this one off. You go get that\n"
	.string "machine back!$"

MtDraco_1F_EventScript_Skitty_Text_0:
	.string "Skitty: Skiiii!$"

MtDraco_1F_EventScript_Poochyena_Text_0:
	.string "Poochyena: Grrrr!$"

MtDraco_1F_EventScript_PeonRival_Text_0:
	.string "Come on, Poochyena! We can take this\n"
	.string "kid!$"

MtDraco_1F_EventScript_PeonBattle_Text_0:
	.string "Ha! You came all the way in here for\n"
	.string "that tech we stole?\p"
	.string "I hate to burst your bubble, but it's\n"
	.string "not here. We already sent it off to our\l"
	.string "base with our Corviknite.\p"
	.string "But since you came all this way, I might\n"
	.string "as well teach you not to mess with\l"
	.string "Cipher!$"

MtDraco_1F_EventScript_AfterPeonBattle_Text_0:
	.string "Tch! You'll regret messing with Cipher.\n"
	.string "Our entire organization will be\l"
	.string "notified about you!$"

MtDraco_1F_EventScript_AfterPeonBattle_Text_1:
	.string "Bella: We did it, {PLAYER}! It's too\n"
	.string "bad they got the device, though.\p"
	.string "We should head back to Prof. Sakura. I'm\n"
	.string "sure she'll want to hear from us.$"

MtDraco_1F_TextPeonDefeated::
	.string "What? Beat by a stupid kid?$"
