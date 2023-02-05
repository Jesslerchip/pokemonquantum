Route101_MapScripts::
	.byte 0


Route1_EventScript_RouteSign::
	lockall
	msgbox Route1_EventScript_RouteSign_Text_0
	releaseall
	return


Route1_EventScript_CaveSign::
	lockall
	msgbox Route1_EventScript_CaveSign_Text_0
	releaseall
	return


Route1_EventScript_StatsGuy::
	lockall
	faceplayer
	msgbox Route1_EventScript_StatsGuy_Text_0, MSGBOX_YESNO
	compare VAR_RESULT, YES
	goto_if_eq Route1_EventScript_StatsGuy_2
	msgbox Route1_EventScript_StatsGuy_Text_2
Route1_EventScript_StatsGuy_1:
	releaseall
	return

Route1_EventScript_StatsGuy_2:
	msgbox Route1_EventScript_StatsGuy_Text_1
	goto Route1_EventScript_StatsGuy_1


Route1_EventScript_Woman::
	lockall
	faceplayer
	msgbox Route1_EventScript_Woman_Text_0
	releaseall
	return


Route1_EventScript_Youngster::
	lockall
	faceplayer
	msgbox Route1_EventScript_Youngster_Text_0
	releaseall
	return


Route1_EventScript_RouteSign_Text_0:
	.string "Route 1\p"
	.string "{UP_ARROW}Springwood Pond\n"
	.string "{RIGHT_ARROW}Glacial Bay$"

Route1_EventScript_CaveSign_Text_0:
	.string "Mt. Draco$"

Route1_EventScript_StatsGuy_Text_0:
	.string "Oh, hi! Did you want to hear about the\n"
	.string "different Pokémon stats?$"

Route1_EventScript_StatsGuy_Text_1:
	.string "Okay! First, every Pokémon species has\n"
	.string "a set of base stats. These are HP,\l"
	.string "Attack, Defense, Special Attack,\l"
	.string "Special Defense, and Speed. The\l"
	.string "specific values are unique to each\l"
	.string "species, but all Pokémon of the same\l"
	.string "species have the same base stats.\p"
	.string "Each Pokémon is also born with IVs, or\n"
	.string "Independent Values. These are fixed,\l"
	.string "but can be changed with certain items.\l"
	.string "Each IV can go up to 31, so try to get\l"
	.string "31 on all a Pokémon's stats to unlock\l"
	.string "their true potential.\p"
	.string "Finally, Pokémon have EVs, or Effort\n"
	.string "Values. These are gained by defeating\l"
	.string "certain Pokémon, or consuming special\l"
	.string "items. A Pokémon can only have a total\l"
	.string "of 510 EVs, and only 252 per stat.\p"
	.string "Working with your Pokémon to train\n"
	.string "these is the best way to make them\l"
	.string "stronger, so do your best to watch\l"
	.string "their stats carefully from the Summary\l"
	.string "Screen!\p"
	.string "A Pokémon's Nature also effects stats,\n"
	.string "by increasing one and decreasing\l"
	.string "another. Try to pick beneficial Natures\l"
	.string "to maximize your capabilities and\l"
	.string "defeat all of your opponents!\p"
	.string "Of course, a Pokémon's stats go up if\n"
	.string "you level them, too- but who doesn't\l"
	.string "like getting obsessed with insanely\l"
	.string "fine details?\p"
	.string "I know I've been talking a lot, but this\n"
	.string "stuff is important if you plan to beat\l"
	.string "Trainers. Of course, I suppose you\l"
	.string "could also just train Pokémon casually\l"
	.string "too, but all the cool kids know about\l"
	.string "stats!$"

Route1_EventScript_StatsGuy_Text_2:
	.string "Yeah, it's pretty complicated. You're\n"
	.string "probably not smart enough to\l"
	.string "understand it like real Trainers do.$"

Route1_EventScript_Woman_Text_0:
	.string "I'm looking for Pokémon in the tall\n"
	.string "grass.\p"
	.string "Some are more rare than others, you\n"
	.string "know. I really want a Skitty, but\l"
	.string "they're super hard to find!$"

Route1_EventScript_Youngster_Text_0:
	.string "Hi! I like shorts!\p"
	.string "They're actually quite durable, and\n"
	.string "they really help in warm weather. Long\l"
	.string "pants are great if you're trying to\l"
	.string "crawl through all that tall grass, but\l"
	.string "if you just want to enjoy a nice,\l"
	.string "summer day? I think shorts are\l"
	.string "honestly the best pick. Really, you can\l"
	.string "wear whatever you'd like- I enjoy\l"
	.string "shorts, but anyone is free to choose\l"
	.string "any outfit they want! All I'm saying is,\l"
	.string "be yourself- but shorts will help you\l"
	.string "be your best self.$"
