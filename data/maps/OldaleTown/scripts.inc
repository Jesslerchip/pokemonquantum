.set LOCALID_MART_EMPLOYEE, 2
.set LOCALID_FOOTPRINTS_MAN, 3
.set LOCALID_RIVAL, 4

OldaleTown_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, OldaleTown_OnTransition
	.byte 0

OldaleTown_OnTransition:
	call Common_EventScript_SetupRivalGfxId
	setflag FLAG_VISITED_OLDALE_TOWN
	call_if_unset FLAG_ADVENTURE_STARTED, OldaleTown_EventScript_BlockWestEntrance
	call_if_unset FLAG_RECEIVED_POTION_OLDALE, OldaleTown_EventScript_MoveMartEmployee
	call_if_set FLAG_ADVENTURE_STARTED, OldaleTown_EventScript_SetOldaleState
	end

@ This script seems pointless because nothing uses VAR_OLDALE_TOWN_STATE
OldaleTown_EventScript_SetOldaleState::
	return

OldaleTown_EventScript_BlockWestEntrance::
	setobjectxyperm LOCALID_FOOTPRINTS_MAN, 1, 11
	setobjectmovementtype LOCALID_FOOTPRINTS_MAN, MOVEMENT_TYPE_FACE_LEFT
	return

OldaleTown_EventScript_MoveMartEmployee::
	setobjectxyperm LOCALID_MART_EMPLOYEE, 13, 14
	setobjectmovementtype LOCALID_MART_EMPLOYEE, MOVEMENT_TYPE_FACE_DOWN
	return

OldaleTown_EventScript_TownSign::
	msgbox OldaleTown_Text_TownSign, MSGBOX_SIGN
	end

OldaleTown_EventScript_Girl::
	msgbox OldaleTown_Text_SavingMyProgress, MSGBOX_NPC
	end

OldaleTown_EventScript_MartEmployee::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_POTION_OLDALE, OldaleTown_EventScript_ExplainPotion
	goto_if_set FLAG_TEMP_1, OldaleTown_EventScript_ExplainPotion
	setflag FLAG_TEMP_1
	playbgm MUS_FOLLOW_ME, FALSE
	msgbox OldaleTown_Text_IWorkAtPokemonMart, MSGBOX_DEFAULT
	closemessage
	switch VAR_FACING
	case DIR_SOUTH, OldaleTown_EventScript_GoToMartSouth
	case DIR_NORTH, OldaleTown_EventScript_GoToMartNorth
	case DIR_EAST, OldaleTown_EventScript_GoToMartEast
	end

OldaleTown_EventScript_GoToMartSouth::
	applymovement LOCALID_MART_EMPLOYEE, OldaleTown_Movement_EmployeeSouth
	applymovement OBJ_EVENT_ID_PLAYER, OldaleTown_Movement_PlayerSouth
	waitmovement 0
	goto OldaleTown_EventScript_ExplainPokemonMart
	end

OldaleTown_EventScript_GoToMartNorth::
	applymovement LOCALID_MART_EMPLOYEE, OldaleTown_Movement_EmployeeNorth
	applymovement OBJ_EVENT_ID_PLAYER, OldaleTown_Movement_PlayerNorth
	waitmovement 0
	goto OldaleTown_EventScript_ExplainPokemonMart
	end

OldaleTown_EventScript_GoToMartEast::
	applymovement OBJ_EVENT_ID_PLAYER, OldaleTown_Movement_PlayerEast
	applymovement LOCALID_MART_EMPLOYEE, OldaleTown_Movement_EmployeeEast
	waitmovement 0
	goto OldaleTown_EventScript_ExplainPokemonMart
	end

OldaleTown_EventScript_ExplainPokemonMart::
	msgbox OldaleTown_Text_ThisIsAPokemonMart, MSGBOX_DEFAULT
	giveitem ITEM_POTION
	goto_if_eq VAR_RESULT, FALSE, OldaleTown_EventScript_BagIsFull
	msgbox OldaleTown_Text_PotionExplanation, MSGBOX_DEFAULT
	setflag FLAG_RECEIVED_POTION_OLDALE
	fadedefaultbgm
	release
	end

OldaleTown_EventScript_ExplainPotion::
	msgbox OldaleTown_Text_PotionExplanation, MSGBOX_DEFAULT
	release
	end

