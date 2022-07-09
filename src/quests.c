#include "global.h"
#include "strings.h"
#include "bg.h"
#include "data.h"
#include "decompress.h"
#include "gpu_regs.h"
#include "graphics.h"
#include "item.h"
#include "item_icon.h"
#include "item_menu.h"
#include "item_menu_icons.h"
#include "list_menu.h"
#include "item_use.h"
#include "main.h"
#include "malloc.h"
#include "menu.h"
#include "menu_helpers.h"
#include "palette.h"
#include "party_menu.h"
#include "scanline_effect.h"
#include "sound.h"
#include "string_util.h"
#include "strings.h"
#include "task.h"
#include "text_window.h"
#include "quests.h"
#include "overworld.h"
#include "event_data.h"
#include "constants/items.h"
#include "constants/field_weather.h"
#include "constants/songs.h"
#include "constants/rgb.h"
#include "mgba_printf/mgba.h"
#include "constants/event_objects.h"
#include "event_object_movement.h"

#include "random.h"

#define tPageItems      data[4]
#define tItemPcParam    data[6]
//PSF TODO in original Unbound an unlocked quest just means it appears in the list, all quests with a NAME are considered Active... deal with this

//PSF TODO The sprite in the bottom left does not fade in and out despite the object layer being told to blend. The object arrows in the center of the screen fade without issue.
//PSF TODO There is a strange artifact when going from object to item.
//PSFP TODO Reorganize all functions near the top
//PSF TODO add static to all non extternal functions

struct QuestMenuResources
{
	MainCallback savedCallback;
	u8 moveModeOrigPos;
	u8 spriteIconSlot;
	u8 maxShowed;
	u8 nItems;
	u8 scrollIndicatorArrowPairId;
	s16 data[3];
	u8 filterMode;
	u8 parentQuest;
	bool8 restoreCursor;
};

struct QuestMenuStaticResources
{
	MainCallback savedCallback;
	u16 scroll;
	u16 row;
	u8 initialized;
	u16 storedScrollOffset;
	u16 storedRowPosition;
};

// RAM
EWRAM_DATA static struct QuestMenuResources *sStateDataPtr = NULL;
EWRAM_DATA static u8 *sBg1TilemapBuffer = NULL;
EWRAM_DATA static struct ListMenuItem *sListMenuItems = NULL;
EWRAM_DATA static struct QuestMenuStaticResources sListMenuState = {0};
EWRAM_DATA static u8 sItemMenuIconSpriteIds[12] = {0};        // from pokefirered src/item_menu_icons.c
EWRAM_DATA static void *questNamePointer = NULL;
EWRAM_DATA static u8 **questNameArray = NULL;

// This File's Functions

void QuestMenu_CreateSprite(u16 itemId, u8 idx, u8 spriteType);
void QuestMenu_ResetSpriteState(void);
void QuestMenu_DestroySprite(u8 idx);
s8 QuestMenu_ManageFavorites(u8 index);

static void QuestMenu_RunSetup(void);
static bool8 QuestMenu_SetupGraphics(void);
static void QuestMenu_FadeAndBail(void);
static void Task_QuestMenuWaitFadeAndBail(u8 taskId);
static bool8 QuestMenu_InitBackgrounds(void);
static bool8 QuestMenu_LoadGraphics(void);
static bool8 QuestMenu_AllocateResourcesForListMenu(void);
static u8 QuestMenu_CountNumberListRows();
static s8 QuestMenu_DoesQuestHaveChildrenAndNotInactive(u16 itemId);
static u16 QuestMenu_BuildMenuTemplate(void);
static void QuestMenu_AssignCancelNameAndId(u8 numRow);
static void QuestMenu_MoveCursorFunc(s32 itemIndex, bool8 onInit,
                                     struct ListMenu *list);
static void QuestMenu_GenerateStateAndPrint(u8 windowId, u32 itemId,
            u8 y);
static void QuestMenu_PrintOrRemoveCursorAt(u8 y, u8 state);
s8 QuestMenu_CountUnlockedQuests(void);
s8 QuestMenu_CountInactiveQuests(void);
s8 QuestMenu_CountActiveQuests(void);
s8 QuestMenu_CountRewardQuests(void);
s8 QuestMenu_CountCompletedQuests(void);
s8 QuestMenu_CountFavoriteQuests(void);
s8 QuestMenu_CountFavoriteAndState(void);
static void QuestMenu_GenerateAndPrintHeader(void);
static void QuestMenu_PlaceTopMenuScrollIndicatorArrows(void);
static void QuestMenu_SetCursorPosition(void);
static void QuestMenu_FreeResources(void);
static void Task_QuestMenuTurnOff2(u8 taskId);
static void QuestMenu_InitItems(void);
static void QuestMenu_SetScrollPosition(void);
static s8 QuestMenu_ManageMode(u8 action);
static void Task_QuestMenu_Main(u8 taskId);
static void Task_QuestMenuCleanUp(u8 taskId);
static void QuestMenu_InitWindows(void);
static void QuestMenu_AddTextPrinterParameterized(u8 windowId, u8 fontId,
            const u8 *str, u8 x, u8 y,
            u8 letterSpacing, u8 lineSpacing, u8 speed, u8 colorIdx);
static void QuestMenu_SetInitializedFlag(u8 a0);
static void QuestMenu_ClearModeOnStartup(void);

static void QuestMenu_SetGpuRegBaseForFade(
      void); //Sets the GPU registers to prepare for a hardware fade
static void QuestMenu_PrepareFadeOut(u8
                                     taskId); //Prepares the input handler for a hardware fade out
static void QuestMenu_PrepareFadeIn(u8
                                    taskId); //Prepares the input handler for a hardware fade in
static bool8 QuestMenu_HandleFadeOut(u8
                                     taskId); //Handles the hardware fade out
static bool8 QuestMenu_HandleFadeIn(u8
                                    taskId); //Handles the hardware fade in
static void Task_QuestMenu_FadeOut(u8 taskId);
static void Task_QuestMenu_FadeIn(u8 taskId);

void QuestMenu_GenerateAndPrintQuestDetails(s32 questId);
void QuestMenu_DetermineSpriteType(s32 questId);
void QuestMenu_PrintQuestLocation(s32 questId);

void QuestMenu_GenerateQuestFlavorText(s32 questId);
void QuestMenu_PrintQuestFlavorText(s32 questId);
void QuestMenu_GenerateQuestLocation(s32 questId);

bool8 QuestMenu_IsSubquestCompletedState(s32 questId);
bool8 QuestMenu_IsQuestRewardState(s32 questId);
bool8 QuestMenu_IsQuestCompletedState(s32 questId);
bool8 QuestMenu_IsQuestUnlocked(s32 questId);
bool8 QuestMenu_IsQuestOnlyActive(s32 questId);
bool8 QuestMenu_IsQuestActive(s32 questId);
bool8 QuestMenu_IsQuestInactive(s32 questId);
void QuestMenu_UpdateQuestFlavorText(s32 questId);

u8 QuestMenu_ToggleSubquestMode(u8 mode);
u8 QuestMenu_ToggleAlphaMode(u8 mode);
u8 QuestMenu_IncrementMode(u8 mode);

u8 QuestMenu_GenerateSubquestList();
u8 QuestMenu_GenerateFilteredList();
u8 QuestMenu_GenerateDefaultList();
void QuestMenu_PrependQuestNumber(u8 countQuest);
u8 QuestMenu_PopulateListRowNameAndId(u8 row, u8 countQuest);
void QuestMenu_PopulateSubquestTitle(u8 parentQuest, u8 countQuest);
void QuestMenu_PopulateEmptyRow(u8 countQuest);
void QuestMenu_PopulateQuestName(u8 countQuest);
void QuestMenu_AddSubQuestButton(u8 countQuest);
void QuestMenu_SetFavoriteQuest(u8 countQuest);
void QuestMenu_AllocateMemoryForArray();
u8 QuestMenu_GetModeAndGenerateList();
u8 *QuestMenu_DefineQuestOrder();
bool8 QuestMenu_CheckSelectedIsCancel(u8 selectedQuestId);
void QuestMenu_ChangeModeAndCleanUp(u8 taskId);
void QuestMenu_ToggleAlphaModeAndCleanUp(u8 taskId);
void QuestMenu_ToggleFavoriteAndCleanUp(u8 taskId, u8 selectedQuestId);
void QuestMenu_ReturnFromSubquestAndCleanUp(u8 taskId);
void QuestMenu_TurnOffQuestMenu(u8 taskId);
void QuestMenu_EnterSubquestModeAndCleanUp(u8 taskId, s16 *data,
            s32 input);

bool8 QuestMenu_IsSubquestCompleted(u8 parentQuest, u8 countQuest);
u8 QuestMenu_GenerateSubquestState(u8 questId);
u8 QuestMenu_GenerateQuestState(u8 questId);
void QuestMenu_PrintQuestState(u8 windowId, u8 y, u8 colorIndex);

bool8 QuestMenu_IfRowIsOutOfBounds(void);
bool8 QuestMenu_IfScrollIsOutOfBounds(void);
void QuestMenu_InitFadeVariables(u8 taskId, u8 blendWeight, u8 frameDelay,
                                 u8 frameTimerBase, u8 delta);

// Tiles, palettes and tilemaps for the Quest Menu
static const u32 sQuestMenuTiles[] =
      INCBIN_U32("graphics/quest_menu/menu.4bpp.lz");
static const u32 sQuestMenuBgPals[] =
      INCBIN_U32("graphics/quest_menu/menu_pal.gbapal.lz");
static const u32 sQuestMenuTilemap[] =
      INCBIN_U32("graphics/quest_menu/tilemap.bin.lz");

//Strings used for the Quest Menu
static const u8 sText_Empty[] = _("");
static const u8 sText_QuestMenu_AllHeader[] = _("All Missions");
static const u8 sText_QuestMenu_InactiveHeader[] = _("Inactive Missions");
static const u8 sText_QuestMenu_ActiveHeader[] = _("Active Missions");
static const u8 sText_QuestMenu_RewardHeader[] = _("Reward Available");
static const u8 sText_QuestMenu_CompletedHeader[] =
      _("Completed Missions");
static const u8 sText_QuestMenu_QuestNumberDisplay[] =
      _("{STR_VAR_1}/{STR_VAR_2}");
static const u8 sText_QuestMenu_Unk[] = _("??????");
static const u8 sText_QuestMenu_Active[] = _("Active");
static const u8 sText_QuestMenu_Reward[] = _("Reward");
static const u8 sText_QuestMenu_Complete[] = _("Done");
static const u8 sText_QuestMenu_ShowLocation[] =
      _("Location: {STR_VAR_2}");
static const u8 sText_QuestMenu_StartForMore[] =
      _("Start for more details.");
static const u8 sText_QuestMenu_ReturnRecieveReward[] =
      _("Return to {STR_VAR_2}\nto recieve your reward!");
static const u8 sText_QuestMenu_SubQuestButton[] = _("{A_BUTTON}");
static const u8 sText_QuestMenu_Type[] = _("{R_BUTTON}Type");
static const u8 sText_QuestMenu_Caught[] = _("Caught");
static const u8 sText_QuestMenu_Found[] = _("Found");
static const u8 sText_QuestMenu_Read[] = _("Read");
static const u8 sText_QuestMenu_Back[] = _("Back");
static const u8 sText_QuestMenu_DotSpace[] = _(". ");
static const u8 sText_QuestMenu_Close[] = _("Close");
static const u8 sText_QuestMenu_ColorGreen[] = _("{COLOR}{GREEN}");
static const u8 sText_QuestMenu_AZ[] = _(" A-Z");

