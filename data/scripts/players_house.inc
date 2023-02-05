@ Local IDs are the same for both players houses
.set LOCALID_MOM, 1
.set LOCALID_DAD, 5
.set LOCALID_MOM_2F, 1

PlayersHouse_2F_EventScript_BlockStairsUntilClockIsSet:: @ 82926FE
	lock
	msgbox PlayersHouse_2F_Text_CheckTheTime, MSGBOX_DEFAULT
	closemessage
	applymovement OBJ_EVENT_ID_PLAYER, PlayersHouse_1F_Movement_SetTime
	waitmovement 0
	goto LittlerootTown_MaysHouse_2F_EventScript_WallClock
	release
	end

PlayersHouse_1F_EventScript_EnterHouseMovingIn:: @ 8292704
	msgbox PlayersHouse_1F_Text_IsntItNiceInHere, MSGBOX_DEFAULT
	applymovement VAR_0x8004, Common_Movement_FacePlayer
	waitmovement 0
	compare VAR_0x8005, MALE
	call_if_eq PlayersHouse_1F_EventScript_MomFacePlayerMovingInMale
	compare VAR_0x8005, FEMALE
	call_if_eq PlayersHouse_1F_EventScript_MomFacePlayerMovingInFemale
	msgbox PlayersHouse_1F_Text_MoversPokemonGoSetClock, MSGBOX_DEFAULT
	closemessage
	setvar VAR_LITTLEROOT_INTRO_STATE, 4
	applymovement OBJ_EVENT_ID_PLAYER, PlayersHouse_1F_Movement_PlayerWalkIn
	applymovement VAR_0x8004, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	releaseall
	end

PlayersHouse_1F_EventScript_MomFacePlayerMovingInMale:: @ 829274D
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	return

PlayersHouse_1F_EventScript_MomFacePlayerMovingInFemale:: @ 8292758
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	return

PlayersHouse_1F_Movement_PlayerWalkIn: @ 8292763
	walk_up
	step_end

PlayersHouse_1F_EventScript_MomGoSeeRoom:: @ 8292765
	msgbox PlayersHouse_1F_Text_ArentYouInterestedInRoom, MSGBOX_DEFAULT
	closemessage
	applymovement VAR_0x8004, Common_Movement_WalkInPlaceFasterUp
	applymovement OBJ_EVENT_ID_PLAYER, PlayersHouse_1F_Movement_MovePlayerAwayFromDoor
	waitmovement 0
	releaseall
	end

LittlerootTown_BrendansHouse_2F_EventScript_WallClock:: @ 8292781
	lockall
	goto PlayersHouse_2F_EventScript_WallClock
	end

LittlerootTown_MaysHouse_2F_EventScript_WallClock:: @ 829278D
	lockall
	goto PlayersHouse_2F_EventScript_WallClock
	end

PlayersHouse_EventSCript_SetClockMale::
	setvar VAR_0x8004, MALE
	goto PlayersHouse_2F_EventScript_WallClock
	end

PlayersHouse_EventScript_SetClockFemale::
	setvar VAR_0x8004, FEMALE
	goto PlayersHouse_2F_EventScript_WallClock
	end

PlayersHouse_2F_EventScript_WallClock:: @ 8292799
	setvar VAR_0x8004, MALE
	goto_if_set FLAG_SET_WALL_CLOCK, PlayersHouse_2F_EventScript_CheckWallClock
	msgbox PlayersHouse_2F_Text_ClockIsStopped, MSGBOX_DEFAULT
	call PlayersHouse_2F_EventScript_SetWallClock
	delay 30
	setvar VAR_LITTLEROOT_INTRO_STATE, 1
	clearflag FLAG_HIDE_LITTLEROOT_TOWN_PLAYERS_BEDROOM_MOM
	setflag FLAG_SET_WALL_CLOCK
	setflag FLAG_HIDE_LITTLEROOT_TOWN_PLAYERS_HOUSE_VIGOROTH_1
	setflag FLAG_HIDE_LITTLEROOT_TOWN_PLAYERS_HOUSE_VIGOROTH_2
	playse SE_EXIT
	call PlayersHouse_2F_EventScript_MomComesUpstairsFemale
	playse SE_EXIT
	setflag FLAG_HIDE_LITTLEROOT_TOWN_PLAYERS_BEDROOM_MOM
	removeobject VAR_0x8008
	releaseall
	end

PlayersHouse_2F_EventScript_MomComesUpstairsMale:: @ 82927DF
	msgbox PlayersHouse_2F_Text_HowDoYouLikeYourRoom, MSGBOX_DEFAULT
	closemessage
	return

PlayersHouse_2F_EventScript_MomComesUpstairsFemale:: @ 829280F
	msgbox PlayersHouse_2F_Text_HowDoYouLikeYourRoom, MSGBOX_DEFAULT
	closemessage
	return

PlayersHouse_2F_Movement_MomLooksLeft:
	walk_in_place_left
	step_end