OldaleTown_EventScript_BagIsFull::
	msgbox gText_TooBadBagIsFull, MSGBOX_DEFAULT
	fadedefaultbgm
	release
	end

OldaleTown_Movement_EmployeeEast:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_in_place_faster_down
	step_end

OldaleTown_Movement_EmployeeSouth:
	walk_left
	walk_up
	walk_up
	walk_right
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_in_place_faster_down
	step_end

OldaleTown_Movement_EmployeeNorth:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_in_place_faster_down
	step_end

OldaleTown_Movement_Unknown1:
	walk_up
	walk_up
	walk_right
	walk_right
	walk_right
	walk_right
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	delay_8
	walk_in_place_faster_down
	step_end

OldaleTown_Movement_PlayerEast:
	walk_right
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

OldaleTown_Movement_PlayerSouth:
	delay_16
	delay_16
	delay_16
	delay_16
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

OldaleTown_Movement_PlayerNorth:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

OldaleTown_Movement_Unknown2:
	walk_left
	walk_up
	walk_up
	walk_right
	walk_right
	walk_right
	walk_right
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

OldaleTown_EventScript_FootprintsMan::
	lock
	faceplayer
	goto_if_set FLAG_ADVENTURE_STARTED, OldaleTown_EventScript_NotBlockingPath
	msgbox OldaleTown_Text_DiscoveredFootprints, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_FOOTPRINTS_MAN, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

OldaleTown_EventScript_BlockedPath::
	lockall
	applymovement OBJ_EVENT_ID_PLAYER, OldaleTown_Movement_PlayerStepBack
	applymovement LOCALID_FOOTPRINTS_MAN, OldaleTown_Movement_BackUp
	waitmovement 0
	msgbox OldaleTown_Text_WaitDontComeInHere, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_FOOTPRINTS_MAN, OldaleTown_Movement_ReturnToOriginalPosition
	waitmovement 0
	releaseall
	end

OldaleTown_EventScript_NotBlockingPath::
	msgbox OldaleTown_Text_FinishedSketchingFootprints, MSGBOX_DEFAULT
	release
	end

OldaleTown_EventScript_Rival::
	lockall
	applymovement LOCALID_RIVAL, Common_Movement_FacePlayer
	waitmovement 0
	setvar VAR_0x8009, 0
	goto OldaleTown_EventScript_ShowRivalMessage
	end

OldaleTown_EventScript_RivalTrigger1::
	lockall
	applymovement LOCALID_RIVAL, OldaleTown_Movement_RivalApproachPlayer1
	waitmovement 0
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	setvar VAR_0x8009, 1
	goto OldaleTown_EventScript_ShowRivalMessage
	end

OldaleTown_EventScript_RivalTrigger2::
	lockall
	applymovement LOCALID_RIVAL, OldaleTown_Movement_RivalApproachPlayer2
	waitmovement 0
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	setvar VAR_0x8009, 1
	goto OldaleTown_EventScript_ShowRivalMessage
	end

OldaleTown_EventScript_RivalTrigger3::
	lockall
	applymovement LOCALID_RIVAL, OldaleTown_Movement_RivalApproachPlayer3
	waitmovement 0
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	setvar VAR_0x8009, 1
	goto OldaleTown_EventScript_ShowRivalMessage
	end

OldaleTown_EventScript_ShowRivalMessage::
	checkplayergender
	goto_if_eq VAR_RESULT, MALE, OldaleTown_EventScript_ShowMayMessage
	goto_if_eq VAR_RESULT, FEMALE, OldaleTown_EventScript_ShowBrendanMessage
	end

OldaleTown_EventScript_ShowMayMessage::
	msgbox OldaleTown_Text_MayLetsGoBack, MSGBOX_DEFAULT
	goto OldaleTown_EventScript_RivalFinish
	end

OldaleTown_EventScript_ShowBrendanMessage::
	msgbox OldaleTown_Text_BrendanLetsGoBack, MSGBOX_DEFAULT
	goto OldaleTown_EventScript_RivalFinish
	end

OldaleTown_EventScript_RivalFinish::
	closemessage
	call_if_eq VAR_0x8009, 0, OldaleTown_EventScript_DoExitMovement1
	call_if_eq VAR_0x8009, 1, OldaleTown_EventScript_DoExitMovement2
	applymovement LOCALID_RIVAL, OldaleTown_Movement_RivalExit
	waitmovement 0
	removeobject LOCALID_RIVAL
	setflag FLAG_HIDE_OLDALE_TOWN_RIVAL
	releaseall
	end