//Declaration of subquest structures. Edits to subquests are made here.
#define sub_quest(i, n, d, m, s, st, t) {.id = i, .name = n, .desc = d, .map = m, .sprite = s, .spritetype = st, .type = t}
static const struct SubQuest sSubQuests1[SUB_QUEST_1_COUNT] =
{
	sub_quest(
	      0,
	      gText_SubQuest1_Name1,
	      gText_SubQuest1_Desc1,
	      gText_SideQuestMap1,
	      OBJ_EVENT_GFX_WALLY,
	      OBJECT,
	      sText_QuestMenu_Found
	),

	sub_quest(
	      1,
	      gText_SubQuest1_Name2,
	      gText_SubQuest1_Desc2,
	      gText_SideQuestMap2,
	      OBJ_EVENT_GFX_WALLY,
	      OBJECT,
	      sText_QuestMenu_Found
	),

	sub_quest(
	      2,
	      gText_SubQuest1_Name3,
	      gText_SubQuest1_Desc3,
	      gText_SideQuestMap3,
	      OBJ_EVENT_GFX_WALLY,
	      OBJECT,
	      sText_QuestMenu_Found
	),

	sub_quest(
	      3,
	      gText_SubQuest1_Name4,
	      gText_SubQuest1_Desc4,
	      gText_SideQuestMap4,
	      OBJ_EVENT_GFX_WALLY,
	      OBJECT,
	      sText_QuestMenu_Found
	),

	sub_quest(
	      4,
	      gText_SubQuest1_Name5,
	      gText_SubQuest1_Desc5,
	      gText_SideQuestMap5,
	      OBJ_EVENT_GFX_WALLY,
	      OBJECT,
	      sText_QuestMenu_Found
	),

	sub_quest(
	      5,
	      gText_SubQuest1_Name6,
	      gText_SubQuest1_Desc6,
	      gText_SideQuestMap6,
	      OBJ_EVENT_GFX_WALLY,
	      OBJECT,
	      sText_QuestMenu_Found
	),

	sub_quest(
	      6,
	      gText_SubQuest1_Name7,
	      gText_SubQuest1_Desc7,
	      gText_SideQuestMap7,
	      OBJ_EVENT_GFX_WALLY,
	      OBJECT,
	      sText_QuestMenu_Found
	),

	sub_quest(
	      7,
	      gText_SubQuest1_Name8,
	      gText_SubQuest1_Desc8,
	      gText_SideQuestMap8,
	      OBJ_EVENT_GFX_WALLY,
	      OBJECT,
	      sText_QuestMenu_Found
	),

	sub_quest(
	      8,
	      gText_SubQuest1_Name9,
	      gText_SubQuest1_Desc9,
	      gText_SideQuestMap9,
	      OBJ_EVENT_GFX_WALLY,
	      OBJECT,
	      sText_QuestMenu_Found
	),

	sub_quest(
	      9,
	      gText_SubQuest1_Name10,
	      gText_SubQuest1_Desc10,
	      gText_SideQuestMap10,
	      OBJ_EVENT_GFX_WALLY,
	      OBJECT,
	      sText_QuestMenu_Found
	),
};

static const struct SubQuest sSubQuests2[SUB_QUEST_2_COUNT] =
{
	sub_quest(
		10,
		gText_SubQuest2_Name1,
		gText_SubQuest2_Desc1,
		gText_SideQuestMap1,
		OBJ_EVENT_GFX_WALLY,
		OBJECT,
		sText_QuestMenu_Found
	),

	sub_quest(
		11,
		gText_SubQuest2_Name2,
		gText_SubQuest2_Desc2,
		gText_SideQuestMap2,
		OBJ_EVENT_GFX_WALLY,
		OBJECT,
		sText_QuestMenu_Found
	),

	sub_quest(
		12,
		gText_SubQuest2_Name3,
		gText_SubQuest2_Desc3,
		gText_SideQuestMap3,
		OBJ_EVENT_GFX_WALLY,
		OBJECT,
		sText_QuestMenu_Found
	),

	sub_quest(
		13,
		gText_SubQuest2_Name4,
		gText_SubQuest2_Desc4,
		gText_SideQuestMap4,
		OBJ_EVENT_GFX_WALLY,
		OBJECT,
		sText_QuestMenu_Found
	),

	sub_quest(
		14,
		gText_SubQuest2_Name5,
		gText_SubQuest2_Desc5,
		gText_SideQuestMap5,
		OBJ_EVENT_GFX_WALLY,
		OBJECT,
		sText_QuestMenu_Found
	),

	sub_quest(
		15,
		gText_SubQuest2_Name6,
		gText_SubQuest2_Desc6,
		gText_SideQuestMap6,
		OBJ_EVENT_GFX_WALLY,
		OBJECT,
		sText_QuestMenu_Found
	),

	sub_quest(
		16,
		gText_SubQuest2_Name7,
		gText_SubQuest2_Desc7,
		gText_SideQuestMap7,
		OBJ_EVENT_GFX_WALLY,
		OBJECT,
		sText_QuestMenu_Found
	),

	sub_quest(
		17,
		gText_SubQuest2_Name8,
		gText_SubQuest2_Desc8,
		gText_SideQuestMap8,
		OBJ_EVENT_GFX_WALLY,
		OBJECT,
		sText_QuestMenu_Found
	),

	sub_quest(
		18,
		gText_SubQuest2_Name9,
		gText_SubQuest2_Desc9,
		gText_SideQuestMap9,
		OBJ_EVENT_GFX_WALLY,
		OBJECT,
		sText_QuestMenu_Found
	),

	sub_quest(
		19,
		gText_SubQuest2_Name10,
		gText_SubQuest2_Desc10,
		gText_SideQuestMap10,
		OBJ_EVENT_GFX_WALLY,
		OBJECT,
		sText_QuestMenu_Found
	),

	sub_quest(
		20,
		gText_SubQuest2_Name11,
		gText_SubQuest2_Desc11,
		gText_SideQuestMap11,
		OBJ_EVENT_GFX_WALLY,
		OBJECT,
		sText_QuestMenu_Found
	),

	sub_quest(
		21,
		gText_SubQuest2_Name12,
		gText_SubQuest2_Desc12,
		gText_SideQuestMap12,
		OBJ_EVENT_GFX_WALLY,
		OBJECT,
		sText_QuestMenu_Found
	),

	sub_quest(
		22,
		gText_SubQuest2_Name13,
		gText_SubQuest2_Desc13,
		gText_SideQuestMap13,
		OBJ_EVENT_GFX_WALLY,
		OBJECT,
		sText_QuestMenu_Found
	),

	sub_quest(
		23,
		gText_SubQuest2_Name14,
		gText_SubQuest2_Desc14,
		gText_SideQuestMap14,
		OBJ_EVENT_GFX_WALLY,
		OBJECT,
		sText_QuestMenu_Found
	),

	sub_quest(
		24,
		gText_SubQuest2_Name15,
		gText_SubQuest2_Desc15,
		gText_SideQuestMap15,
		OBJ_EVENT_GFX_WALLY,
		OBJECT,
		sText_QuestMenu_Found
	),

	sub_quest(
		25,
		gText_SubQuest2_Name16,
		gText_SubQuest2_Desc16,
		gText_SideQuestMap16,
		OBJ_EVENT_GFX_WALLY,
		OBJECT,
		sText_QuestMenu_Found
	),

	sub_quest(
		26,
		gText_SubQuest2_Name17,
		gText_SubQuest2_Desc17,
		gText_SideQuestMap17,
		OBJ_EVENT_GFX_WALLY,
		OBJECT,
		sText_QuestMenu_Found
	),

	sub_quest(
		27,
		gText_SubQuest2_Name18,
		gText_SubQuest2_Desc18,
		gText_SideQuestMap18,
		OBJ_EVENT_GFX_WALLY,
		OBJECT,
		sText_QuestMenu_Found
	),

	sub_quest(
		28,
		gText_SubQuest2_Name19,
		gText_SubQuest2_Desc19,
		gText_SideQuestMap19,
		OBJ_EVENT_GFX_WALLY,
		OBJECT,
		sText_QuestMenu_Found
	),

	sub_quest(
		29,
		gText_SubQuest2_Name20,
		gText_SubQuest2_Desc20,
		gText_SideQuestMap20,
		OBJ_EVENT_GFX_WALLY,
		OBJECT,
		sText_QuestMenu_Found
	),

};