PlayersHouse_2F_Movement_MomQuestion:
	emote_question_mark
	step_end

PlayersHouse_2F_EventScript_CheckWallClock:: @ 829283F
	incrementgamestat GAME_STAT_CHECKED_CLOCK
	fadescreen FADE_TO_BLACK
	special Special_ViewWallClock
	waitstate
	releaseall
	end

PlayersHouse_2F_EventScript_SetWallClock:: @ 8292849
	fadescreen FADE_TO_BLACK
	special StartWallClock
	waitstate
	return

PlayersHouse_1F_Movement_SetTime:
	delay_8
	walk_right
	walk_right
	walk_in_place_up
	step_end

PlayersHouse_2F_Movement_MomEntersMale: @ 8292850
	delay_8
	walk_down
	walk_in_place_faster_left
	delay_16
	delay_8
	walk_left
	step_end

PlayersHouse_2F_Movement_MomExitsMale: @ 8292857
	walk_right
	walk_up
	delay_8
	step_end

PlayersHouse_2F_Movement_MomEntersFemale: @ 829285B
	delay_8
	walk_down
	walk_in_place_faster_right
	delay_16
	delay_8
	walk_right
	step_end

PlayersHouse_2F_Movement_MomExitsFemale: @ 8292862
	walk_left
	walk_up
	delay_8
	step_end

PlayersHouse_1F_EventScript_SetWatchedBroadcast:: @ 8292866
	setvar VAR_LITTLEROOT_INTRO_STATE, 7
	releaseall
	end

PlayersHouse_1F_EventScript_PetalburgGymReportMale:: @ 829286D
	applymovement VAR_0x8005, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	call PlayersHouse_1F_EventScript_MomNoticeGymBroadcast
	applymovement OBJ_EVENT_ID_PLAYER, PlayersHouse_1F_Movement_PlayerApproachTVForGymMale
	waitmovement 0
	playbgm MUS_ENCOUNTER_INTERVIEWER, 0
	msgbox PlayersHouse_1F_Text_MaybeDadWillBeOn, MSGBOX_DEFAULT
	closemessage
	applymovement VAR_0x8005, PlayersHouse_1F_Movement_MomMakeRoomToSeeTVMale
	waitmovement 0
	applymovement OBJ_EVENT_ID_PLAYER, PlayersHouse_1F_Movement_PlayerMoveToTVMale
	waitmovement 0
	call PlayersHouse_1F_EventScript_WatchGymBroadcast
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	msgbox PlayersHouse_1F_Text_ItsOverWeMissedHim, MSGBOX_DEFAULT
	msgbox PlayersHouse_1F_Text_GoIntroduceYourselfNextDoor, MSGBOX_DEFAULT
	closemessage
	setvar VAR_TEMP_1, 1
	applymovement VAR_0x8005, PlayersHouse_1F_Movement_MomReturnToSeatMale
	waitmovement 0
	goto PlayersHouse_1F_EventScript_SetWatchedBroadcast
	end

PlayersHouse_1F_EventScript_PetalburgGymReportFemale:: @ 82928DC
	applymovement VAR_0x8005, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	call PlayersHouse_1F_EventScript_MomNoticeGymBroadcast
	applymovement OBJ_EVENT_ID_PLAYER, PlayersHouse_1F_Movement_PlayerApproachTVForGymFemale
	waitmovement 0
	playbgm MUS_ENCOUNTER_INTERVIEWER, 0
	msgbox PlayersHouse_1F_Text_MaybeDadWillBeOn, MSGBOX_DEFAULT
	closemessage
	applymovement VAR_0x8005, PlayersHouse_1F_Movement_MomMakeRoomToSeeTVFemale
	waitmovement 0
	applymovement OBJ_EVENT_ID_PLAYER, PlayersHouse_1F_Movement_PlayerMoveToTVFemale
	waitmovement 0
	call PlayersHouse_1F_EventScript_WatchGymBroadcast
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	msgbox PlayersHouse_1F_Text_ItsOverWeMissedHim, MSGBOX_DEFAULT
	msgbox PlayersHouse_1F_Text_GoIntroduceYourselfNextDoor, MSGBOX_DEFAULT
	closemessage
	setvar VAR_TEMP_1, 1
	applymovement VAR_0x8005, PlayersHouse_1F_Movement_MomReturnToSeatFemale
	waitmovement 0
	goto PlayersHouse_1F_EventScript_SetWatchedBroadcast
	end

PlayersHouse_1F_EventScript_MomNoticeGymBroadcast:: @ 829294B
	playse SE_PIN
	applymovement VAR_0x8005, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement VAR_0x8005, Common_Movement_Delay48
	waitmovement 0
	msgbox PlayersHouse_1F_Text_OhComeQuickly, MSGBOX_DEFAULT
	closemessage
	return