OldaleTown_EventScript_DoExitMovement1::
	goto_if_ne VAR_FACING, DIR_SOUTH, OldaleTown_EventScript_DoExitMovement2
	applymovement LOCALID_RIVAL, OldaleTown_Movement_RivalExit
	waitmovement 0
	return

OldaleTown_EventScript_DoExitMovement2::
	applymovement OBJ_EVENT_ID_PLAYER, OldaleTown_Movement_WatchRivalExit
	applymovement LOCALID_RIVAL, OldaleTown_Movement_RivalExit
	waitmovement 0
	return

OldaleTown_Movement_RivalApproachPlayer1:
	walk_left
	walk_left
	step_end

OldaleTown_Movement_RivalApproachPlayer2:
	walk_left
	step_end

OldaleTown_Movement_RivalApproachPlayer3:
	face_left
	step_end

OldaleTown_Movement_RivalExit:
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

OldaleTown_Movement_WatchRivalExit:
	delay_8
	delay_4
	walk_in_place_faster_down
	step_end

OldaleTown_Movement_PlayerStepBack:
	delay_8
	walk_right
	step_end

OldaleTown_Movement_BackUp:
	walk_fast_up
	walk_in_place_faster_left
	lock_facing_direction
	walk_right
	unlock_facing_direction
	step_end

OldaleTown_Movement_ReturnToOriginalPosition:
	walk_down
	walk_left
	step_end

OldaleTown_Text_SavingMyProgress:
	.string "I want to take a rest, so I'm saving my\n"
	.string "progress.$"

OldaleTown_Text_IWorkAtPokemonMart:
	.string "Hi!\n"
	.string "I work at a POKéMON MART.\p"
	.string "Can I get you to come with me?$"

OldaleTown_Text_ThisIsAPokemonMart:
	.string "This is a POKéMON MART.\n"
	.string "Just look for our blue roof.\p"
	.string "We sell a variety of goods including\n"
	.string "POKé BALLS for catching POKéMON.\p"
	.string "Here, I'd like you to have this as\n"
	.string "a promotional item.$"

OldaleTown_Text_PotionExplanation:
	.string "A POTION can be used anytime, so it's\n"
	.string "even more useful than a POKéMON CENTER\l"
	.string "in certain situations.$"

OldaleTown_Text_WaitDontComeInHere:
	.string "Aaaaah! Wait!\n"
	.string "Please don't come in here.\p"
	.string "I just discovered the footprints of\n"
	.string "a rare POKéMON!\p"
	.string "Wait until I finish sketching\n"
	.string "them, okay?$"

OldaleTown_Text_DiscoveredFootprints:
	.string "I just discovered the footprints of\n"
	.string "a rare POKéMON!\p"
	.string "Wait until I finish sketching\n"
	.string "them, okay?$"

OldaleTown_Text_FinishedSketchingFootprints:
	.string "I finished sketching the footprints of\n"
	.string "a rare POKéMON.\p"
	.string "But it turns out they were only my\n"
	.string "own footprints…$"

OldaleTown_Text_MayLetsGoBack:
	.string "MAY: {PLAYER}{KUN}!\n"
	.string "Over here!\l"
	.string "Let's hurry home!$"

OldaleTown_Text_BrendanLetsGoBack:
	.string "BRENDAN: I'm heading back to my dad's\n"
	.string "LAB now.\l"
	.string "{PLAYER}, you should hustle back, too.$"

OldaleTown_Text_TownSign:
	.string "OLDALE TOWN\n"
	.string "“Where things start off scarce.”$"

TaprootTown_EventScript_TownSign::
	lockall
	msgbox TaprootTown_EventScript_TownSign_Text_0
	releaseall
	return


TaprootTown_EventScript_LockedDoor::
	lockall
	msgbox TaprootTown_EventScript_LockedDoor_Text_0
	releaseall
	return


TaprootTown_EventScript_OldMan::
	lock
	faceplayer
	msgbox TaprootTown_EventScript_OldMan_Text_0
	release
	return


TaprootTown_EventScript_Woman::
	lock
	faceplayer
	msgbox TaprootTown_EventScript_Woman_Text_0
	release
	return