//Declaration of side quest structures. Edits to subquests are made here.
#define side_quest(n, d, dd, m, s, st, sq, ns) {.name = n, .desc = d, .donedesc = dd, .map = m, .sprite = s, .spritetype = st, .subquests = sq, .numSubquests = ns}
static const struct SideQuest sSideQuests[QUEST_COUNT] =
{
	side_quest(
	      gText_SideQuestName_1,
	      gText_SideQuestDesc_1,
	      gText_SideQuestDoneDesc_1,
	      gText_SideQuestMap1,
	      OBJ_EVENT_GFX_WALLY,
	      OBJECT,
	      sSubQuests1,
	      SUB_QUEST_1_COUNT
	),
	side_quest(
	      gText_SideQuestName_2,
	      gText_SideQuestDesc_2,
	      gText_SideQuestDoneDesc_2,
	      gText_SideQuestMap2,
	      OBJ_EVENT_GFX_WALLY,
	      OBJECT,
	      sSubQuests2,
	      SUB_QUEST_2_COUNT
	),
	side_quest(
	      gText_SideQuestName_3,
	      gText_SideQuestDesc_3,
	      gText_SideQuestDoneDesc_3,
	      gText_SideQuestMap3,
	      OBJ_EVENT_GFX_WALLY,
	      OBJECT,
	      NULL,
	      0
	),
	side_quest(
	      gText_SideQuestName_4,
	      gText_SideQuestDesc_4,
	      gText_SideQuestDoneDesc_4,
	      gText_SideQuestMap4,
	      OBJ_EVENT_GFX_WALLY,
	      OBJECT,
	      NULL,
	      0
	),
	side_quest(
	      gText_SideQuestName_5,
	      gText_SideQuestDesc_5,
	      gText_SideQuestDoneDesc_5,
	      gText_SideQuestMap5,
	      OBJ_EVENT_GFX_WALLY,
	      OBJECT,
	      NULL,
	      0
	),
	side_quest(
	      gText_SideQuestName_6,
	      gText_SideQuestDesc_6,
	      gText_SideQuestDoneDesc_6,
	      gText_SideQuestMap6,
	      OBJ_EVENT_GFX_WALLY,
	      OBJECT,
	      NULL,
	      0
	),
	side_quest(
	      gText_SideQuestName_7,
	      gText_SideQuestDesc_7,
	      gText_SideQuestDoneDesc_7,
	      gText_SideQuestMap7,
	      OBJ_EVENT_GFX_WALLY,
	      OBJECT,
	      NULL,
	      0
	),
	side_quest(
	      gText_SideQuestName_8,
	      gText_SideQuestDesc_8,
	      gText_SideQuestDoneDesc_8,
	      gText_SideQuestMap8,
	      OBJ_EVENT_GFX_WALLY,
	      OBJECT,
	      NULL,
	      0
	),
	side_quest(
	      gText_SideQuestName_9,
	      gText_SideQuestDesc_9,
	      gText_SideQuestDoneDesc_9,
	      gText_SideQuestMap9,
	      OBJ_EVENT_GFX_WALLY,
	      OBJECT,
	      NULL,
	      0
	),
	side_quest(
	      gText_SideQuestName_10,
	      gText_SideQuestDesc_10,
	      gText_SideQuestDoneDesc_10,
	      gText_SideQuestMap10,
	      OBJ_EVENT_GFX_WALLY,
	      OBJECT,
	      NULL,
	      0
	),
	side_quest(
	      gText_SideQuestName_11,
	      gText_SideQuestDesc_11,
	      gText_SideQuestDoneDesc_11,
	      gText_SideQuestMap11,
	      OBJ_EVENT_GFX_WALLY,
	      OBJECT,
	      NULL,
	      0
	),
	side_quest(
	      gText_SideQuestName_12,
	      gText_SideQuestDesc_12,
	      gText_SideQuestDoneDesc_12,
	      gText_SideQuestMap12,
	      OBJ_EVENT_GFX_WALLY,
	      OBJECT,
	      NULL,
	      0
	),
	side_quest(
	      gText_SideQuestName_13,
	      gText_SideQuestDesc_13,
	      gText_SideQuestDoneDesc_13,
	      gText_SideQuestMap13,
	      OBJ_EVENT_GFX_WALLY,
	      OBJECT,
	      NULL,
	      0
	),
	side_quest(
	      gText_SideQuestName_14,
	      gText_SideQuestDesc_14,
	      gText_SideQuestDoneDesc_14,
	      gText_SideQuestMap14,
	      OBJ_EVENT_GFX_WALLY,
	      OBJECT,
	      NULL,
	      0
	),
	side_quest(
	      gText_SideQuestName_15,
	      gText_SideQuestDesc_15,
	      gText_SideQuestDoneDesc_15,
	      gText_SideQuestMap15,
	      OBJ_EVENT_GFX_WALLY,
	      OBJECT,
	      NULL,
	      0
	),
	side_quest(
	      gText_SideQuestName_16,
	      gText_SideQuestDesc_16,
	      gText_SideQuestDoneDesc_16,
	      gText_SideQuestMap16,
	      OBJ_EVENT_GFX_WALLY,
	      OBJECT,
	      NULL,
	      0
	),
	side_quest(
	      gText_SideQuestName_17,
	      gText_SideQuestDesc_17,
	      gText_SideQuestDoneDesc_17,
	      gText_SideQuestMap17,
	      OBJ_EVENT_GFX_WALLY,
	      OBJECT,
	      NULL,
	      0
	),
	side_quest(
	      gText_SideQuestName_18,
	      gText_SideQuestDesc_18,
	      gText_SideQuestDoneDesc_18,
	      gText_SideQuestMap18,
	      OBJ_EVENT_GFX_WALLY,
	      OBJECT,
	      NULL,
	      0
	),
	side_quest(
	      gText_SideQuestName_19,
	      gText_SideQuestDesc_19,
	      gText_SideQuestDoneDesc_19,
	      gText_SideQuestMap19,
	      OBJ_EVENT_GFX_WALLY,
	      OBJECT,
	      NULL,
	      0
	),
	side_quest(
	      gText_SideQuestName_20,
	      gText_SideQuestDesc_20,
	      gText_SideQuestDoneDesc_20,
	      gText_SideQuestMap20,
	      OBJ_EVENT_GFX_WALLY,
	      OBJECT,
	      NULL,
	      0
	),
	side_quest(
	      gText_SideQuestName_21,
	      gText_SideQuestDesc_21,
	      gText_SideQuestDoneDesc_21,
	      gText_SideQuestMap21,
	      OBJ_EVENT_GFX_WALLY,
	      OBJECT,
	      NULL,
	      0
	),
	side_quest(
	      gText_SideQuestName_22,
	      gText_SideQuestDesc_22,
	      gText_SideQuestDoneDesc_22,
	      gText_SideQuestMap22,
	      OBJ_EVENT_GFX_WALLY,
	      OBJECT,
	      NULL,
	      0
	),
	side_quest(
	      gText_SideQuestName_23,
	      gText_SideQuestDesc_23,
	      gText_SideQuestDoneDesc_23,
	      gText_SideQuestMap23,
	      OBJ_EVENT_GFX_WALLY,
	      OBJECT,
	      NULL,
	      0
	),
	side_quest(
	      gText_SideQuestName_24,
	      gText_SideQuestDesc_24,
	      gText_SideQuestDoneDesc_24,
	      gText_SideQuestMap24,
	      OBJ_EVENT_GFX_WALLY,
	      OBJECT,
	      NULL,
	      0
	),
	side_quest(
	      gText_SideQuestName_25,
	      gText_SideQuestDesc_25,
	      gText_SideQuestDoneDesc_25,
	      gText_SideQuestMap25,
	      OBJ_EVENT_GFX_WALLY,
	      OBJECT,
	      NULL,
	      0
	),
	side_quest(
	      gText_SideQuestName_26,
	      gText_SideQuestDesc_26,
	      gText_SideQuestDoneDesc_26,
	      gText_SideQuestMap26,
	      OBJ_EVENT_GFX_WALLY,
	      OBJECT,
	      NULL,
	      0
	),
	side_quest(
	      gText_SideQuestName_27,
	      gText_SideQuestDesc_27,
	      gText_SideQuestDoneDesc_27,
	      gText_SideQuestMap27,
	      OBJ_EVENT_GFX_WALLY,
	      OBJECT,
	      NULL,
	      0
	),
	side_quest(
	      gText_SideQuestName_28,
	      gText_SideQuestDesc_28,
	      gText_SideQuestDoneDesc_28,
	      gText_SideQuestMap28,
	      OBJ_EVENT_GFX_WALLY,
	      OBJECT,
	      NULL,
	      0
	),
	side_quest(
	      gText_SideQuestName_29,
	      gText_SideQuestDesc_29,
	      gText_SideQuestDoneDesc_29,
	      gText_SideQuestMap29,
	      OBJ_EVENT_GFX_WALLY,
	      OBJECT,
	      NULL,
	      0
	),
	side_quest(
	      gText_SideQuestName_30,
	      gText_SideQuestDesc_30,
	      gText_SideQuestDoneDesc_30,
	      gText_SideQuestMap30,
	      OBJ_EVENT_GFX_WALLY,
	      OBJECT,
	      NULL,
	      0
	),
    /*
	side_quest(
	      side quest name string,
	      side quest description string,
	      side quest complete string,
	      side quest map string ,
	      quest object / item / pokemon id,
          object's type
          subquest struct
	      0
	),
     */
};

//BG layer defintions
static const struct BgTemplate sQuestMenuBgTemplates[2] =
{
	{
		//All text and content is loaded to this window
		.bg = 0,
		.charBaseIndex = 0,
		.mapBaseIndex = 31,
		.priority = 1
	},
	{
		///Backgrounds and UI elements are loaded to this window
		.bg = 1,
		.charBaseIndex = 3,
		.mapBaseIndex = 30,
		.priority = 2
	}
};

//Window definitions
static const struct WindowTemplate sQuestMenuHeaderWindowTemplates[] =
{
	{
		//0: Content window
		.bg = 0,
		.tilemapLeft = 0,
		.tilemapTop = 2,
		.width = 30,
		.height = 8,
		.paletteNum = 15,
		.baseBlock = 1
	},
	{
		//1: Footer window
		.bg = 0,
		.tilemapLeft = 0,
		.tilemapTop = 12,
		.width = 30,
		.height = 12,
		.paletteNum = 15,
		.baseBlock = 361
	},
	{
		// 2: Header window
		.bg = 0,
		.tilemapLeft = 0,
		.tilemapTop = 0,
		.width = 30,
		.height = 2,
		.paletteNum = 15,
		.baseBlock = 721
	},
	DUMMY_WIN_TEMPLATE
};

//Font color combinations for printed text
static const u8 sQuestMenuWindowFontColors[][4] =
{
	{
		//Header of Quest Menu
		TEXT_COLOR_TRANSPARENT,
		TEXT_COLOR_DARK_GRAY,
		TEXT_COLOR_TRANSPARENT
	},
	{
		//Reward state progress indicator
		TEXT_COLOR_TRANSPARENT,
		TEXT_COLOR_RED,
		TEXT_COLOR_TRANSPARENT
	},
	{
		//Done state progress indicator
		TEXT_COLOR_TRANSPARENT,
		TEXT_COLOR_GREEN,
		TEXT_COLOR_TRANSPARENT
	},
	{
		//Active state progress indicator
		TEXT_COLOR_TRANSPARENT,
		TEXT_COLOR_BLUE,
		TEXT_COLOR_TRANSPARENT
	},
	{
		//Footer flavor text
		TEXT_COLOR_TRANSPARENT,
		TEXT_COLOR_WHITE,
		TEXT_COLOR_TRANSPARENT
	},
};

//Functions begin here

//ported from firered by ghoulslash
void QuestMenu_Init(u8 a0, MainCallback callback)
{
	u8 i;

	if (a0 >= 2)
	{
		SetMainCallback2(callback);
		return;
	}

	if ((sStateDataPtr = Alloc(sizeof(struct QuestMenuResources))) == NULL)
	{
		SetMainCallback2(callback);
		return;
	}

	if (a0 != 1)
	{
		sListMenuState.savedCallback = callback;
		sListMenuState.scroll = sListMenuState.row = 0;
	}

	sStateDataPtr->moveModeOrigPos = 0xFF;
	sStateDataPtr->spriteIconSlot = 0;
	sStateDataPtr->scrollIndicatorArrowPairId = 0xFF;
	sStateDataPtr->savedCallback = 0;
	for (i = 0; i < 3; i++)
	{
		sStateDataPtr->data[i] = 0;
	}

	SetMainCallback2(QuestMenu_RunSetup);
}

static void QuestMenu_MainCB(void)
{
	RunTasks();
	AnimateSprites();
	BuildOamBuffer();
	DoScheduledBgTilemapCopiesToVram();
	UpdatePaletteFade();
}

static void QuestMenu_VBlankCB(void)
{
	LoadOam();
	ProcessSpriteCopyRequests();
	TransferPlttBuffer();
}

static void QuestMenu_RunSetup(void)
{
	while (1)
	{
		if (QuestMenu_SetupGraphics() == TRUE)
		{
			break;
		}
	}
}