PlayersHouse_1F_EventScript_WatchGymBroadcast:: @ 829296C
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	msgbox PlayersHouse_1F_Text_ReportFromPetalburgGym, MSGBOX_DEFAULT
	fadedefaultbgm
	special TurnOffTVScreen
	setflag FLAG_SYS_TV_HOME
	delay 35
	return

PlayersHouse_1F_Movement_MomApproachDadMale: @ 8292989
	walk_up
	walk_right
	walk_right
	walk_right
	walk_right
	walk_down
	walk_in_place_faster_right
	step_end

PlayersHouse_1F_Movement_MomApproachDadFemale: @ 8292991
	walk_up
	walk_left
	walk_left
	walk_left
	walk_left
	walk_down
	walk_in_place_faster_left
	step_end

PlayersHouse_1F_Movement_MomApproachPlayerMale: @ 8292999
	walk_right
	step_end

PlayersHouse_1F_Movement_MomApproachPlayerFemale: @ 829299B
	walk_left
	step_end

PlayersHouse_1F_Movement_MomNoticesLatiBroadcastMale: @ 829299D
	walk_in_place_faster_left
	delay_16
	delay_16
	delay_16
	delay_16
	walk_in_place_faster_right
	delay_16
	delay_16
	step_end

PlayersHouse_1F_Movement_MomNoticesLatiBroadcastFemale: @ 82929A6
	walk_in_place_faster_right
	delay_16
	delay_16
	delay_16
	delay_16
	walk_in_place_faster_left
	delay_16
	delay_16
	step_end

PlayersHouse_1F_Movement_MomApproachPlayerAfterTVMale: @ 82929AF
	walk_up
	walk_left
	walk_left
	step_end

PlayersHouse_1F_Movement_MomApproachPlayerAfterTVFemale: @ 82929B3
	walk_up
	walk_right
	walk_right
	step_end

PlayersHouse_1F_Movement_MomMakeRoomToSeeTVMale: @ 82929B7
	walk_left
	walk_in_place_faster_right
	step_end

PlayersHouse_1F_Movement_MomMakeRoomToSeeTVFemale: @ 82929BA
	walk_right
	walk_in_place_faster_left
	step_end

PlayersHouse_1F_Movement_MomReturnToSeatMale: @ 82929BD
	walk_left
	walk_down
	walk_in_place_faster_right
	step_end

PlayersHouse_1F_Movement_MomReturnToSeatFemale: @ 82929C1
	walk_right
	walk_down
	walk_in_place_faster_left
	step_end

PlayersHouse_1F_EventScript_Mom:: @ 82929C5
	lock
	faceplayer
	compare VAR_LITTLEROOT_HOUSES_STATE_MAY, 4
	goto_if_eq PlayersHouse_1F_EventScript_DontPushYourselfTooHard
	compare VAR_LITTLEROOT_HOUSES_STATE_BRENDAN, 4
	goto_if_eq PlayersHouse_1F_EventScript_DontPushYourselfTooHard
	goto_if_set FLAG_HAS_MATCH_CALL, PlayersHouse_1F_EventScript_TryRegisterMom
	goto_if_set FLAG_RESCUED_BIRCH, PlayersHouse_1F_EventScript_MomHealsParty
	compare VAR_TEMP_1, 1
	goto_if_eq PlayersHouse_1F_EventScript_SeeYouHoney
	compare VAR_LITTLEROOT_INTRO_STATE, 7
	goto_if_eq PlayersHouse_1F_EventScript_DidYouMeetProfBirch
	msgbox PlayersHouse_1F_Text_IsntItNiceInHere, MSGBOX_DEFAULT
	release
	end

PlayersHouse_1F_EventScript_DontPushYourselfTooHard:: @ 8292A0F
	msgbox PlayersHouse_1F_Text_DontPushYourselfTooHard, MSGBOX_DEFAULT
	release
	end

PlayersHouse_1F_EventScript_TryRegisterMom:: @ 8292A19
	goto_if_set FLAG_ENABLE_MOM_MATCH_CALL, PlayersHouse_1F_EventScript_CheckGiveAmuletCoin
	msgbox PlayersHouse_1F_Text_IsThatAPokenav, MSGBOX_DEFAULT
	closemessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox PlayersHouse_1F_Text_RegisteredMom, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_ENABLE_MOM_MATCH_CALL
	release
	end

PlayersHouse_1F_EventScript_CheckGiveAmuletCoin:: @ 8292A43
	goto_if_set FLAG_BADGE05_GET, PlayersHouse_1F_EventScript_TryGiveAmuletCoin
	goto PlayersHouse_1F_EventScript_MomHealsParty

PlayersHouse_1F_EventScript_TryGiveAmuletCoin:: @ 8292A51
	goto_if_set FLAG_RECEIVED_AMULET_COIN, PlayersHouse_1F_EventScript_MomHealsParty
	msgbox PlayersHouse_1F_Text_GotDadsBadgeHeresSomethingFromMom, MSGBOX_DEFAULT
	giveitem ITEM_AMULET_COIN
	compare VAR_RESULT, FALSE
	goto_if_eq Common_EventScript_ShowBagIsFull
	msgbox PlayersHouse_1F_Text_DontPushYourselfTooHard, MSGBOX_DEFAULT
	setflag FLAG_RECEIVED_AMULET_COIN
	release
	end