TaprootTown_EventScript_Boy::
	lock
	faceplayer
	msgbox TaprootTown_EventScript_Boy_Text_0
	release
	return


TaprootTown_EventScript_Fisherman::
	lock
	faceplayer
	goto_if_unset FLAG_RECEIVED_OLD_ROD, TaprootTown_EventScript_Fisherman_2
	msgbox TaprootTown_EventScript_Fisherman_Text_1
TaprootTown_EventScript_Fisherman_1:
	release
	return

TaprootTown_EventScript_Fisherman_2:
	msgbox TaprootTown_EventScript_Fisherman_Text_0
	giveitem ITEM_OLD_ROD
	setflag FLAG_RECEIVED_OLD_ROD
	goto TaprootTown_EventScript_Fisherman_1


TaprootTown_EventScript_ItemTM63::
	finditem ITEM_TM63
	return


TaprootTown_EventScript_VilderTrigger1::
	lockall
	playse SE_PIN
	applymovement 6, Common_Movement_ExclamationMark
	waitmovement 0
	playbgm MUS_RG_ENCOUNTER_DEOXYS, 1
	applymovement 6, Common_Movement_WalkInPlaceUp
	waitmovement 0
	msgbox TaprootTown_EventScript_VilderTrigger1_Text_0
	closemessage
	applymovement 6, TaprootTown_Movement_VilderMovementTrigger1
	waitmovement 0
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkInPlaceRight
	waitmovement 0
	msgbox TaprootTown_EventScript_VilderTrigger1_Text_1
	trainerbattle_no_intro TRAINER_VILDER_1, TaprootTown_Text_VilderDefeated
	goto TaprootTown_EventScript_AfterVilderBattle
	return


TaprootTown_EventScript_VilderTrigger2::
	lockall
	playse SE_PIN
	applymovement 6, Common_Movement_ExclamationMark
	waitmovement 0
	playbgm MUS_RG_ENCOUNTER_DEOXYS, 1
	applymovement 6, Common_Movement_WalkInPlaceUp
	waitmovement 0
	msgbox TaprootTown_EventScript_VilderTrigger1_Text_0
	closemessage
	applymovement 6, TaprootTown_Movement_VilderMovementTrigger2
	waitmovement 0
	msgbox TaprootTown_EventScript_VilderTrigger1_Text_1
	trainerbattle_no_intro TRAINER_VILDER_1, TaprootTown_Text_VilderDefeated
	goto TaprootTown_EventScript_AfterVilderBattle
	return


TaprootTown_EventScript_VilderTrigger3::
	lockall
	playse SE_PIN
	applymovement 6, Common_Movement_ExclamationMark
	waitmovement 0
	playbgm MUS_RG_ENCOUNTER_DEOXYS, 1
	applymovement 6, Common_Movement_WalkInPlaceUp
	waitmovement 0
	msgbox TaprootTown_EventScript_VilderTrigger1_Text_0
	closemessage
	applymovement 6, TaprootTown_Movement_VilderMovementTrigger3
	waitmovement 0
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkInPlaceLeft
	waitmovement 0
	msgbox TaprootTown_EventScript_VilderTrigger1_Text_1
	trainerbattle_no_intro TRAINER_VILDER_1, TaprootTown_Text_VilderDefeated
	goto TaprootTown_EventScript_AfterVilderBattle
	return


TaprootTown_EventScript_AfterVilderBattle::
	msgbox TaprootTown_EventScript_AfterVilderBattle_Text_0
	closemessage
	applymovement 6, TaprootTown_EventScript_VilderLeaves
	waitmovement 0
	removeobject 6
	setflag FLAG_HIDE_TAPROOT_TOWN_VILDER
	setvar VAR_TAPROOT_TOWN_STATE, 1
	fadedefaultbgm
	completesubquest QUEST_A_SPOOKY_START, SUB_QUEST_3
	questmenu QUEST_MENU_BUFFER_QUEST_NAME, QUEST_A_SPOOKY_START
	call QuestUpdated
	closemessage
	releaseall
	return


TaprootTown_Movement_VilderMovementTrigger1:
	walk_up
	walk_up
	walk_in_place_left
	step_end

TaprootTown_Movement_VilderMovementTrigger2:
	walk_up
	walk_in_place_up
	step_end

TaprootTown_Movement_VilderMovementTrigger3:
	walk_up
	walk_up
	walk_in_place_right
	step_end