static bool8 QuestMenu_SetupGraphics(void)
{
	u8 taskId;
	switch (gMain.state)
	{
		case 0:
			SetVBlankHBlankCallbacksToNull();
			ClearScheduledBgCopiesToVram();
			gMain.state++;
			break;
		case 1:
			ScanlineEffect_Stop();
			gMain.state++;
			break;
		case 2:
			FreeAllSpritePalettes();
			gMain.state++;
			break;
		case 3:
			ResetPaletteFade();
			gMain.state++;
			break;
		case 4:
			ResetSpriteData();
			gMain.state++;
			break;
		case 5:
			QuestMenu_ResetSpriteState();
			gMain.state++;
			break;
		case 6:
			ResetTasks();
			gMain.state++;
			break;
		case 7:
			if (QuestMenu_InitBackgrounds())
			{
				sStateDataPtr->data[0] = 0;
				gMain.state++;
			}
			else
			{
				QuestMenu_FadeAndBail();
				return TRUE;
			}
			break;
		case 8:
			if (QuestMenu_LoadGraphics() == TRUE)
			{
				gMain.state++;
			}
			break;
		case 9:
			//Loads the background, text and sprites will still spawn
			QuestMenu_InitWindows();
			gMain.state++;
			break;
		case 10:
			//When commented out, question marks loads for every slot and page does not scroll when going past number 6
			QuestMenu_ClearModeOnStartup();
			QuestMenu_InitItems();
			//Doesn't seem to do anything?
			QuestMenu_SetCursorPosition();
			//Doesn't seem to do anything?
			QuestMenu_SetScrollPosition();
			gMain.state++;
			break;
		case 11:
			//If allocating resource for the itemsin quest menu works, then advance, otherwise quit the quest menu
			if (QuestMenu_AllocateResourcesForListMenu())
			{
				gMain.state++;
			}
			else
			{
				QuestMenu_FadeAndBail();
				return TRUE;
			}
			break;
		case 12:
			//print the quest titles, avatars, desc and status
			//When this is gone, page does not seem to play nice
			QuestMenu_AllocateMemoryForArray();
			QuestMenu_BuildMenuTemplate();
			gMain.state++;
			break;
		case 13:
			//header does not print
			QuestMenu_GenerateAndPrintHeader();
			gMain.state++;
			break;
		case 14:
			//sub_80985E4();
			gMain.state++;
			break;
		case 15:
			//everything loads, but cannot scroll or quit the meun
			taskId = CreateTask(Task_QuestMenu_Main, 0);
			//background loads but interface is entirely glitched out
			gTasks[taskId].data[0] = ListMenuInit(&gMultiuseListMenuTemplate,
			                                      sListMenuState.scroll,
			                                      sListMenuState.row);
			gMain.state++;
			break;
		case 16:
			//arrows at the top and bottom don't appear without this
			QuestMenu_PlaceTopMenuScrollIndicatorArrows();
			gMain.state++;
			break;
		case 17:
			gMain.state++;
			break;
		case 18:
			//unknown
			if (sListMenuState.initialized == 1)
			{
				BlendPalettes(0xFFFFFFFF, 16, RGB_BLACK);
			}
			gMain.state++;
			break;
		case 19:
			//unknown
			if (sListMenuState.initialized == 1)
			{
				BeginNormalPaletteFade(0xFFFFFFFF, 0, 16, 0, RGB_BLACK);
			}
			else
			{

				BeginNormalPaletteFade(0xFFFFFFFF, 0, 16, 0, RGB_BLACK);
				QuestMenu_SetInitializedFlag(1);
			}
			gMain.state++;
			break;
		default:
			//quest menu begins and loads, you can quit, but cannot see or interact
			SetVBlankCallback(QuestMenu_VBlankCB);
			//screen goes to black, nothing else happens
			SetMainCallback2(QuestMenu_MainCB);
			return TRUE;
	}
	return FALSE;
}

static void QuestMenu_FadeAndBail(void)
{
	BeginNormalPaletteFade(0xFFFFFFFF, 0, 0, 16, RGB_BLACK);
	CreateTask(Task_QuestMenuWaitFadeAndBail, 0);
	SetVBlankCallback(QuestMenu_VBlankCB);
	SetMainCallback2(QuestMenu_MainCB);
}

static void Task_QuestMenuWaitFadeAndBail(u8 taskId)
{
	if (!gPaletteFade.active)
	{
		SetMainCallback2(sListMenuState.savedCallback);
		QuestMenu_FreeResources();
		DestroyTask(taskId);
	}
}

static bool8 QuestMenu_InitBackgrounds(void)
{
	ResetAllBgsCoordinatesAndBgCntRegs();
	sBg1TilemapBuffer = Alloc(0x800);
	if (sBg1TilemapBuffer == NULL)
	{
		return FALSE;
	}

	memset(sBg1TilemapBuffer, 0, 0x800);
	ResetBgsAndClearDma3BusyFlags(0);
	InitBgsFromTemplates(0, sQuestMenuBgTemplates,
	                     NELEMS(sQuestMenuBgTemplates));
	SetBgTilemapBuffer(1, sBg1TilemapBuffer);
	ScheduleBgCopyTilemapToVram(1);
	SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_OBJ_1D_MAP | DISPCNT_OBJ_ON);
	SetGpuReg(REG_OFFSET_BLDCNT, 0);
	ShowBg(0);
	ShowBg(1);
	return TRUE;
}

static bool8 QuestMenu_LoadGraphics(void)
{
	switch (sStateDataPtr->data[0])
	{
		case 0:
			ResetTempTileDataBuffers();
			DecompressAndCopyTileDataToVram(1, sQuestMenuTiles, 0, 0, 0);
			sStateDataPtr->data[0]++;
			break;
		case 1:
			if (FreeTempTileDataBuffersIfPossible() != TRUE)
			{
				LZDecompressWram(sQuestMenuTilemap, sBg1TilemapBuffer);
				sStateDataPtr->data[0]++;
			}
			break;
		case 2:
			LoadCompressedPalette(sQuestMenuBgPals, 0x00, 0x60);
			sStateDataPtr->data[0]++;
			break;
		case 3:
			sStateDataPtr->data[0]++;
			break;
		default:
			sStateDataPtr->data[0] = 0;
			return TRUE;
	}
	return FALSE;
}

#define try_alloc(ptr__, size) ({ \
		void ** ptr = (void **)&(ptr__);             \
		*ptr = Alloc(size);                 \
		if (*ptr == NULL)                   \
		{                                   \
			QuestMenu_FreeResources();                  \
			QuestMenu_FadeAndBail();                  \
			return FALSE;                   \
		}                                   \
	})

static bool8 QuestMenu_AllocateResourcesForListMenu(void)
{
	try_alloc(sListMenuItems,
	          sizeof(struct ListMenuItem) * QuestMenu_CountNumberListRows() + 1);
	return TRUE;
}

void QuestMenu_AllocateMemoryForArray(void)
{
	u8 i;
	questNameArray = Alloc(sizeof(void *) * QUEST_COUNT + 1);

	for (i = 0; i < 32; i++)
	{
		questNameArray[i] = Alloc(sizeof(u8) * 32);
	}
}