PlayersHouse_1F_EventScript_MomHealsParty:: @ 8292A86
	msgbox PlayersHouse_1F_Text_YouShouldRestABit, MSGBOX_DEFAULT
	goto PlayersHouse_1F_EventScript_HealParty
	end

PlayersHouse_1F_EventScript_SeeYouHoney:: @ 8292A94
	msgbox PlayersHouse_1F_Text_SeeYouHoney, MSGBOX_DEFAULT
	release
	end

PlayersHouse_1F_EventScript_HealParty:: @ 8292A9E
	closemessage
	call Common_EventScript_OutOfCenterPartyHeal
	incrementgamestat GAME_STAT_RESTED_AT_HOME
	msgbox PlayersHouse_1F_Text_TakeCareHoney, MSGBOX_DEFAULT
	release
	end

PlayersHouse_1F_EventScript_DidYouMeetProfBirch:: @ 8292AB0
	msgbox PlayersHouse_1F_Text_DidYouMeetProfBirch, MSGBOX_DEFAULT
	release
	end

PlayersHouse_1F_EventScript_Vigoroth1:: @ 8292ABA
	lock
	faceplayer
	waitse
	playmoncry SPECIES_VIGOROTH, 0
	msgbox PlayersHouse_1F_Text_Vigoroth1, MSGBOX_DEFAULT
	waitmoncry
	release
	end

PlayersHouse_1F_EventScript_Vigoroth2:: @ 8292ACD
	lock
	faceplayer
	waitse
	playmoncry SPECIES_VIGOROTH, 0
	msgbox PlayersHouse_1F_Text_Vigoroth2, MSGBOX_DEFAULT
	waitmoncry
	release
	end

PlayersHouse_1F_Movement_PlayerApproachTVForGymMale: @ 8292AE0
	walk_down
	walk_down
	walk_left
	walk_left
	walk_left
	step_end

PlayersHouse_1F_Movement_PlayerMoveToTVMale: @ 8292AE6
	walk_left
	step_end

PlayersHouse_1F_Movement_PlayerApproachTVForGymFemale: @ 8292AE8
	walk_down
	walk_down
	walk_right
	walk_right
	walk_right
	step_end

PlayersHouse_1F_Movement_PlayerMoveToTVFemale: @ 8292AEE
	walk_right
	step_end

PlayersHouse_1F_Movement_MovePlayerAwayFromDoor: @ 8292AF0
	walk_up
	step_end

