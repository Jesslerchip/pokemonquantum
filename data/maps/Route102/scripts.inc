Route102_EventScript_LittleBoy::
	msgbox Route102_Text_ImNotVeryTall, MSGBOX_NPC
	end

Route102_EventScript_RouteSignOldale::
	msgbox Route102_Text_RouteSignOldale, MSGBOX_SIGN
	end

Route102_EventScript_RouteSignPetalburg::
	msgbox Route102_Text_RouteSignPetalburg, MSGBOX_SIGN
	end

Route102_EventScript_Boy::
	msgbox Route102_Text_CatchWholeBunchOfPokemon, MSGBOX_NPC
	end

Route102_EventScript_Calvin::
	trainerbattle_single TRAINER_CALVIN_1, Route102_Text_CalvinIntro, Route102_Text_CalvinDefeated, Route102_EventScript_CalvinRegisterMatchCallAfterBattle
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, Route102_EventScript_CalvinRematch
	setvar VAR_0x8004, TRAINER_CALVIN_1
	specialvar VAR_RESULT, IsTrainerRegistered
	goto_if_eq VAR_RESULT, FALSE, Route102_EventScript_CalvinTryRegister
	msgbox Route102_Text_CalvinPostBattle, MSGBOX_DEFAULT
	release
	end

Route102_EventScript_CalvinRegisterMatchCallAfterBattle::
	special PlayerFaceTrainerAfterBattle
	waitmovement 0
	goto_if_set FLAG_HAS_MATCH_CALL, Route102_EventScript_CalvinRegisterMatchCall
	release
	end

Route102_EventScript_CalvinRegisterMatchCall::
	msgbox Route102_Text_CalvinRegisterShort, MSGBOX_DEFAULT
	register_matchcall TRAINER_CALVIN_1
	release
	end

Route102_EventScript_CalvinTryRegister::
	goto_if_set FLAG_HAS_MATCH_CALL, Route102_EventScript_CalvinRegister
	msgbox Route102_Text_CalvinPostBattle, MSGBOX_DEFAULT
	release
	end

Route102_EventScript_CalvinRegister::
	msgbox Route102_Text_CalvinRegister, MSGBOX_DEFAULT
	register_matchcall TRAINER_CALVIN_1
	release
	end

Route102_EventScript_CalvinRematch::
	trainerbattle_rematch TRAINER_CALVIN_1, Route102_Text_CalvinRematchIntro, Route102_Text_CalvinRematchDefeated
	msgbox Route102_Text_CalvinRematchPostBattle, MSGBOX_AUTOCLOSE
	end

Route102_EventScript_Rick::
	trainerbattle_single TRAINER_RICK, Route102_Text_RickIntro, Route102_Text_RickDefeated
	msgbox Route102_Text_RickPostBattle, MSGBOX_AUTOCLOSE
	end

Route102_EventScript_Tiana::
	trainerbattle_single TRAINER_TIANA, Route102_Text_TianaIntro, Route102_Text_TianaDefeated
	msgbox Route102_Text_TianaPostBattle, MSGBOX_AUTOCLOSE
	end

Route102_EventScript_Allen::
	trainerbattle_single TRAINER_ALLEN, Route102_Text_AllenIntro, Route102_Text_AllenDefeated
	msgbox Route102_Text_AllenPostBattle, MSGBOX_AUTOCLOSE
	end

Route102_Text_WatchMeCatchPokemon:
	.string "WALLY: {PLAYER}…\n"
	.string "POKéMON hide in tall grass like this,\l"
	.string "don't they?\p"
	.string "Please watch me and see if I can\n"
	.string "catch one properly.\p"
	.string "…Whoa!$"

Route102_Text_WallyIDidIt:
	.string "WALLY: I did it… It's my…\n"
	.string "My POKéMON!$"

Route102_Text_LetsGoBack:
	.string "{PLAYER}, thank you!\n"
	.string "Let's go back to the GYM!$"

Route102_Text_ImNotVeryTall:
	.string "I'm…not very tall, so I sink right\n"
	.string "into tall grass.\p"
	.string "The grass goes up my nose and…\n"
	.string "Fwafwafwafwafwa…\p"
	.string "Fwatchoo!$"

Route102_Text_CatchWholeBunchOfPokemon:
	.string "I'm going to catch a whole bunch of\n"
	.string "POKéMON!$"

Route102_Text_RouteSignOldale:
	.string "ROUTE 102\n"
	.string "{RIGHT_ARROW} OLDALE TOWN$"

Route102_Text_RouteSignPetalburg:
	.string "ROUTE 102\n"
	.string "{LEFT_ARROW} PETALBURG CITY$"