static s8 QuestMenu_DoesQuestHaveChildrenAndNotInactive(u16 itemId)
{
	if (sSideQuests[itemId].numSubquests != 0
	            && QuestMenu_GetSetQuestState(itemId, FLAG_GET_UNLOCKED)
	            && !QuestMenu_GetSetQuestState(itemId, FLAG_GET_INACTIVE))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

static bool8 QuestMenu_IsSubquestMode(void)
{
	if (sStateDataPtr->filterMode > SORT_DONE_AZ)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

static u8 QuestMenu_CountNumberListRows()
{
	u8 mode = sStateDataPtr->filterMode % 10;

	if (QuestMenu_IsSubquestMode())
	{
		return sSideQuests[sStateDataPtr->parentQuest].numSubquests + 1;
	}

	switch (mode)
	{
		case SORT_DEFAULT:
			return QUEST_COUNT + 1;
		case SORT_INACTIVE:
			return QuestMenu_CountInactiveQuests() + 1;
		case SORT_ACTIVE:
			return QuestMenu_CountActiveQuests() + 1;
		case SORT_REWARD:
			return QuestMenu_CountRewardQuests() + 1;
		case SORT_DONE:
			return QuestMenu_CountCompletedQuests() + 1;
	}

}


static bool8 QuestMenu_IsNotFilteredMode(void)
{
	u8 mode = sStateDataPtr->filterMode % 10;

	if (mode == FLAG_GET_UNLOCKED)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

static bool8 QuestMenu_IsAlphaMode(void)
{
	if (sStateDataPtr->filterMode < SORT_SUBQUEST
	            && sStateDataPtr->filterMode > SORT_DONE)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

static void QuestMenu_AssignCancelNameAndId(u8 numRow)
{
	if (QuestMenu_IsSubquestMode())
	{
		sListMenuItems[numRow].name = sText_QuestMenu_Back;
	}
	else
	{
		sListMenuItems[numRow].name = sText_QuestMenu_Close;
	}

	sListMenuItems[numRow].id = LIST_CANCEL;
}

u8 QuestMenu_GenerateSubquestList()
{
	u8 parentQuest = sStateDataPtr->parentQuest;
	u8 mode = sStateDataPtr->filterMode % 10;
	u8 lastRow = 0, numRow = 0, countQuest = 0;

	for (numRow = 0; numRow < sSideQuests[parentQuest].numSubquests; numRow++)
	{
		QuestMenu_PrependQuestNumber(countQuest);
		QuestMenu_PopulateSubquestTitle(parentQuest, countQuest);
		QuestMenu_PopulateListRowNameAndId(numRow, countQuest);

		countQuest++;
		lastRow = numRow + 1;
	}
	return lastRow;
}

u8 QuestMenu_PopulateListRowNameAndId(u8 row, u8 countQuest)
{
	sListMenuItems[row].name = questNameArray[countQuest];
	sListMenuItems[row].id = countQuest;
}

bool8 QuestMenu_IsSubquestCompleted(u8 parentQuest, u8 countQuest)
{
	if (QuestMenu_GetSetSubquestState(parentQuest, FLAG_GET_COMPLETED,
	                                  countQuest))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void QuestMenu_PopulateSubquestTitle(u8 parentQuest, u8 countQuest)
{
	if (QuestMenu_IsSubquestCompleted(parentQuest, countQuest))
	{
		questNamePointer = StringAppend(questNamePointer,
		                                sSideQuests[parentQuest].subquests[countQuest].name);
	}
	else
	{
		questNamePointer = StringAppend(questNamePointer, sText_QuestMenu_Unk);
	}
}

void QuestMenu_PrependQuestNumber(u8 countQuest)
{
	questNamePointer = ConvertIntToDecimalStringN(questNameArray[countQuest],
	                   countQuest + 1, STR_CONV_MODE_LEFT_ALIGN, 2);
	questNamePointer = StringAppend(questNamePointer,
	                                sText_QuestMenu_DotSpace);
}

u8 QuestMenu_GenerateFilteredList()
{
	u8 mode = sStateDataPtr->filterMode % 10;
	u8 lastRow = 0, numRow = 0, offset = 0, newRow = 0, countQuest = 0,
	   selectedQuestId = 0;
	u8 *sortedQuestList;

	sortedQuestList = QuestMenu_DefineQuestOrder();

	for (countQuest = 0; countQuest < sStateDataPtr->nItems; countQuest++)
	{
		selectedQuestId = *(sortedQuestList + countQuest);

		if (QuestMenu_GetSetQuestState(selectedQuestId, mode))
		{
			QuestMenu_PopulateEmptyRow(selectedQuestId);

			if (QuestMenu_GetSetQuestState(selectedQuestId,
			                               FLAG_GET_FAVORITE)) //how do only conditionally show this line? //PSF TODO
			{
				QuestMenu_SetFavoriteQuest(selectedQuestId);
				newRow = numRow;
				numRow++;
			}
			else
			{
				newRow = QuestMenu_CountFavoriteAndState() + offset;
				offset++;
			}

			QuestMenu_PopulateQuestName(selectedQuestId);
			QuestMenu_PopulateListRowNameAndId(newRow, selectedQuestId);
		}
	}
	return numRow + offset;
}

void QuestMenu_SetFavoriteQuest(u8 countQuest)
{
	questNamePointer = StringAppend(questNameArray[countQuest],
	                                sText_QuestMenu_ColorGreen);
}

u8 *QuestMenu_DefineQuestOrder()
{
	static u8 sortedList[QUEST_COUNT];
	u8 a, c, d, e;
	u8 placeholderVariable;

	for (a = 0; a < QUEST_COUNT; a++)
	{
		sortedList[a] = a;
	}

	if (QuestMenu_IsAlphaMode())
	{
		for (c = 0; c < QUEST_COUNT; c++)
		{
			for (d = c + 1; d < QUEST_COUNT; d++)
			{
				if (StringCompare(sSideQuests[sortedList[c]].name,
				                  sSideQuests[sortedList[d]].name) > 0)
				{
					placeholderVariable = sortedList[c];
					sortedList[c] = sortedList[d];
					sortedList[d] = placeholderVariable;
				}
			}
		}
	}

	return sortedList;
}

void QuestMenu_PopulateQuestName(u8 countQuest)
{
	if (QuestMenu_GetSetQuestState(countQuest, FLAG_GET_UNLOCKED))
	{
		questNamePointer = StringAppend(questNameArray[countQuest],
		                                sSideQuests[countQuest].name);
		QuestMenu_AddSubQuestButton(countQuest);
	}
	else
	{
		StringAppend(questNameArray[countQuest], sText_QuestMenu_Unk);
	}
}

void QuestMenu_AddSubQuestButton(u8 countQuest)
{
	if (QuestMenu_DoesQuestHaveChildrenAndNotInactive(countQuest))
	{
		questNamePointer = StringAppend(questNameArray[countQuest],
		                                sText_QuestMenu_SubQuestButton);
	}

}

void QuestMenu_PopulateEmptyRow(u8 countQuest)
{
	questNamePointer = StringCopy(questNameArray[countQuest], sText_Empty);
}


u8 QuestMenu_GenerateDefaultList()
{
	u8 mode = sStateDataPtr->filterMode % 10;
	u8 lastRow = 0, numRow = 0, offset = 0, newRow = 0, countQuest = 0,
	   selectedQuestId = 0;
	u8 *sortedQuestList;

	sortedQuestList = QuestMenu_DefineQuestOrder();

	for (countQuest = 0; countQuest < sStateDataPtr->nItems; countQuest++)
	{
		selectedQuestId = *(sortedQuestList + countQuest);
		QuestMenu_PopulateEmptyRow(selectedQuestId);

		if (QuestMenu_GetSetQuestState(selectedQuestId, FLAG_GET_FAVORITE))
		{
			QuestMenu_SetFavoriteQuest(selectedQuestId);
			newRow = numRow;
			numRow++;
		}
		else
		{
			newRow = QuestMenu_CountFavoriteQuests() +
			         offset; //merge with CountState into one function PSF TODO
			offset++;
		}

		QuestMenu_PopulateQuestName(selectedQuestId);
		QuestMenu_PopulateListRowNameAndId(newRow, selectedQuestId);
		lastRow = numRow + offset;
	}
	return lastRow;
}

u8 QuestMenu_GetModeAndGenerateList()
{
	MgbaPrintf(4, "size of subquests %lu", sizeof(gSaveBlock2Ptr->subQuests));
	MgbaPrintf(4, "size of questdata%lu", sizeof(gSaveBlock2Ptr->questData));

	if (QuestMenu_IsSubquestMode())
	{
		return QuestMenu_GenerateSubquestList();
	}
	else if (!QuestMenu_IsNotFilteredMode())
	{
		return QuestMenu_GenerateFilteredList();
	}
	else
	{
		return QuestMenu_GenerateDefaultList();
	}
}

static u16 QuestMenu_BuildMenuTemplate(void)
{
	u8 lastRow = QuestMenu_GetModeAndGenerateList();

	QuestMenu_AssignCancelNameAndId(lastRow);

	gMultiuseListMenuTemplate.totalItems = QuestMenu_CountNumberListRows();
	gMultiuseListMenuTemplate.items = sListMenuItems;
	gMultiuseListMenuTemplate.windowId = 0;
	gMultiuseListMenuTemplate.header_X = 0;
	gMultiuseListMenuTemplate.cursor_X = 15;
	gMultiuseListMenuTemplate.item_X = 23;
	gMultiuseListMenuTemplate.lettersSpacing = 1;
	gMultiuseListMenuTemplate.itemVerticalPadding = 2;
	gMultiuseListMenuTemplate.upText_Y = 2;
	gMultiuseListMenuTemplate.maxShowed = sStateDataPtr->maxShowed;
	gMultiuseListMenuTemplate.fontId = 2;
	gMultiuseListMenuTemplate.cursorPal = 2;
	gMultiuseListMenuTemplate.fillValue = 0;
	gMultiuseListMenuTemplate.cursorShadowPal = 0;
	gMultiuseListMenuTemplate.moveCursorFunc = QuestMenu_MoveCursorFunc;
	gMultiuseListMenuTemplate.itemPrintFunc = QuestMenu_GenerateStateAndPrint;
	gMultiuseListMenuTemplate.scrollMultiple = LIST_MULTIPLE_SCROLL_DPAD;
	gMultiuseListMenuTemplate.cursorKind = 0;
}

void QuestMenu_CreateSprite(u16 itemId, u8 idx, u8 spriteType)
{
	u8 *ptr = &sItemMenuIconSpriteIds[10];
	u8 spriteId;
	struct SpriteSheet spriteSheet;
	struct CompressedSpritePalette spritePalette;
	struct SpriteTemplate *spriteTemplate;

	if (ptr[idx] == 0xFF)
	{
		FreeSpriteTilesByTag(102 + idx);
		FreeSpritePaletteByTag(102 + idx);

		switch (spriteType)
		{
			case OBJECT:
				spriteId = CreateObjectGraphicsSprite(itemId, SpriteCallbackDummy, 20,
				                                      132, 0);
				break;
			case ITEM:
				spriteId = AddItemIconSprite(102 + idx, 102 + idx, itemId);
				break;
			default:
				break;
		}

		gSprites[spriteId].oam.objMode = ST_OAM_OBJ_BLEND;

		if (spriteId != MAX_SPRITES)
		{
			ptr[idx] = spriteId;

			if (spriteType == ITEM)
			{
				gSprites[spriteId].x2 = 24;
				gSprites[spriteId].y2 = 140;
			}
		}
	}
}

void QuestMenu_ResetSpriteState(void)
{
	u16 i;

	for (i = 0; i < NELEMS(sItemMenuIconSpriteIds); i++)
	{
		sItemMenuIconSpriteIds[i] = 0xFF;
	}
}

void QuestMenu_DestroySprite(u8 idx)
{
	u8 *ptr = &sItemMenuIconSpriteIds[10];

	if (ptr[idx] != 0xFF)
	{
		DestroySpriteAndFreeResources(&gSprites[ptr[idx]]);
		ptr[idx] = 0xFF;
	}
}

void QuestMenu_PlayCursorSound(bool8 firstRun)
{
	if (firstRun == FALSE)
	{
		PlaySE(SE_RG_BAG_CURSOR);
	}
}

void QuestMenu_PrintDetailsForCancel()
{
	FillWindowPixelBuffer(1, 0);

	QuestMenu_AddTextPrinterParameterized(1, 2, sText_Empty, 2, 3, 2, 0, 0,
	                                      0);
	QuestMenu_AddTextPrinterParameterized(1, 2, sText_Empty, 40, 19, 5, 0, 0,
	                                      0);

	QuestMenu_CreateSprite(-1, sStateDataPtr->spriteIconSlot, ITEM);
}

static void QuestMenu_MoveCursorFunc(s32 questId, bool8 onInit,
                                     struct ListMenu *list)
{
	QuestMenu_PlayCursorSound(onInit);

	if (sStateDataPtr->moveModeOrigPos == 0xFF)
	{
		QuestMenu_DestroySprite(sStateDataPtr->spriteIconSlot ^ 1);
		sStateDataPtr->spriteIconSlot ^= 1;

		if (questId == LIST_CANCEL)
		{
			QuestMenu_PrintDetailsForCancel();
		}
		else
		{
			QuestMenu_GenerateAndPrintQuestDetails(questId);
			QuestMenu_DetermineSpriteType(questId);
		}
	}
}

void QuestMenu_DetermineSpriteType(s32 questId)
{
	u16 spriteId;
    u8 spriteType;

	if (QuestMenu_IsSubquestMode() == FALSE)
	{
		spriteId = sSideQuests[questId].sprite;
        spriteType = sSideQuests[questId].spritetype;

		QuestMenu_CreateSprite(spriteId, sStateDataPtr->spriteIconSlot, spriteType);
	}
	else if (QuestMenu_IsSubquestCompletedState(questId) == TRUE)
	{
		spriteId =
		      sSideQuests[sStateDataPtr->parentQuest].subquests[questId].sprite;
        spriteType = 
		      sSideQuests[sStateDataPtr->parentQuest].subquests[questId].spritetype;
		QuestMenu_CreateSprite(spriteId, sStateDataPtr->spriteIconSlot, spriteType);
	}
	else
	{
		QuestMenu_CreateSprite(ITEM_NONE, sStateDataPtr->spriteIconSlot, ITEM);
	}
	QuestMenu_DestroySprite(sStateDataPtr->spriteIconSlot ^ 1);
	sStateDataPtr->spriteIconSlot ^= 1;
}

void QuestMenu_GenerateAndPrintQuestDetails(s32 questId)
{
	QuestMenu_GenerateQuestLocation(questId);
	QuestMenu_PrintQuestLocation(questId);
	QuestMenu_GenerateQuestFlavorText(questId);
	QuestMenu_PrintQuestFlavorText(questId);
}

void QuestMenu_GenerateQuestLocation(s32 questId)
{
	if (!QuestMenu_IsSubquestMode())
	{
		StringCopy(gStringVar2, sSideQuests[questId].map);
	}
	else
	{
		StringCopy(gStringVar2,
		           sSideQuests[sStateDataPtr->parentQuest].subquests[questId].map);
	}

	StringExpandPlaceholders(gStringVar4, sText_QuestMenu_ShowLocation);
}

void QuestMenu_PrintQuestLocation(s32 questId)
{
	FillWindowPixelBuffer(1, 0);
	QuestMenu_AddTextPrinterParameterized(1, 2, gStringVar4, 2, 3, 2, 0, 0,
	                                      4);
}

void QuestMenu_GenerateQuestFlavorText(s32 questId)
{
	if (QuestMenu_IsSubquestMode() == FALSE)
	{
		if (QuestMenu_IsQuestInactive(questId) == TRUE)
		{
			StringCopy(gStringVar1, sText_QuestMenu_StartForMore);
		}
		if (QuestMenu_IsQuestActive(questId) == TRUE)
		{
			QuestMenu_UpdateQuestFlavorText(questId);
		}
		if (QuestMenu_IsQuestRewardState(questId) == TRUE)
		{
			StringCopy(gStringVar1, sText_QuestMenu_ReturnRecieveReward);
		}
		if (QuestMenu_IsQuestCompletedState(questId) == TRUE)
		{
			StringCopy(gStringVar1, sSideQuests[questId].donedesc);
		}
	}
	else
	{
		if (QuestMenu_IsSubquestCompletedState(questId) == TRUE)
		{
			StringCopy(gStringVar1,
			           sSideQuests[sStateDataPtr->parentQuest].subquests[questId].desc);
		}
		else
		{
			StringCopy(gStringVar1, sText_Empty);
		}
	}

	StringExpandPlaceholders(gStringVar3, gStringVar1);
}

void QuestMenu_PrintQuestFlavorText(s32 questId)
{
	QuestMenu_AddTextPrinterParameterized(1, 2, gStringVar3, 40, 19, 5, 0, 0,
	                                      4);
}

void QuestMenu_UpdateQuestFlavorText(s32 questId)
{
	StringCopy(gStringVar1, sSideQuests[questId].desc);
}

bool8 QuestMenu_IsQuestInactive(s32 questId)
{
	if (!QuestMenu_GetSetQuestState(questId, FLAG_GET_ACTIVE))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

bool8 QuestMenu_IsQuestActive(s32 questId)
{
	if (QuestMenu_GetSetQuestState(questId, FLAG_GET_ACTIVE))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

bool8 QuestMenu_IsSubquestCompletedState(s32 questId)
{
	if (QuestMenu_GetSetSubquestState(sStateDataPtr->parentQuest,
	                                  FLAG_GET_COMPLETED,
	                                  questId))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

bool8 QuestMenu_IsQuestRewardState(s32 questId)
{
	if (QuestMenu_GetSetQuestState(questId, FLAG_GET_REWARD))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

bool8 QuestMenu_IsQuestCompletedState(s32 questId)
{
	if (QuestMenu_GetSetQuestState(questId, FLAG_GET_COMPLETED))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

bool8 QuestMenu_IsQuestUnlocked(s32 questId)
{
	if (QuestMenu_GetSetQuestState(questId, FLAG_GET_UNLOCKED))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

u8 QuestMenu_GenerateSubquestState(u8 questId)
{
	u8 parentQuest = sStateDataPtr->parentQuest;

	if (QuestMenu_GetSetSubquestState(parentQuest, FLAG_GET_COMPLETED,
	                                  questId))
	{
        StringCopy(gStringVar4, sSideQuests[parentQuest].subquests[questId].type);
	}
	else
	{
		StringCopy(gStringVar4, sText_Empty);
	}

	return 2;
}

u8 QuestMenu_GenerateQuestState(u8 questId)
{
	if (QuestMenu_GetSetQuestState(questId, FLAG_GET_COMPLETED))
	{
		StringCopy(gStringVar4, sText_QuestMenu_Complete);
		return 2;
	}
	else if (QuestMenu_GetSetQuestState(questId, FLAG_GET_REWARD))
	{
		StringCopy(gStringVar4, sText_QuestMenu_Reward);
		return 1;
	}
	else if (QuestMenu_GetSetQuestState(questId, FLAG_GET_ACTIVE))
	{
		StringCopy(gStringVar4, sText_QuestMenu_Active);
		return 3;
	}
	else
	{
		StringCopy(gStringVar4, sText_Empty);
	}
}

static void QuestMenu_GenerateStateAndPrint(u8 windowId, u32 questId,
            u8 y)
{
	u8 colorIndex;

	if (questId != LIST_CANCEL)
	{
		if (QuestMenu_IsSubquestMode())
		{
			colorIndex = QuestMenu_GenerateSubquestState(questId);
		}
		else
		{
			colorIndex = QuestMenu_GenerateQuestState(questId);
		}

		QuestMenu_PrintQuestState(windowId, y, colorIndex);
	}
}

void QuestMenu_PrintQuestState(u8 windowId, u8 y, u8 colorIndex)
{
	QuestMenu_AddTextPrinterParameterized(windowId, 0, gStringVar4, 200, y, 0,
	                                      0, 0xFF, colorIndex);
}

static void QuestMenu_PrintOrRemoveCursor(u8 listMenuId, u8 colorIdx)
{
	QuestMenu_PrintOrRemoveCursorAt(ListMenuGetYCoordForPrintingArrowCursor(
	                                      listMenuId), colorIdx);
}

static void QuestMenu_PrintOrRemoveCursorAt(u8 y, u8 colorIdx)
{
	if (colorIdx == 0xFF)
	{
		u8 maxWidth = GetFontAttribute(1, FONTATTR_MAX_LETTER_WIDTH);
		u8 maxHeight = GetFontAttribute(1, FONTATTR_MAX_LETTER_HEIGHT);
		FillWindowPixelRect(0, 0, 0, y, maxWidth, maxHeight);
	}
	else
	{
		QuestMenu_AddTextPrinterParameterized(0, 2, gText_SelectorArrow, 0, y, 0,
		                                      0, 0, colorIdx);
	}
}

s8 QuestMenu_CountUnlockedQuests(void)
{
	u8 q = 0, i = 0;

	for (i = 0; i < QUEST_COUNT; i++)
	{
		if (QuestMenu_GetSetQuestState(i, FLAG_GET_UNLOCKED))
		{
			q++;
		}
	}
	return q;
}

s8 QuestMenu_CountInactiveQuests(void)
{
	u8 q = 0, i = 0;

	for (i = 0; i < QUEST_COUNT; i++)
	{
		if (QuestMenu_GetSetQuestState(i, FLAG_GET_INACTIVE))
		{
			q++;
		}
	}
	return q;
}

s8 QuestMenu_CountActiveQuests(void)
{
	u8 q = 0, i = 0;

	for (i = 0; i < QUEST_COUNT; i++)
	{
		if (QuestMenu_GetSetQuestState(i, FLAG_GET_ACTIVE))
		{
			q++;
		}
	}
	return q;
}

s8 QuestMenu_CountRewardQuests(void)
{
	u8 q = 0, i = 0;

	for (i = 0; i < QUEST_COUNT; i++)
	{
		if (QuestMenu_GetSetQuestState(i, FLAG_GET_REWARD))
		{
			q++;
		}
	}
	return q;
}

s8 QuestMenu_CountFavoriteQuests(void)
{
	u8 q = 0, i = 0;

	for (i = 0; i < QUEST_COUNT; i++)
	{
		if (QuestMenu_GetSetQuestState(i, FLAG_GET_FAVORITE))
		{
			q++;
		}
	}
	return q;
}

s8 QuestMenu_CountFavoriteAndState(void)
{
	u8 q = 0, i = 0;

	u8 mode = sStateDataPtr->filterMode % 10;

	for (i = 0; i < QUEST_COUNT; i++)
	{
		if (QuestMenu_GetSetQuestState(i, mode)
		            && QuestMenu_GetSetQuestState(i, FLAG_GET_FAVORITE))
		{
			q++;
		}
	}

	return q;
}

s8 QuestMenu_CountCompletedQuests(void)
{
	u8 q = 0, i = 0;

	u8 parentQuest = sStateDataPtr->parentQuest;

	if (QuestMenu_IsSubquestMode())
	{
		for (i = 0; i < sSideQuests[parentQuest].numSubquests; i++)
		{
			if (QuestMenu_GetSetSubquestState(parentQuest, FLAG_GET_COMPLETED, i))
			{
				q++;
			}
		}
	}
	else
	{
		for (i = 0; i < QUEST_COUNT; i++)
		{
			if (QuestMenu_GetSetQuestState(i, FLAG_GET_COMPLETED))
			{
				q++;
			}
		}
	}

	return q;
}

void QuestMenu_GenerateDenominatorNumQuests(void)
{
	ConvertIntToDecimalStringN(gStringVar2, QUEST_COUNT,
	                           STR_CONV_MODE_LEFT_ALIGN, 6);
}

void QuestMenu_GenerateNumeratorNumQuests(void)
{
	u8 mode = sStateDataPtr->filterMode % 10;
	u8 parentQuest = sStateDataPtr->parentQuest;

	switch (mode)
	{
		case SORT_DEFAULT:
			ConvertIntToDecimalStringN(gStringVar1, QuestMenu_CountUnlockedQuests(),
			                           STR_CONV_MODE_LEFT_ALIGN,
			                           6);
			break;
		case SORT_INACTIVE:
			ConvertIntToDecimalStringN(gStringVar1, QuestMenu_CountInactiveQuests(),
			                           STR_CONV_MODE_LEFT_ALIGN,
			                           6);
			break;
		case SORT_ACTIVE:
			ConvertIntToDecimalStringN(gStringVar1, QuestMenu_CountActiveQuests(),
			                           STR_CONV_MODE_LEFT_ALIGN, 6);
			break;
		case SORT_REWARD:
			ConvertIntToDecimalStringN(gStringVar1, QuestMenu_CountRewardQuests(),
			                           STR_CONV_MODE_LEFT_ALIGN, 6);
			break;
		case SORT_DONE:
			ConvertIntToDecimalStringN(gStringVar1, QuestMenu_CountCompletedQuests(),
			                           STR_CONV_MODE_LEFT_ALIGN,
			                           6);
			break;
	}

	if (QuestMenu_IsSubquestMode())
	{
		ConvertIntToDecimalStringN(gStringVar2,
		                           sSideQuests[parentQuest].numSubquests,
		                           STR_CONV_MODE_LEFT_ALIGN, 6);
		ConvertIntToDecimalStringN(gStringVar1, QuestMenu_CountCompletedQuests(),
		                           STR_CONV_MODE_LEFT_ALIGN,
		                           6);
	}
}

void QuestMenu_GenerateMenuContext(void)
{
	u8 mode = sStateDataPtr->filterMode % 10;
	u8 parentQuest = sStateDataPtr->parentQuest;

	switch (mode)
	{
		case SORT_DEFAULT:
			questNamePointer = StringCopy(questNameArray[QUEST_COUNT + 1],
			                              sText_QuestMenu_AllHeader);
			break;
		case SORT_INACTIVE:
			questNamePointer = StringCopy(questNameArray[QUEST_COUNT + 1],
			                              sText_QuestMenu_InactiveHeader);
			break;
		case SORT_ACTIVE:
			questNamePointer = StringCopy(questNameArray[QUEST_COUNT + 1],
			                              sText_QuestMenu_ActiveHeader);
			break;
		case SORT_REWARD:
			questNamePointer = StringCopy(questNameArray[QUEST_COUNT + 1],
			                              sText_QuestMenu_RewardHeader);
			break;
		case SORT_DONE:
			questNamePointer = StringCopy(questNameArray[QUEST_COUNT + 1],
			                              sText_QuestMenu_CompletedHeader);
			break;
	}

	if (QuestMenu_IsAlphaMode())
	{
		questNamePointer = StringAppend(questNameArray[QUEST_COUNT + 1],
		                                sText_QuestMenu_AZ);
	}
	if (QuestMenu_IsSubquestMode())
	{
		questNamePointer = StringCopy(questNameArray[QUEST_COUNT + 1],
		                              sSideQuests[parentQuest].name);

	}
}

void QuestMenu_PrintNumQuests(void)
{
	StringExpandPlaceholders(gStringVar4, sText_QuestMenu_QuestNumberDisplay);
	QuestMenu_AddTextPrinterParameterized(2, 0, gStringVar4, 167, 1, 0, 1, 0,
	                                      0);
}
void QuestMenu_PrintMenuContext(void)
{
	QuestMenu_AddTextPrinterParameterized(2, 0,
	                                      questNameArray[QUEST_COUNT + 1], 10, 1, 0, 1, 0, 0);
}
void QuestMenu_PrintTypeFilterButton(void)
{
	QuestMenu_AddTextPrinterParameterized(2, 0, sText_QuestMenu_Type, 198, 1,
	                                      0, 1, 0, 0);

}

static void QuestMenu_GenerateAndPrintHeader(void)
{
	QuestMenu_GenerateDenominatorNumQuests();
	QuestMenu_GenerateNumeratorNumQuests();
	QuestMenu_GenerateMenuContext();

	QuestMenu_PrintNumQuests();
	QuestMenu_PrintMenuContext();

	if (!QuestMenu_IsSubquestMode())
	{
		QuestMenu_PrintTypeFilterButton();
	}
}

static void QuestMenu_PlaceTopMenuScrollIndicatorArrows(void)
{
	u8 listSize = QuestMenu_CountNumberListRows();

	if (listSize < sStateDataPtr->maxShowed)
	{
		listSize = sStateDataPtr->maxShowed;
	}

	sStateDataPtr->scrollIndicatorArrowPairId =
	      AddScrollIndicatorArrowPairParameterized(2, 94, 8, 90,
	                  (listSize - sStateDataPtr->maxShowed), 110, 110, &sListMenuState.scroll);
}

static void QuestMenu_RemoveScrollIndicatorArrowPair(void)
{
	if (sStateDataPtr->scrollIndicatorArrowPairId != 0xFF)
	{
		RemoveScrollIndicatorArrowPair(sStateDataPtr->scrollIndicatorArrowPairId);
		sStateDataPtr->scrollIndicatorArrowPairId = 0xFF;
	}
}

bool8 QuestMenu_IfScrollIsOutOfBounds(void)
{
	if (sListMenuState.scroll != 0
	            && sListMenuState.scroll + sStateDataPtr->maxShowed >
	            sStateDataPtr->nItems + 1)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

bool8 QuestMenu_IfRowIsOutOfBounds(void)
{
	if (sListMenuState.scroll + sListMenuState.row >= sStateDataPtr->nItems +
	            1)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

static void QuestMenu_SetCursorPosition(void)
{
	if (QuestMenu_IfScrollIsOutOfBounds())
	{
		sListMenuState.scroll = (sStateDataPtr->nItems + 1) -
		                        sStateDataPtr->maxShowed;
	}

	if (QuestMenu_IfRowIsOutOfBounds())
	{
		if (sStateDataPtr->nItems + 1 < 2)
		{
			sListMenuState.row = 0;
		}
		else
		{
			sListMenuState.row = sStateDataPtr->nItems;
		}
	}
}

#define try_free(ptr) ({        \
		void ** ptr__ = (void **)&(ptr);   \
		if (*ptr__ != NULL)                \
			Free(*ptr__);                  \
	})

static void QuestMenu_FreeResources(void)
{
	int i;

	try_free(sStateDataPtr);
	try_free(sBg1TilemapBuffer);
	try_free(sListMenuItems);

	for (i = 31; i > -1 ; i--)
	{
		try_free(questNameArray[i]);
	}

	try_free(questNameArray);
	FreeAllWindowBuffers();
}

static void Task_QuestMenuTurnOff1(u8 taskId)
{
	BeginNormalPaletteFade(0xFFFFFFFF, 0, 0, 16, RGB_BLACK);
	gTasks[taskId].func = Task_QuestMenuTurnOff2;
}

static void Task_QuestMenuTurnOff2(u8 taskId)
{
	s16 *data = gTasks[taskId].data;

	if (!gPaletteFade.active)
	{
		DestroyListMenuTask(data[0], &sListMenuState.scroll, &sListMenuState.row);
		if (sStateDataPtr->savedCallback != NULL)
		{
			SetMainCallback2(sStateDataPtr->savedCallback);
		}
		else
		{
			SetMainCallback2(sListMenuState.savedCallback);
		}

		QuestMenu_RemoveScrollIndicatorArrowPair();
		QuestMenu_FreeResources();
		DestroyTask(taskId);
	}
}

static u8 QuestMenu_GetCursorPosition(void)
{
	return sListMenuState.scroll + sListMenuState.row;
}


static void QuestMenu_InitItems(void)
{
	sStateDataPtr->nItems = (QuestMenu_CountNumberListRows()) - 1;

	sStateDataPtr->maxShowed = sStateDataPtr->nItems + 1 <= 4 ?
	                           sStateDataPtr->nItems + 1 : 4;
}

static void QuestMenu_SetScrollPosition(void)
{
	u8 i;

	if (sListMenuState.row > 3)
	{
		for (i = 0; i <= sListMenuState.row - 3;
		            sListMenuState.row--, sListMenuState.scroll++, i++)
		{
			if (sListMenuState.scroll + sStateDataPtr->maxShowed ==
			            sStateDataPtr->nItems + 1)
			{
				break;
			}
		}
	}
}

static void QuestMenu_SetInitializedFlag(u8 a0)
{
	sListMenuState.initialized = a0;
}

static s8 QuestMenu_ManageMode(u8 action)
{
	u8 mode = sStateDataPtr->filterMode;

	switch (action)
	{
		case SUB:
			mode = QuestMenu_ToggleSubquestMode(mode);
			break;

		case ALPHA:
			mode = QuestMenu_ToggleAlphaMode(mode);
			break;

		default:
			mode = QuestMenu_IncrementMode(mode);
			break;
	}
	sStateDataPtr->filterMode = mode;
}

u8 QuestMenu_ToggleSubquestMode(u8 mode)
{
	if (QuestMenu_IsSubquestMode())
	{
		mode -= SORT_SUBQUEST;
	}
	else
	{
		mode += SORT_SUBQUEST;
	}

	return mode;
}

u8 QuestMenu_ToggleAlphaMode(u8 mode)
{
	if (QuestMenu_IsAlphaMode())
	{
		mode -= SORT_DEFAULT_AZ;
	}
	else
	{
		mode += SORT_DEFAULT_AZ;
	}

	return mode;
}

u8 QuestMenu_IncrementMode(u8 mode)
{
	if (mode % 10 == SORT_DONE)
	{
		mode -= SORT_DONE;
	}
	else
	{
		mode++;
	}

	return mode;
}


static void QuestMenu_SaveScrollAndRow(s16 *data)
{
	ListMenuGetScrollAndRow(data[0], &sListMenuState.storedScrollOffset,
	                        &sListMenuState.storedRowPosition);
}

static void QuestMenu_ResetCursorToTop(s16 *data)
{
	sListMenuState.row = 0;
	sListMenuState.scroll = 0;
	data[0] = ListMenuInit(&gMultiuseListMenuTemplate, sListMenuState.scroll,
	                       sListMenuState.row);
}

static void QuestMenu_RestoreSavedScrollAndRow(s16 *data)
{
	data[0] = ListMenuInit(&gMultiuseListMenuTemplate,
	                       sListMenuState.storedScrollOffset,
	                       sListMenuState.storedRowPosition);
}

//PSF TODO refactor stop

void QuestMenu_ChangeModeAndCleanUp(u8 taskId)
{
	if (!QuestMenu_IsSubquestMode())
	{
		PlaySE(SE_SELECT);
		QuestMenu_ManageMode(INCREMENT);
		sStateDataPtr->restoreCursor = FALSE;
		Task_QuestMenuCleanUp(taskId);
	}
}

void QuestMenu_ToggleAlphaModeAndCleanUp(u8 taskId)
{
	if (!QuestMenu_IsSubquestMode())
	{
		PlaySE(SE_SELECT);
		QuestMenu_ManageMode(ALPHA);
		sStateDataPtr->restoreCursor = FALSE;
		Task_QuestMenuCleanUp(taskId);
	}
}

bool8 QuestMenu_CheckSelectedIsCancel(u8 selectedQuestId)
{
	if (selectedQuestId == (0xFF - 1))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void QuestMenu_ToggleFavoriteAndCleanUp(u8 taskId, u8 selectedQuestId)
{
	if (!QuestMenu_IsSubquestMode()
	            && !QuestMenu_CheckSelectedIsCancel(selectedQuestId))
	{
		PlaySE(SE_SELECT);
		QuestMenu_ManageFavorites(selectedQuestId);
		sStateDataPtr->restoreCursor = FALSE;
		Task_QuestMenuCleanUp(taskId);
	}
}

void QuestMenu_ReturnFromSubquestAndCleanUp(u8 taskId)
{
	QuestMenu_PrepareFadeOut(taskId);

	PlaySE(SE_SELECT);
	QuestMenu_ManageMode(SUB);
	sStateDataPtr->restoreCursor = TRUE;
	gTasks[taskId].func = Task_QuestMenu_FadeOut;
}

void QuestMenu_TurnOffQuestMenu(u8 taskId)
{
	QuestMenu_SetInitializedFlag(0);
	gTasks[taskId].func = Task_QuestMenuTurnOff1;
}

void QuestMenu_EnterSubquestModeAndCleanUp(u8 taskId, s16 *data,
            s32 input)
{
	if (QuestMenu_DoesQuestHaveChildrenAndNotInactive(input))
	{
		QuestMenu_PrepareFadeOut(taskId);

		PlaySE(SE_SELECT);
		sStateDataPtr->parentQuest = input;
		QuestMenu_ManageMode(SUB);
		sStateDataPtr->restoreCursor = FALSE;
		QuestMenu_SaveScrollAndRow(data);
		gTasks[taskId].func = Task_QuestMenu_FadeOut;
	}
}

static void Task_QuestMenu_Main(u8 taskId)
{
	s16 *data = gTasks[taskId].data;
	s32 input = ListMenu_ProcessInput(data[0]);

	u8 selectedQuestId = sListMenuItems[QuestMenu_GetCursorPosition()].id;

	if (!gPaletteFade.active)
	{
		ListMenuGetScrollAndRow(data[0], &sListMenuState.scroll,
		                        &sListMenuState.row);

		switch (input)
		{
			case LIST_NOTHING_CHOSEN:
				if (JOY_NEW(R_BUTTON))
				{
					QuestMenu_ChangeModeAndCleanUp(taskId);
				}
				if (JOY_NEW(START_BUTTON))
				{
					QuestMenu_ToggleAlphaModeAndCleanUp(taskId);
				}
				if (JOY_NEW(SELECT_BUTTON))
				{
					QuestMenu_ToggleFavoriteAndCleanUp(taskId, selectedQuestId);
				}
				break;

			case LIST_CANCEL:
				if (QuestMenu_IsSubquestMode())
				{
					QuestMenu_ReturnFromSubquestAndCleanUp(taskId);
				}
				else
				{
					QuestMenu_TurnOffQuestMenu(taskId);
				}
				break;

			default:
				if (!QuestMenu_IsSubquestMode())
				{
					QuestMenu_EnterSubquestModeAndCleanUp(taskId, data, input);
				}
				break;
		}
	}
}

static void Task_QuestMenuCleanUp(u8 taskId)
{
	s16 *data = gTasks[taskId].data;

	QuestMenu_RemoveScrollIndicatorArrowPair();
	DestroyListMenuTask(data[0], &sListMenuState.scroll, &sListMenuState.row);
	ClearStdWindowAndFrameToTransparent(2, FALSE);

	QuestMenu_GenerateAndPrintHeader();
	QuestMenu_AllocateResourcesForListMenu();
	QuestMenu_BuildMenuTemplate();
	QuestMenu_PlaceTopMenuScrollIndicatorArrows();

	if (sStateDataPtr->restoreCursor == TRUE)
	{
		QuestMenu_RestoreSavedScrollAndRow(data);
	}
	else
	{
		QuestMenu_ResetCursorToTop(data);
	}

}

// pokefirered text_window.c
static void QuestMenu_InitWindows(void)
{
	u8 i;

	InitWindows(sQuestMenuHeaderWindowTemplates);
	DeactivateAllTextPrinters();

	for (i = 0; i < 3; i++)
	{
		FillWindowPixelBuffer(i, 0x00);
		PutWindowTilemap(i);
	}

	ScheduleBgCopyTilemapToVram(0);
}

static void QuestMenu_AddTextPrinterParameterized(u8 windowId, u8 fontId,
            const u8 *str, u8 x, u8 y,
            u8 letterSpacing, u8 lineSpacing, u8 speed, u8 colorIdx)
{
	AddTextPrinterParameterized4(windowId, fontId, x, y, letterSpacing,
	                             lineSpacing,
	                             sQuestMenuWindowFontColors[colorIdx], speed, str);
}

void Task_QuestMenu_OpenFromStartMenu(u8 taskId)
{
	s16 *data = gTasks[taskId].data;
	if (!gPaletteFade.active)
	{
		CleanupOverworldWindowsAndTilemaps();
		QuestMenu_Init(tItemPcParam, CB2_ReturnToFieldWithOpenMenu);
		DestroyTask(taskId);
	}
}


s8 QuestMenu_ManageFavorites(u8 selectedQuestId)
{
	if (QuestMenu_GetSetQuestState(selectedQuestId, FLAG_GET_FAVORITE))
	{
		QuestMenu_GetSetQuestState(selectedQuestId, FLAG_REMOVE_FAVORITE);
	}
	else
	{
		QuestMenu_GetSetQuestState(selectedQuestId, FLAG_SET_FAVORITE);
	}
}

s8 QuestMenu_GetSetSubquestState(u8 quest, u8 caseId, u8 childQuest)
{

	//PSF TODO our version of this was only wasn't using index at all. I replaced uniqueId with index and it still works. I assume if I hadn't fixed this, it would have overflowed evantually?

	u8 uniqueId = sSideQuests[quest].subquests[childQuest].id;
	u8  index = uniqueId / 8; //8 bits per byte
	u8	bit = uniqueId % 8;
	u8	mask = 1 << bit;

	switch (caseId)
	{
		case FLAG_GET_COMPLETED:
			return gSaveBlock2Ptr->subQuests[index] & mask;
		case FLAG_SET_COMPLETED:
			gSaveBlock2Ptr->subQuests[index] |= mask;
			return 1;
	}

	return -1;
}

s8 QuestMenu_GetSetQuestState(u8 quest, u8 caseId)
{
	u8 index = quest * 5 / 8;
	u8 bit = quest * 5 % 8;
	u8 mask = 0, index2 = 0, bit2 = 0, index3 = 0, bit3 = 0, mask2 = 0,
	   mask3 = 0;

	// 0 : locked
	// 1 : actived
	// 2 : rewarded
	// 3 : completed
	// 4 : favorited

	switch (caseId)
	{
		case FLAG_GET_UNLOCKED:
		case FLAG_SET_UNLOCKED:
			break;
		case FLAG_GET_INACTIVE:
		case FLAG_GET_ACTIVE:
		case FLAG_SET_ACTIVE:
		case FLAG_REMOVE_ACTIVE:
			bit += 1;
			break;
		case FLAG_GET_REWARD:
		case FLAG_SET_REWARD:
		case FLAG_REMOVE_REWARD:
			bit += 2;
			break;
		case FLAG_GET_COMPLETED:
		case FLAG_SET_COMPLETED:
			bit += 3;
			break;
		case FLAG_GET_FAVORITE:
		case FLAG_SET_FAVORITE:
		case FLAG_REMOVE_FAVORITE:
			bit += 4;
			break;
	}
	if (bit >= 8)
	{
		index += 1;
		bit %= 8;
	}
	mask = 1 << bit;

	switch (caseId)
	{
		case FLAG_GET_UNLOCKED:
			return gSaveBlock2Ptr->questData[index] & mask;
		case FLAG_SET_UNLOCKED:
			gSaveBlock2Ptr->questData[index] |= mask;
			return 1;
		case FLAG_GET_INACTIVE:
			bit2 = bit + 1;
			bit3 = bit + 2;
			index2 = index;
			index3 = index;

			if (bit2 >= 8)
			{
				index2 += 1;
				bit2 %= 8;
			}
			if (bit3 >= 8)
			{
				index3 += 1;
				bit3 %= 8;
			}

			mask2 = 1 << bit2;
			mask3 = 1 << bit3;
			return !(gSaveBlock2Ptr->questData[index] & mask) && \
			       !(gSaveBlock2Ptr->questData[index2] & mask2) && \
			       !(gSaveBlock2Ptr->questData[index3] & mask3);
		case FLAG_GET_ACTIVE:
			return gSaveBlock2Ptr->questData[index] & mask;
		case FLAG_SET_ACTIVE:
			gSaveBlock2Ptr->questData[index] |= mask;
			return 1;
		case FLAG_REMOVE_ACTIVE:
			gSaveBlock2Ptr->questData[index] &= ~mask;
			return 1;
		case FLAG_GET_REWARD:
			return gSaveBlock2Ptr->questData[index] & mask;
		case FLAG_SET_REWARD:
			gSaveBlock2Ptr->questData[index] |= mask;
			return 1;
		case FLAG_REMOVE_REWARD:
			gSaveBlock2Ptr->questData[index] &= ~mask;
			return 1;
		case FLAG_GET_COMPLETED:
			return gSaveBlock2Ptr->questData[index] & mask;
		case FLAG_SET_COMPLETED:
			gSaveBlock2Ptr->questData[index] |= mask;
			return 1;
		case FLAG_GET_FAVORITE:
			return gSaveBlock2Ptr->questData[index] & mask;
		case FLAG_SET_FAVORITE:
			gSaveBlock2Ptr->questData[index] |= mask;
			return 1;
		case FLAG_REMOVE_FAVORITE:
			gSaveBlock2Ptr->questData[index] &= ~mask;
			return 1;
	}
	return -1;  //failure
}

void QuestMenu_ActivateMenu(void)
{
	FlagSet(FLAG_QUEST_MENU_ACTIVE);
}

void QuestMenu_CopyQuestName(u8 *dst, u8 questId)
{
	StringCopy(dst, sSideQuests[questId].name);
}

void QuestMenu_CopySubquestName(u8 *dst, u8 parentId, u8 childId)
{
	StringCopy(dst,sSideQuests[parentId].subquests[childId].name);
}

void QuestMenu_ResetMenuSaveData(void)
{
	memset(&gSaveBlock2Ptr->questData, 0,
	       sizeof(gSaveBlock2Ptr->questData));
	memset(&gSaveBlock2Ptr->subQuests, 0,
	       sizeof(gSaveBlock2Ptr->subQuests));
}

static void
QuestMenu_SetGpuRegBaseForFade() //Sets the GPU registers to prepare for a hardware fade
{
	SetGpuReg(REG_OFFSET_BLDCNT,
	          BLDCNT_TGT1_OBJ | BLDCNT_TGT1_BG0 | BLDCNT_TGT2_BG1 |
	          BLDCNT_EFFECT_BLEND);      //Blend Sprites and BG0 into BG1
	SetGpuReg(REG_OFFSET_BLDY, 0);
}

#define MAX_FADE_INTENSITY 16
#define MIN_FADE_INTENSITY 0

void QuestMenu_InitFadeVariables(u8 taskId, u8 blendWeight, u8 frameDelay,
                                 u8 frameTimerBase, u8 delta)
{
	gTasks[taskId].data[1] = blendWeight;
	gTasks[taskId].data[2] = frameDelay;
	gTasks[taskId].data[3] = gTasks[taskId].data[frameTimerBase];
	gTasks[taskId].data[4] = delta;
}

static void QuestMenu_PrepareFadeOut(u8 taskId)
{
	QuestMenu_SetGpuRegBaseForFade();
	SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(MAX_FADE_INTENSITY, 0));
	QuestMenu_InitFadeVariables(taskId, MAX_FADE_INTENSITY, 0, 2, 2);
}

static bool8 QuestMenu_HandleFadeOut(u8 taskId)
{
	if (gTasks[taskId].data[3]-- != 0)
	{
		return FALSE;
	}

	//Set the timer, decrease the fade weight by the delta, increase the delta by the timer
	gTasks[taskId].data[3] = gTasks[taskId].data[2];
	gTasks[taskId].data[1] -= gTasks[taskId].data[4];
	gTasks[taskId].data[2] += gTasks[taskId].data[3];

	//When blend weight runs out, set final blend and quit
	if (gTasks[taskId].data[1] <= 0)
	{
		SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(0, gTasks[taskId].data[1]));
		return TRUE;
	}
	//Set intermediate blend state
	SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(gTasks[taskId].data[1],
	            MAX_FADE_INTENSITY - gTasks[taskId].data[1]));
	return FALSE;
}

static void QuestMenu_PrepareFadeIn(u8
                                    taskId) //Prepares the input handler for a hardware fade in
{
	QuestMenu_SetGpuRegBaseForFade();
	SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(0,
	            MAX_FADE_INTENSITY));
	QuestMenu_InitFadeVariables(taskId, MIN_FADE_INTENSITY, 0, 1, 2);
}

static bool8 QuestMenu_HandleFadeIn(u8
                                    taskId) //Handles the hardware fade in
{
	//Set the timer, ncrease the fade weight by the delta,
	gTasks[taskId].data[3] = gTasks[taskId].data[2];
	gTasks[taskId].data[1] += gTasks[taskId].data[4];

	//When blend weight reaches max, set final blend and quit
	if (gTasks[taskId].data[1] >= MAX_FADE_INTENSITY)
	{
		SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(MAX_FADE_INTENSITY,
		            MIN_FADE_INTENSITY));
		return TRUE;
	}
	//Set intermediate blend state
	SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(gTasks[taskId].data[1],
	            MAX_FADE_INTENSITY - gTasks[taskId].data[1]));
	return FALSE;
}

static void Task_QuestMenu_FadeOut(u8 taskId)
{
	if (QuestMenu_HandleFadeOut(taskId))
	{
		QuestMenu_PrepareFadeIn(taskId);
		Task_QuestMenuCleanUp(taskId);
		gTasks[taskId].func = Task_QuestMenu_FadeIn;
	}
}

static void Task_QuestMenu_FadeIn(u8 taskId)
{
	if (QuestMenu_HandleFadeIn(taskId))
	{
		gTasks[taskId].func = Task_QuestMenu_Main;
	}
}

void QuestMenu_ClearModeOnStartup(void)
{
	sStateDataPtr->filterMode = 0;
}