PlayersHouse_1F_EventScript_GetSSTicketAndSeeLatiTV:: @ 8292AF2
	lockall
	checkplayergender
	compare VAR_RESULT, MALE
	call_if_eq PlayersHouse_1F_EventScript_SetUpObjectEventVarsMale
	compare VAR_RESULT, FEMALE
	call_if_eq PlayersHouse_1F_EventScript_SetUpObjectEventVarsFemale
	compare VAR_0x8008, MALE
	call_if_eq PlayersHouse_1F_EventScript_PlayerEnterRoomMale
	compare VAR_0x8008, FEMALE
	call_if_eq PlayersHouse_1F_EventScript_PlayerEnterRoomFemale
	applymovement VAR_0x8009, Common_Movement_FacePlayer
	waitmovement 0
	playse SE_PIN
	applymovement VAR_0x8009, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement VAR_0x8009, Common_Movement_Delay48
	waitmovement 0
	delay 20
	compare VAR_0x8008, MALE
	call_if_eq PlayersHouse_1F_EventScript_DadApproachPlayerMale
	compare VAR_0x8008, FEMALE
	call_if_eq PlayersHouse_1F_EventScript_DadApproachPlayerFemale
	msgbox PlayersHouse_1F_Text_TicketFromBrineyCameForYou, MSGBOX_DEFAULT
	giveitem ITEM_SS_TICKET
	msgbox PlayersHouse_1F_Text_PortsInSlateportLilycove, MSGBOX_DEFAULT
	closemessage
	delay 20
	compare VAR_0x8008, MALE
	call_if_eq PlayersHouse_1F_EventScript_MomApproachDadMale
	compare VAR_0x8008, FEMALE
	call_if_eq PlayersHouse_1F_EventScript_MomApproachDadFemale
	msgbox PlayersHouse_1F_Text_BetterGetBackToGym, MSGBOX_DEFAULT
	closemessage
	compare VAR_0x8008, MALE
	call_if_eq PlayersHouse_1F_EventScript_DadExitsMale
	compare VAR_0x8008, FEMALE
	call_if_eq PlayersHouse_1F_EventScript_DadExitsFemale
	playse SE_DOOR
	removeobject VAR_0x8009
	setflag FLAG_RECEIVED_SS_TICKET
	delay 30
	compare VAR_0x8008, MALE
	call_if_eq PlayersHouse_1F_EventScript_MomApproachPlayerMale
	compare VAR_0x8008, FEMALE
	call_if_eq PlayersHouse_1F_EventScript_MomApproachPlayerFemale
	delay 20
	msgbox PlayersHouse_1F_Text_DadShouldStayLonger, MSGBOX_DEFAULT
	closemessage
	setflag FLAG_SYS_TV_LATIAS_LATIOS
	special TurnOnTVScreen
	delay 60
	compare VAR_0x8008, MALE
	call_if_eq PlayersHouse_1F_EventScript_MomNoticesLatiBroadcastMale
	compare VAR_0x8008, FEMALE
	call_if_eq PlayersHouse_1F_EventScript_MomNoticesLatiBroadcastFemale
	msgbox PlayersHouse_1F_Text_IsThatABreakingStory, MSGBOX_DEFAULT
	closemessage
	compare VAR_0x8008, MALE
	call_if_eq PlayersHouse_1F_EventScript_PlayerApproachTVForLatiMale
	compare VAR_0x8008, FEMALE
	call_if_eq PlayersHouse_1F_EventScript_PlayerApproachTVForLatiFemale
	msgbox PlayersHouse_1F_Text_LatiEmergencyNewsFlash, MSGBOX_DEFAULT
	closemessage
	clearflag FLAG_SYS_TV_LATIAS_LATIOS
	setflag FLAG_LATIOS_OR_LATIAS_ROAMING
	special TurnOffTVScreen
	compare VAR_0x8008, MALE
	call_if_eq PlayersHouse_1F_EventScript_MomApproachPlayerAfterTVMale
	compare VAR_0x8008, FEMALE
	call_if_eq PlayersHouse_1F_EventScript_MomApproachPlayerAfterTVFemale
	msgbox PlayersHouse_1F_Text_WhatColorDidTheySay, MSGBOX_DEFAULT
	multichoice 22, 8, MULTI_TV_LATI, 1
	copyvar VAR_0x8004, VAR_RESULT
	special InitRoamer
	copyvar VAR_ROAMER_POKEMON, VAR_RESULT
	msgbox PlayersHouse_1F_Text_StillUnknownPokemon, MSGBOX_DEFAULT
	closemessage
	setvar VAR_LITTLEROOT_HOUSES_STATE_MAY, 4
	setvar VAR_LITTLEROOT_HOUSES_STATE_BRENDAN, 4
	releaseall
	end

@ Never called.
PlayersHouse_1F_EventScript_AirLatiBroadcast:: @ 8292C72
	setflag FLAG_SYS_TV_LATIAS_LATIOS
	return

PlayersHouse_1F_EventScript_SetUpObjectEventVarsMale:: @ 8292C76
	setvar VAR_0x8008, MALE
	setvar VAR_0x8009, LOCALID_DAD
	setvar VAR_0x800A, LOCALID_MOM
	return

PlayersHouse_1F_EventScript_SetUpObjectEventVarsFemale:: @ 8292C86
	setvar VAR_0x8008, FEMALE
	setvar VAR_0x8009, LOCALID_DAD
	setvar VAR_0x800A, LOCALID_MOM
	return

PlayersHouse_1F_EventScript_DadApproachPlayerMale:: @ 8292C96
	applymovement VAR_0x8009, PlayersHouse_1F_Movement_DadApproachPlayerMale
	waitmovement 0
	return

PlayersHouse_1F_EventScript_DadApproachPlayerFemale:: @ 8292CA1
	applymovement VAR_0x8009, PlayersHouse_1F_Movement_DadApproachPlayerFemale
	waitmovement 0
	return

PlayersHouse_1F_EventScript_MomApproachDadMale:: @ 8292CAC
	applymovement VAR_0x800A, PlayersHouse_1F_Movement_MomApproachDadMale
	waitmovement 0
	applymovement VAR_0x8009, PlayersHouse_1F_Movement_DadFaceMomMale
	waitmovement 0
	return

PlayersHouse_1F_EventScript_MomApproachDadFemale:: @ 8292CC1
	applymovement VAR_0x800A, PlayersHouse_1F_Movement_MomApproachDadFemale
	waitmovement 0
	applymovement VAR_0x8009, PlayersHouse_1F_Movement_DadFaceMomFemale
	waitmovement 0
	return

PlayersHouse_1F_EventScript_DadExitsMale:: @ 8292CD6
	applymovement OBJ_EVENT_ID_PLAYER, PlayersHouse_1F_Movement_MomAndPlayerWatchDadExit
	applymovement VAR_0x800A, PlayersHouse_1F_Movement_MomAndPlayerWatchDadExit
	applymovement VAR_0x8009, PlayersHouse_1F_Movement_DadExitsMale
	waitmovement 0
	return

