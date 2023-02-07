.set LOCALID_NURSE, 1

PacifidlogTown_PokemonCenter_1F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, PacifidlogTown_PokemonCenter_1F_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, CableClub_OnResume
	.byte 0

PacifidlogTown_PokemonCenter_1F_OnTransition:
	setrespawn HEAL_LOCATION_PACIFIDLOG_TOWN
	end

PacifidlogTown_PokemonCenter_1F_EventScript_Nurse::
	setvar VAR_0x800B, LOCALID_NURSE
	call Common_EventScript_PkmnCenterNurse
	waitmessage
	waitbuttonpress
	release
	end

PacifidlogTown_PokemonCenter_1F_EventScript_Girl::
	msgbox PacifidlogTown_PokemonCenter_1F_Text_WhatColorTrainerCard, MSGBOX_NPC
	end

PacifidlogTown_PokemonCenter_1F_EventScript_Woman::
	msgbox PacifidlogTown_PokemonCenter_1F_Text_OnColonyOfCorsola, MSGBOX_NPC
	end

PacifidlogTown_PokemonCenter_1F_EventScript_OldMan::
	msgbox PacifidlogTown_PokemonCenter_1F_Text_AncestorsLivedOnBoats, MSGBOX_NPC
	end

PacifidlogTown_PokemonCenter_1F_Text_WhatColorTrainerCard:
	.string "What color is your TRAINER CARD?\n"
	.string "Mine's copper!$"

PacifidlogTown_PokemonCenter_1F_Text_OnColonyOfCorsola:
	.string "PACIFIDLOG TOWN floats on top of\n"
	.string "a colony of CORSOLA.\p"
	.string "If I told you that, would you believe\n"
	.string "me?$"

PacifidlogTown_PokemonCenter_1F_Text_AncestorsLivedOnBoats:
	.string "The ancestors of the people in\n"
	.string "PACIFIDLOG were said to have been\l"
	.string "born on boats and then lived and died \l"
	.string "aboard them.\p"
	.string "I understand that they lived that way\n"
	.string "because they were searching for\l"
	.string "something.$"

TaprootTown_PokemonCenter_EventScript_Quantum::
	lockall
	faceplayer
	msgbox TaprootTown_PokemonCenter_EventScript_Quantum_Text_0
	releaseall
	return


TaprootTown_PokemonCenter_EventScript_NiceRest::
	lockall
	faceplayer
	msgbox TaprootTown_PokemonCenter_EventScript_NiceRest_Text_0
	releaseall
	return


TaprootTown_PokemonCenter_EventScript_RegionMap::
	lockall
	faceplayer
	msgbox TaprootTown_PokemonCenter_EventScript_RegionMap_Text_0
	releaseall
	return


TaprootTown_PokemonCenter_EventScript_GuyInPurple::
	lockall
	faceplayer
	msgbox TaprootTown_PokemonCenter_EventScript_GuyInPurple_Text_0
	releaseall
	return


TaprootTown_PokemonCenter_EventScript_Quantum_Text_0:
	.string "Hey, have you heard of Team Quantum?\p"
	.string "Everyone says they're going around\n"
	.string "wreaking havoc and talking about crazy\l"
	.string "science things. What's up with that?$"

TaprootTown_PokemonCenter_EventScript_NiceRest_Text_0:
	.string "I love coming to Taproot Town for a nice\n"
	.string "rest. It helps my Pokémon heal up on\l"
	.string "the way to Spectre City.$"

TaprootTown_PokemonCenter_EventScript_RegionMap_Text_0:
	.string "Have you taken a look at the region\n"
	.string "map? It'll show you exactly where you\l"
	.string "are in Rika.$"

TaprootTown_PokemonCenter_EventScript_GuyInPurple_Text_0:
	.string "Some guy dressed up in purple used some\n"
	.string "super strong Pokémon on me. I have no\l"
	.string "idea what his deal is, but he was way\l"
	.string "more aggressive than a Trainer ought\l"
	.string "to be. Pokémon are friends, not tools\l"
	.string "of war!$"