Route102_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Route102_MapScripts_MAP_SCRIPT_ON_TRANSITION
	.byte 0

Route102_MapScripts_MAP_SCRIPT_ON_TRANSITION:
	goto_if_unset FLAG_HIDE_ROUTE_2_RIVAL, Route102_MapScripts_MAP_SCRIPT_ON_TRANSITION_1
	return

Route102_MapScripts_MAP_SCRIPT_ON_TRANSITION_1:
	call Common_EventScript_SetupRivalGfxId
	return


Route2_EventScript_Nurse::
	lock
	faceplayer
	msgbox Route2_EventScript_Nurse_Text_0
	closemessage
	fadescreen FADE_TO_BLACK
	playfanfare MUS_HEAL
	waitfanfare
	special HealPlayerParty
	fadescreen FADE_FROM_BLACK
	msgbox Route2_EventScript_Nurse_Text_1
	applymovement 4, Route2_Movement_Bow
	waitmovement 0
	closemessage
	release
	return


Route2_Movement_Bow:
	nurse_joy_bow
	delay_4
	step_end

Route2_EventScript_CaveSign::
	lockall
	msgbox Route2_EventScript_CaveSign_Text_0
	releaseall
	return


Route2_EventScript_Timmy::
	trainerbattle_single TRAINER_TIMMY, Route2_Text_TimmyIntro, Route2_Text_TimmyDefeat
	msgbox Route2_Text_TimmyPostBattle, MSGBOX_AUTOCLOSE
	return


Route2_EventScript_Poppy::
	trainerbattle_single TRAINER_POPPY, Route2_Text_PoppyIntro, Route2_Text_PoppyDefeat
	msgbox Route2_Text_PoppyPostBattle, MSGBOX_AUTOCLOSE
	return


Route2_EventScript_Richtor::
	trainerbattle_single TRAINER_RICHTOR, Route2_Text_RichtorIntro, Route2_Text_RichtorDefeat
	msgbox Route2_Text_RichtorPostBattle, MSGBOX_AUTOCLOSE
	return


Route2_EventScript_Artist::
	lock
	faceplayer
	msgbox Route2_EventScript_Artist_Text_0
	release
	return


Route2_EventScript_ItemPokeDoll::
	finditem ITEM_POKE_DOLL
	return


Route2_EventScript_RouteSign::
	lockall
	msgbox Route2_EventScript_RouteSign_Text_0
	releaseall
	return


Route2_EventScript_RivalTrigger1::
	lockall
	playse SE_PIN
	playbgm MUS_ENCOUNTER_BRENDAN, 0
	applymovement 7, Route2_Movement_BellaApproachTrigger1
	waitmovement 0
	goto Route2_EventScript_RivalBattle
	return


Route2_EventScript_RivalTrigger2::
	lockall
	playse SE_PIN
	playbgm MUS_ENCOUNTER_BRENDAN, 0
	applymovement 7, Route2_Movement_BellaApproachTrigger2
	waitmovement 0
	goto Route2_EventScript_RivalBattle
	return


Route2_EventScript_RivalTrigger3::
	lockall
	playse SE_PIN
	playbgm MUS_ENCOUNTER_BRENDAN, 0
	applymovement 7, Route2_Movement_BellaApproachTrigger3
	waitmovement 0
	goto Route2_EventScript_RivalBattle
	return


Route2_EventScript_RivalTrigger4::
	lockall
	playse SE_PIN
	applymovement 7, Common_Movement_ExclamationMark
	waitmovement 0
	playbgm MUS_ENCOUNTER_BRENDAN, 0
	applymovement 7, Route2_Movement_BellaApproachTrigger4
	waitmovement 0
	goto Route2_EventScript_RivalBattle
	return


Route2_EventScript_RivalBattle::
	msgbox Route2_EventScript_RivalBattle_Text_0
	trainerbattle_no_intro TRAINER_BELLA_1, Route2_Text_BellaDefeated
	goto Route2_EventScript_AfterRivalBattle
	return


Route2_EventScript_AfterRivalBattle::
	msgbox Route2_EventScript_AfterRivalBattle_Text_0
	closemessage
	applymovement 7, Route2_Movement_BellaLeaves
	waitmovement 0
	setflag FLAG_HIDE_ROUTE_2_RIVAL
	removeobject 7
	setvar VAR_ROUTE2_STATE, 1
	clearflag FLAG_HIDE_TAPROOT_TOWN_VILDER
	setvar VAR_TAPROOT_TOWN_STATE, 0
	fadedefaultbgm
	completesubquest QUEST_A_SPOOKY_START, SUB_QUEST_2
	questmenu QUEST_MENU_BUFFER_QUEST_NAME, QUEST_A_SPOOKY_START
	call QuestUpdated
	closemessage
	releaseall
	return