PlayersHouse_1F_EventScript_DadExitsFemale:: @ 8292CEF
	applymovement OBJ_EVENT_ID_PLAYER, PlayersHouse_1F_Movement_MomAndPlayerWatchDadExit
	applymovement VAR_0x800A, PlayersHouse_1F_Movement_MomAndPlayerWatchDadExit
	applymovement VAR_0x8009, PlayersHouse_1F_Movement_DadExitsFemale
	waitmovement 0
	return

PlayersHouse_1F_EventScript_PlayerEnterRoomMale:: @ 8292D08
	applymovement OBJ_EVENT_ID_PLAYER, PlayersHouse_1F_Movement_PlayerEnterRoomMale
	waitmovement 0
	return

PlayersHouse_1F_EventScript_PlayerEnterRoomFemale:: @ 8292D13
	applymovement OBJ_EVENT_ID_PLAYER, PlayersHouse_1F_Movement_PlayerEnterRoomFemale
	waitmovement 0
	return

PlayersHouse_1F_EventScript_PlayerApproachTVForLatiMale:: @ 8292D1E
	applymovement OBJ_EVENT_ID_PLAYER, PlayersHouse_1F_Movement_PlayerApproachTVForLatiMale
	waitmovement 0
	applymovement VAR_0x800A, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	return

PlayersHouse_1F_EventScript_PlayerApproachTVForLatiFemale:: @ 8292D33
	applymovement OBJ_EVENT_ID_PLAYER, PlayersHouse_1F_Movement_PlayerApproachTVForLatiFemale
	waitmovement 0
	applymovement VAR_0x800A, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	return

PlayersHouse_1F_EventScript_MomApproachPlayerMale:: @ 8292D48
	applymovement VAR_0x800A, PlayersHouse_1F_Movement_MomApproachPlayerMale
	waitmovement 0
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	return

PlayersHouse_1F_EventScript_MomApproachPlayerFemale:: @ 8292D5D
	applymovement VAR_0x800A, PlayersHouse_1F_Movement_MomApproachPlayerFemale
	waitmovement 0
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	return

PlayersHouse_1F_EventScript_MomNoticesLatiBroadcastMale:: @ 8292D72
	applymovement VAR_0x800A, PlayersHouse_1F_Movement_MomNoticesLatiBroadcastMale
	waitmovement 0
	return

PlayersHouse_1F_EventScript_MomNoticesLatiBroadcastFemale:: @ 8292D7D
	applymovement VAR_0x800A, PlayersHouse_1F_Movement_MomNoticesLatiBroadcastFemale
	waitmovement 0
	return

PlayersHouse_1F_EventScript_MomApproachPlayerAfterTVMale:: @ 8292D88
	applymovement VAR_0x800A, PlayersHouse_1F_Movement_MomApproachPlayerAfterTVMale
	waitmovement 0
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	return

PlayersHouse_1F_EventScript_MomApproachPlayerAfterTVFemale:: @ 8292D9D
	applymovement VAR_0x800A, PlayersHouse_1F_Movement_MomApproachPlayerAfterTVFemale
	waitmovement 0
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	return

PlayersHouse_1F_Movement_DadApproachPlayerMale: @ 8292DB2
	walk_right
	walk_right
	step_end

PlayersHouse_1F_Movement_DadApproachPlayerFemale: @ 8292DB5
	walk_left
	walk_left
	step_end

PlayersHouse_1F_Movement_DadFaceMomMale: @ 8292DB8
	face_left
	step_end

PlayersHouse_1F_Movement_DadFaceMomFemale: @ 8292DBA
	face_right
	step_end

PlayersHouse_1F_Movement_DadExitsMale: @ 8292DBC
	walk_down
	walk_right
	walk_down
	delay_8
	step_end

PlayersHouse_1F_Movement_DadExitsFemale: @ 8292DC1
	walk_down
	walk_left
	walk_down
	delay_8
	step_end

PlayersHouse_1F_Movement_PlayerEnterRoomMale: @ 8292DC6
	delay_16
	walk_down
	walk_down
	walk_down
	walk_in_place_faster_left
	step_end

PlayersHouse_1F_Movement_MomAndPlayerWatchDadExit: @ 8292DCC
	delay_8
	delay_16
	delay_16
	walk_in_place_faster_down
	step_end

PlayersHouse_1F_Movement_PlayerEnterRoomFemale: @ 8292DD1
	delay_16
	walk_down
	walk_down
	walk_down
	walk_in_place_faster_right
	step_end

PlayersHouse_1F_Movement_PlayerApproachTVForLatiMale: @ 8292DD7
	walk_up
	walk_left
	walk_left
	walk_left
	walk_left
	walk_in_place_faster_up
	step_end

PlayersHouse_1F_Movement_PlayerApproachTVForLatiFemale: @ 8292DDE
	walk_up
	walk_right
	walk_right
	walk_right
	walk_right
	walk_in_place_faster_up
	step_end