TaprootTown_EventScript_VilderLeaves:
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

TaprootTown_EventScript_TownSign_Text_0:
	.string "Taproot Town\p"
	.string "“The cozy retreat for Trainers and\n"
	.string "travelers alike!”$"

TaprootTown_EventScript_LockedDoor_Text_0:
	.string "The building is locked.\p"
	.string "A sign reads: “Keep out! -- Team\n"
	.string "Quantum”$"

TaprootTown_EventScript_OldMan_Text_0:
	.string "Taproot Town is usually a very peaceful\n"
	.string "place. Tourists come here often.\p"
	.string "Lately, however, there have been some\n"
	.string "shifty characters coming through.\l"
	.string "Some of them are from this thing called\l"
	.string "Cipher, and there's others from a group\l"
	.string "called Team Quantum.\p"
	.string "I'm not sure what they're trying to\n"
	.string "accomplish, but they'd better stop\l"
	.string "ransacking our peaceful little home!$"

TaprootTown_EventScript_Woman_Text_0:
	.string "Did you hear about those artifically\n"
	.string "aggressive Pokémon?\p"
	.string "That Cipher organization has started\n"
	.string "testing them out. I think it's cruel.$"

TaprootTown_EventScript_Boy_Text_0:
	.string "I used to live in that house over there,\n"
	.string "but that nasty Team Quantum gang\l"
	.string "completely trashed it. Now I've moved\l"
	.string "in with my neighbors.$"

TaprootTown_EventScript_Fisherman_Text_0:
	.string "Hey, do you like to fish?\p"
	.string "Yes!\p"
	.string "Yes, you do!\p"
	.string "I know you do!\p"
	.string "You definitely like to fish, don't you?\p"
	.string "Oh, I bet you do!\p"
	.string "Look at that look on your face!\p"
	.string "That's the look of an onlooker that\n"
	.string "says, “Look! I like fishing!”\p"
	.string "Well, have I got a deal for you!\p"
	.string "I'll give you this Old Rod.\p"
	.string "You heard me right!\p"
	.string "It's free!\p"
	.string "Take it!\p"
	.string "Hurry!\p"
	.string "Go on, take it!\p"
	.string "Why are you looking at me like that?\p"
	.string "No, it's not stolen!\p"
	.string "Just take the Old Rod!$"

TaprootTown_EventScript_Fisherman_Text_1:
	.string "You should take care of that Old Rod I\n"
	.string "gave you!\p"
	.string "Can you do that?\p"
	.string "Can you?\p"
	.string "Can you really?\p"
	.string "I sure hope you can!\p"
	.string "Have fun!\p"
	.string "Fishing, that is!\p"
	.string "For Pokémon!\p"
	.string "You know, with that Old Rod!\p"
	.string "The one that isn't stolen!\p"
	.string "Quit looking at me like that!\p"
	.string "Go fish!\p"
	.string "Hurry!$"

TaprootTown_EventScript_VilderTrigger1_Text_0:
	.string "???: Who are you?$"

TaprootTown_EventScript_VilderTrigger1_Text_1:
	.string "???: I see. So, you're {PLAYER}. I'll\n"
	.string "remember that.\p"
	.string "Vilder: My name is Vilder. I'm the most\n"
	.string "powerful Trainer in the world.\p"
	.string "I was hoping the Rika League would\n"
	.string "encourage serious Trainers to come out\l"
	.string "and challenge me, but so far? The whole\l"
	.string "thing's a joke.\p"
	.string "With my Shadow Pokémon, nobody has a\n"
	.string "fighting chance. Soon, the world will\l"
	.string "know my name.\p"
	.string "Vilder: So, you?\p"
	.string "You're just another weakling in my way.\p"
	.string "Let me show you just how weak you are!$"

TaprootTown_EventScript_AfterVilderBattle_Text_0:
	.string "Vilder: Interesting. But I thought…\p"
	.string "You're lucky my Shadow Pokémon haven't\n"
	.string "reached their full power yet,\l"
	.string "{PLAYER}.\p"
	.string "Still, you're not like other Trainers. I'll\n"
	.string "be watching your progress closely.\p"
	.string "Stay out of my way, {PLAYER}.\n"
	.string "Consider this your only warning.$"

TaprootTown_Text_VilderDefeated::
	.string "Hmph. You're tougher than I thought.$"