Route2_Movement_BellaLeaves:
	walk_left
	walk_left
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

Route2_Movement_BellaApproachTrigger1:
	walk_right
	walk_down
	walk_in_place_right
	step_end

Route2_Movement_BellaApproachTrigger2:
	walk_right
	walk_in_place_right
	step_end

Route2_Movement_BellaApproachTrigger3:
	walk_right
	walk_up
	walk_up
	walk_in_place_up
	step_end

Route2_Movement_BellaApproachTrigger4:
	walk_up
	walk_up
	walk_in_place_up
	step_end

Route2_EventScript_Nurse_Text_0:
	.string "Your Pokémon must be tired.\p"
	.string "Here, I can get them healed right up!$"

Route2_EventScript_Nurse_Text_1:
	.string "There! Your Pokémon are ready to go!$"

Route2_EventScript_CaveSign_Text_0:
	.string "Springrock Path$"

Route2_EventScript_Artist_Text_0:
	.string "I like to come out here sometimes to\n"
	.string "get ideas for paintings. Sometimes, if\l"
	.string "you stand really still, you can even\l"
	.string "hear the Pokémon.$"

Route2_EventScript_RouteSign_Text_0:
	.string "Route 2\p"
	.string "{RIGHT_ARROW}Springrock Path\n"
	.string "{DOWN_ARROW}Taproot Town$"

Route2_EventScript_RivalBattle_Text_0:
	.string "Bella: Hey, {PLAYER}! I told you I'd\n"
	.string "beat you here!\p"
	.string "You seriously took Springrock Path? I\n"
	.string "was just yanking your chain!\p"
	.string "Me? I had my mother's Braviary carry me\n"
	.string "here. I've been waiting on you for a\l"
	.string "while!\p"
	.string "I've already caught loads of Pokémon\n"
	.string "for Prof. Sakura, too. I bet they're all\l"
	.string "better than yours!\p"
	.string "Here, let me prove it. I'll crush you in a\n"
	.string "Pokémon battle!$"

Route2_EventScript_AfterRivalBattle_Text_0:
	.string "Bella: You must have gotten lucky,\n"
	.string "{PLAYER}. My Pokémon are the best,\l"
	.string "and I know it!\p"
	.string "Let's raise the stakes. Why don't we see\n"
	.string "who can get the Spectre City Gym Badge\l"
	.string "first?\p"
	.string "No way you beat Polter with your team.\n"
	.string "You need to train up, {PLAYER}!\p"
	.string "Oh, right. Everyone here is spooked\n"
	.string "about some guy lurking around with a\l"
	.string "really strong Pokémon. They say he\l"
	.string "artificially closed the door to its\l"
	.string "heart, or something.\p"
	.string "Watch out, {PLAYER}. You ought to\n"
	.string "heal at the Pokémon Center before\l"
	.string "wandering around Taproot Town too\l"
	.string "much.\p"
	.string "Otherwise, those whimpy Pokémon of\n"
	.string "yours are toast!\p"
	.string "See you after I've got that first\n"
	.string "Badge, {PLAYER}!$"

Route2_Text_TimmyIntro::
	.string "Hey, you look like a new Trainer!\p"
	.string "I can totally beat you!$"

Route2_Text_TimmyDefeat::
	.string "I never stood a chance…$"

Route2_Text_TimmyPostBattle::
	.string "You were way tougher than I thought\n"
	.string "you'd be. That's not fair!$"

Route2_Text_PoppyIntro::
	.string "Rika is the prettiest region in the\n"
	.string "world! Wouldn't you agree?$"

Route2_Text_PoppyDefeat::
	.string "Oh, no! My Pokémon!$"

Route2_Text_PoppyPostBattle::
	.string "Battling isn't everything. In a Contest,\n"
	.string "looks, attitude, and showmanship all\l"
	.string "play a role.\p"
	.string "It's a much more elegant form of\n"
	.string "Pokémon competition.$"

Route2_Text_RichtorIntro::
	.string "“Light pink boots trudged through\n"
	.string "thick layers of snow as a child\l"
	.string "wandered, seemingly alone, through the\l"
	.string "woods.”\p"
	.string "I'm writing a story about Rika! Let's\n"
	.string "battle for inspiration!$"

Route2_Text_RichtorDefeat::
	.string "How inspiring!$"

Route2_Text_RichtorPostBattle::
	.string "“There was a blizzard whipping her\n"
	.string "around like a leaf, snow pelting her\l"
	.string "face and ungloved hands.”\p"
	.string "What do you think of that line?$"

Route2_Text_BellaDefeated::
	.string "Huh? You beat me? That's impossible!$"