EventScript_RunningShoesManual:: @ 8292DE5
	msgbox PlayersHouse_1F_Text_RunningShoesManual, MSGBOX_SIGN
	end

PlayersHouse_2F_Text_CheckTheTime:
	.string "Mom: Hey, {PLAYER}! Could you\n"
	.string "check the time for me?$"

PlayersHouse_2F_Text_ClockIsStopped: @ 81F8668
	.string "{PLAYER} glanced at the clock.\n"
	.string "What time is it?$"

PlayersHouse_2F_Text_GoDownstairs:
	.string "Mom: Oh no! I'm running late!\p"
	.string "Hurry downstairs so we can talk before\n"
	.string "I leave, okay {PLAYER}?$"

PlayersHouse_2F_EventScript_Notebook:: @ 81F8656
	msgbox PlayersHouse_2F_Text_Notebook, MSGBOX_SIGN
	end

PlayersHouse_2F_EventScript_GameCube:: @ 81F865F
	msgbox PlayersHouse_2F_Text_ItsAGameCube, MSGBOX_SIGN
	end

PlayersHouse_2F_Text_HowDoYouLikeYourRoom: @ 81F869A
	.string "Mom: Don't worry about it, {PLAYER}.\n"
	.string "I can check the time on my Rotom Phone.$"

PlayersHouse_2F_Text_PokeGear1:
	.string ". . . . . . . . . . . . . . . . . . .$"

PlayersHouse_2F_Text_PokeGear2:
	.string "Mom: Oh no! I'm running late!$"

PlayersHouse_2F_Text_RunningLate:
	.string "Mom: Sorry, {PLAYER}. I need to go.\n"
	.string "I'll miss my flight to Sinnoh!\p"
	.string "Maybe you can hang out with that new kid!\n"
	.string "You know, the neighbor, from Galar. They\l"
	.string "were around your age, {PLAYER}.\p"
	.string "What was their name again?$"

PlayersHouse_2F_Text_RivalName:
	.string "Mom: Oh, that's right! {RIVAL}!\p"
	.string "Okay honey, be good! Really, do try\n"
	.string "to make friends with {RIVAL}, okay?\p"
	.string "I'll be back in a few days.\p"
	.string "Sakura will be in charge while I'm gone.\n"
	.string "She's probably still at her lab.\p"
	.string "She'll come over when she's done working\n"
	.string "this evening, okay? Don't wander off!\p"
	.string "Springrock Path is dangerous alone.$"

PlayersHouse_2F_Text_IKnowYouWantAPokemon:
	.string "Mom: Oh, {PLAYER}, I know you really\n"
	.string "want a Pokémon of your own, but...\p"
	.string "Well, we can talk about it when I'm\n"
	.string "back from Sinnoh, okay? Prof. Rowan will\l"
	.string "get annoyed if I don't make it on time.\p"
	.string "...!\p"
	.string "I forgot! I have something to give you\n"
	.string "before I leave! Come downstairs with me!$"

PlayersHouse_2F_Text_Notebook: @ 81F877F
	.string "{PLAYER} flipped open the notebook.\p"
	.string "Dear {PLAYER}...\p"
	.string "Thanks for playing Pokémon Quantum!\n"
	.string "--Cappuccino$"

Common_Text_LookCloserAtMap: @ 81F8820
	.string "Hey, it's a map of Rika!$"

PlayersHouse_2F_Text_ItsAGameCube: @ 81F884F
	.string "It's a Nintendo Gamecube.\p"
	.string "It was a great console, but it's\n"
	.string "starting to collect a lot of dust.$"

PlayersHouse_1F_Text_IsntItNiceInHere:
	.string "MOM: See, {PLAYER}?\n"
	.string "Isn't it nice in here, too?$"

PlayersHouse_1F_Text_MoversPokemonGoSetClock:
	.string "The mover's POKéMON do all the work\n"
	.string "of moving us in and cleaning up after.\l"
	.string "This is so convenient!\p"
	.string "{PLAYER}, your room is upstairs.\n"
	.string "Go check it out, dear!\p"
	.string "DAD bought you a new clock to mark\n"
	.string "our move here.\l"
	.string "Don't forget to set it!$"

PlayersHouse_1F_Text_ArentYouInterestedInRoom:
	.string "MOM: Well, {PLAYER}?\p"
	.string "Aren't you interested in seeing your\n"
	.string "very own room?$"

PlayersHouse_1F_Text_GoSetTheClock:
	.string "MOM: {PLAYER}.\p"
	.string "Go set the clock in your room, honey.$"

PlayersHouse_1F_Text_OhComeQuickly:
	.string "MOM: Oh! {PLAYER}, {PLAYER}!\n"
	.string "Quick! Come quickly!$"

PlayersHouse_1F_Text_MaybeDadWillBeOn:
	.string "MOM: Look! It's PETALBURG GYM!\n"
	.string "Maybe DAD will be on!$"

PlayersHouse_1F_Text_ItsOverWeMissedHim:
	.string "MOM: Oh… It's over.\p"
	.string "I think DAD was on, but we missed him.\n"
	.string "Too bad.$"

PlayersHouse_1F_Text_GoIntroduceYourselfNextDoor:
	.string "Oh, yes.\n"
	.string "One of DAD's friends lives in town.\p"
	.string "PROF. BIRCH is his name.\p"
	.string "He lives right next door, so you should\n"
	.string "go over and introduce yourself.$"

PlayersHouse_1F_Text_SeeYouHoney:
	.string "MOM: See you, honey!$"

PlayersHouse_1F_Text_DidYouMeetProfBirch:
	.string "MOM: Did you introduce yourself to\n"
	.string "PROF. BIRCH?$"

PlayersHouse_1F_Text_YouShouldRestABit:
	.string "MOM: How are you doing, {PLAYER}?\n"
	.string "You look a little tired.\p"
	.string "I think you should rest a bit.$"

PlayersHouse_1F_Text_TakeCareHoney:
	.string "MOM: Take care, honey!$"

PlayersHouse_1F_Text_GotDadsBadgeHeresSomethingFromMom:
	.string "MOM: Oh? Did DAD give you that BADGE?\p"
	.string "Then here's something from your MOM!$"

PlayersHouse_1F_Text_DontPushYourselfTooHard:
	.string "Don't push yourself too hard, dear.\n"
	.string "You can always come home.\p"
	.string "Go for it, honey!$"

PlayersHouse_1F_Text_IsThatAPokenav:
	.string "MOM: What is that, honey? A POKéNAV?\n"
	.string "Someone from DEVON gave it to you?\p"
	.string "Well, honey, how about registering\n"
	.string "your mom?\p"
	.string "… … …$"

PlayersHouse_1F_Text_RegisteredMom:
	.string "Registered MOM\n"
	.string "in the POKéNAV.$"

PlayersHouse_1F_Text_Vigoroth1:
	.string "Fugiiiiih!$"

PlayersHouse_1F_Text_Vigoroth2:
	.string "Huggoh, uggo uggo…$"

PlayersHouse_1F_Text_ReportFromPetalburgGym:
	.string "INTERVIEWER: …We brought you this\n"
	.string "report from in front of PETALBURG GYM.$"

PlayersHouse_1F_Text_TheresAMovieOnTV:
	.string "There is a movie on TV.\p"
	.string "Two men are dancing on a big piano\n"
	.string "keyboard.\p"
	.string "Better get going!$"

PlayersHouse_1F_Text_RunningShoesManual:
	.string "It's the instruction booklet for the\n"
	.string "RUNNING SHOES.\p"
	.string "“Press the B Button to run while\n"
	.string "wearing your RUNNING SHOES.\p"
	.string "“Lace up your RUNNING SHOES and hit\n"
	.string "the road running!”$"

PlayersHouse_1F_Text_TicketFromBrineyCameForYou:
	.string "DAD: Hm?\p"
	.string "Hey, it's {PLAYER}!\p"
	.string "It's been a while since I saw you,\n"
	.string "but you look…stronger, somehow.\p"
	.string "That's the impression I get.\n"
	.string "But your old man hasn't given up yet!\p"
	.string "Oh, yes, I have something for you.\n"
	.string "This came to you from someone named\l"
	.string "MR. BRINEY.$"

PlayersHouse_1F_Text_PortsInSlateportLilycove:
	.string "DAD: Hm, a TICKET for a ferry?\p"
	.string "If I recall, there are ferry ports in\n"
	.string "SLATEPORT and LILYCOVE.$"

PlayersHouse_1F_Text_BetterGetBackToGym:
	.string "I'd better get back to PETALBURG GYM.\p"
	.string "MOM, thanks for looking after the house\n"
	.string "while I'm away.$"

PlayersHouse_1F_Text_DadShouldStayLonger:
	.string "MOM: That DAD of yours…\p"
	.string "He comes home for the first time in a\n"
	.string "while, but all he talks about is POKéMON.\p"
	.string "He should relax and stay a little longer.$"

PlayersHouse_1F_Text_IsThatABreakingStory:
	.string "MOM: Is that a breaking news story?$"

PlayersHouse_1F_Text_LatiEmergencyNewsFlash:
	.string "We bring you this emergency\n"
	.string "news flash!\p"
	.string "In various HOENN locales, there have\n"
	.string "been reports of a BZZT…colored\l"
	.string "POKéMON in flight.\p"
	.string "The identity of this POKéMON is\n"
	.string "currently unknown.\p"
	.string "We now return you to the regular\n"
	.string "movie program.$"

PlayersHouse_1F_Text_WhatColorDidTheySay:
	.string "MOM: {PLAYER}, did you catch that?\p"
	.string "What color did the announcer say\n"
	.string "that POKéMON was?$"

PlayersHouse_1F_Text_StillUnknownPokemon:
	.string "MOM: Well, isn't that something!\n"
	.string "There are still unknown POKéMON.$"
