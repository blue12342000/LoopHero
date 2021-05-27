#include "ImageManager.h"
#include "Image.h"

HRESULT ImageManager::Init()
{
    mLpImages.insert(make_pair("ingame_backbuffer", new Image()));
    mLpImages["ingame_backbuffer"]->Init(644 * 2, 364 * 2);

    mLpImages.insert(make_pair("title_background", new Image()));
    mLpImages["title_background"]->Init("Image/UI/Title/title_background.bmp", 644 * 2, 364 * 2);

    mLpImages.insert(make_pair("InGame_BackGround", new Image()));
    mLpImages["InGame_BackGround"]->Init("Image/UI/InGame/InGame_BackGround.bmp", 644 * 2, 364 * 2);

    mLpImages.insert(make_pair("ingame_bosstimer_background", new Image()));
    mLpImages["ingame_bosstimer_background"]->Init("Image/UI/InGame/ingame_bosstimer_background.bmp", 121 * 2, 27 * 2);

    mLpImages.insert(make_pair("ingame_info_background", new Image()));
    mLpImages["ingame_info_background"]->Init("Image/UI/InGame/ingame_info_background.bmp", 296 * 2, 360 * 2, 2, 1, 2, true, RGB(0, 170, 255));

    mLpImages.insert(make_pair("battle_background", new Image()));
    mLpImages["battle_background"]->Init("Image/UI/InGame/battle_background.bmp", 301 * 2, 257 * 2, 1, 1, 1, true, RGB(0, 170, 255));

    mLpImages.insert(make_pair("battle_background", new Image()));
    mLpImages["battle_background"]->Init("Image/UI/InGame/battle_background.bmp", 301 * 2, 257 * 2, 1, 1, 1, true, RGB(0, 170, 255));

    mLpImages.insert(make_pair("battle_unit_statusbar", new Image()));
    mLpImages["battle_unit_statusbar"]->Init("Image/UI/InGame/battle_unit_statusbar.bmp", 37 * 2, 7 * 2, true, RGB(0, 170, 255));

    mLpImages.insert(make_pair("battle_unit_statusbar_hp", new Image()));
    mLpImages["battle_unit_statusbar_hp"]->Init("Image/UI/InGame/battle_unit_statusbar_hp.bmp", 25 * 2, 2 * 2);

    mLpImages.insert(make_pair("battle_unit_statusbar_action", new Image()));
    mLpImages["battle_unit_statusbar_action"]->Init("Image/UI/InGame/battle_unit_statusbar_action.bmp", 25 * 2, 1 * 2);

    mLpImages.insert(make_pair("battle_unit_statusbar_stamina", new Image()));
    mLpImages["battle_unit_statusbar_stamina"]->Init("Image/UI/InGame/battle_unit_statusbar_stamina.bmp", 25 * 2, 1 * 2);

    mLpImages.insert(make_pair("spider_icon", new Image()));
    mLpImages["spider_icon"]->Init("Image/Icon/spider_icon.bmp", 29 * 2, 29 * 2);
    mLpImages.insert(make_pair("spider_card", new Image()));
    mLpImages["spider_card"]->Init("Image/Cards/spider_card.bmp", 41 * 2, 58 * 2, true, RGB(0, 170, 255));

    mLpImages.insert(make_pair("village_icon", new Image()));
    mLpImages["village_icon"]->Init("Image/Icon/village_icon.bmp", 29 * 2, 29 * 2);
    mLpImages.insert(make_pair("village_card", new Image()));
    mLpImages["village_card"]->Init("Image/Cards/village_card.bmp", 41 * 2, 58 * 2, true, RGB(0, 170, 255));

    mLpImages.insert(make_pair("mountain_icon", new Image()));
    mLpImages["mountain_icon"]->Init("Image/Icon/mountain_icon.bmp", 29 * 2, 29 * 2);
    mLpImages.insert(make_pair("mountain_card", new Image()));
    mLpImages["mountain_card"]->Init("Image/Cards/mountain_card.bmp", 41 * 2, 58 * 2, true, RGB(0, 170, 255));

    mLpImages.insert(make_pair("battlefield_icon", new Image()));
    mLpImages["battlefield_icon"]->Init("Image/Icon/battlefield_icon.bmp", 29 * 2, 29 * 2);
    mLpImages.insert(make_pair("battlefield_card", new Image()));
    mLpImages["battlefield_card"]->Init("Image/Cards/battlefield_card.bmp", 41 * 2, 58 * 2, true, RGB(0, 170, 255));

    mLpImages.insert(make_pair("bloodgrove_icon", new Image()));
    mLpImages["bloodgrove_icon"]->Init("Image/Icon/bloodgrove_icon.bmp", 29 * 2, 29 * 2);
    mLpImages.insert(make_pair("bloodgrove_card", new Image()));
    mLpImages["bloodgrove_card"]->Init("Image/Cards/bloodgrove_card.bmp", 41 * 2, 58 * 2, true, RGB(0, 170, 255));

    mLpImages.insert(make_pair("cemetery_icon", new Image()));
    mLpImages["cemetery_icon"]->Init("Image/Icon/cemetery_icon.bmp", 29 * 2, 29 * 2);
    mLpImages.insert(make_pair("cemetery_card", new Image()));
    mLpImages["cemetery_card"]->Init("Image/Cards/cemetery_card.bmp", 41 * 2, 58 * 2, true, RGB(0, 170, 255));

    mLpImages.insert(make_pair("crystal_icon", new Image()));
    mLpImages["crystal_icon"]->Init("Image/Icon/crystal_icon.bmp", 29 * 2, 29 * 2);
    mLpImages.insert(make_pair("crystal_card", new Image()));
    mLpImages["crystal_card"]->Init("Image/Cards/crystal_card.bmp", 41 * 2, 58 * 2, true, RGB(0, 170, 255));

    mLpImages.insert(make_pair("grove_icon", new Image()));
    mLpImages["grove_icon"]->Init("Image/Icon/grove_icon.bmp", 29 * 2, 29 * 2);
    mLpImages.insert(make_pair("grove_card", new Image()));
    mLpImages["grove_card"]->Init("Image/Cards/grove_card.bmp", 41 * 2, 58 * 2, true, RGB(0, 170, 255));

    mLpImages.insert(make_pair("lantern_icon", new Image()));
    mLpImages["lantern_icon"]->Init("Image/Icon/lantern_icon.bmp", 29 * 2, 29 * 2);
    mLpImages.insert(make_pair("lantern_card", new Image()));
    mLpImages["lantern_card"]->Init("Image/Cards/lantern_card.bmp", 41 * 2, 58 * 2, true, RGB(0, 170, 255));

    mLpImages.insert(make_pair("meadom_icon", new Image()));
    mLpImages["meadom_icon"]->Init("Image/Icon/meadom_icon.bmp", 29 * 2, 29 * 2);
    mLpImages.insert(make_pair("meadom_card", new Image()));
    mLpImages["meadom_card"]->Init("Image/Cards/meadom_card.bmp", 41 * 2, 58 * 2, true, RGB(0, 170, 255));

    mLpImages.insert(make_pair("swamp_icon", new Image()));
    mLpImages["swamp_icon"]->Init("Image/Icon/swamp_icon.bmp", 29 * 2, 29 * 2);
    mLpImages.insert(make_pair("swamp_card", new Image()));
    mLpImages["swamp_card"]->Init("Image/Cards/swamp_card.bmp", 41 * 2, 58 * 2, true, RGB(0, 170, 255));

    mLpImages.insert(make_pair("oblivion_icon", new Image()));
    mLpImages["oblivion_icon"]->Init("Image/Icon/oblivion_icon.bmp", 29 * 2, 29 * 2);
    mLpImages.insert(make_pair("oblivion_card", new Image()));
    mLpImages["oblivion_card"]->Init("Image/Cards/oblivion_card.bmp", 41 * 2, 58 * 2, true, RGB(0, 170, 255));

    mLpImages.insert(make_pair("rock_icon", new Image()));
    mLpImages["rock_icon"]->Init("Image/Icon/rock_icon.bmp", 29 * 2, 29 * 2);
    mLpImages.insert(make_pair("rock_card", new Image()));
    mLpImages["rock_card"]->Init("Image/Cards/rock_card.bmp", 41 * 2, 58 * 2, true, RGB(0, 170, 255));

    mLpImages.insert(make_pair("tresury_icon", new Image()));
    mLpImages["tresury_icon"]->Init("Image/Icon/tresury_icon.bmp", 29 * 2, 29 * 2);
    mLpImages.insert(make_pair("tresury_card", new Image()));
    mLpImages["tresury_card"]->Init("Image/Cards/tresury_card.bmp", 41 * 2, 58 * 2, true, RGB(0, 170, 255));

    mLpImages.insert(make_pair("item", new Image()));
    mLpImages["item"]->Init("Image/Item/item.bmp", 375 * 2, 325 * 2, 15, 13, 15*13, true, RGB(0, 170, 255));

    mLpImages.insert(make_pair("item_rank", new Image()));
    mLpImages["item_rank"]->Init("Image/Item/item_rank.bmp", 92 * 2, 23 * 2, 4, 1, 4, true, RGB(0, 170, 255));

    mLpImages.insert(make_pair("selection_33", new Image()));
    mLpImages["selection_33"]->Init("Image/UI/Common/selection_33.bmp", 66 * 2, 33 * 2, 2, 1, 2, true, RGB(0, 170, 255));

    mLpImages.insert(make_pair("battle_unit_hover_80_58", new Image()));
    mLpImages["battle_unit_hover_80_58"]->Init("Image/UI/InGame/battle_unit_hover_80_58.bmp", 80 * 2, 58 * 2, 1, 1, 1, true, RGB(0, 170, 255));

    mLpImages.insert(make_pair("battle_unit_hover_80_92", new Image()));
    mLpImages["battle_unit_hover_80_92"]->Init("Image/UI/InGame/battle_unit_hover_80_92.bmp", 80 * 2, 92 * 2, 1, 1, 1, true, RGB(0, 170, 255));

    mLpImages.insert(make_pair("button_exit", new Image()));
    mLpImages["button_exit"]->Init("Image/UI/InGame/Button/button_exit.bmp", 35 * 2, 87 * 2, 1, 3, 3, true, RGB(0, 170, 255));

    // ���� ======================================================================
    mLpImages.insert(make_pair("Warrior_icon", new Image()));
    mLpImages["Warrior_icon"]->Init("Image/ObjectIcon/hero_icon.bmp", 56 * 2, 14 * 2, 4, 1, 4, true, RGB(0, 170, 255));
    mLpImages.insert(make_pair("Warrior_idle", new Image()));
    mLpImages["Warrior_idle"]->Init("Image/Hero/Warrior/hero_idle.bmp", 60 * 2, 39 * 2, 1, 1, 1, true, RGB(0, 170, 255));
    mLpImages.insert(make_pair("Warrior_attack", new Image()));
    mLpImages["Warrior_attack"]->Init("Image/Hero/Warrior/hero_attack.bmp", 300 * 2, 39 * 2, 5, 1, 5, true, RGB(0, 170, 255));
    mLpImages.insert(make_pair("Warrior_hurt", new Image()));
    mLpImages["Warrior_hurt"]->Init("Image/Hero/Warrior/hero_hurt.bmp", 240 * 2, 39 * 2, 4, 1, 4, true, RGB(0, 170, 255));
    mLpImages.insert(make_pair("Warrior_death", new Image()));
    mLpImages["Warrior_death"]->Init("Image/Hero/Warrior/hero_death.bmp", 300 * 2, 39 * 2, 5, 1, 5, true, RGB(0, 170, 255));
    mLpImages.insert(make_pair("Warrior_revive", new Image()));
    mLpImages["Warrior_revive"]->Init("Image/Hero/Warrior/hero_revive.bmp", 360 * 2, 39 * 2, 6, 1, 6, true, RGB(0, 170, 255));

    mLpImages.insert(make_pair("Slime_icon", new Image()));
    mLpImages["Slime_icon"]->Init("Image/ObjectIcon/slime_icon.bmp", (int)(28 * 1.5f), (int)(14 * 1.5f), 2, 1, 2, true, RGB(0, 170, 255));
    mLpImages.insert(make_pair("Slime_idle", new Image()));
    mLpImages["Slime_idle"]->Init("Image/Enemy/Slime/slime_idle.bmp", 60 * 2, 39 * 2, 1, 1, 1, true, RGB(0, 170, 255));
    mLpImages.insert(make_pair("Slime_attack", new Image()));
    mLpImages["Slime_attack"]->Init("Image/Enemy/Slime/slime_attack.bmp", 300 * 2, 39 * 2, 5, 1, 5, true, RGB(0, 170, 255));
    mLpImages.insert(make_pair("Slime_hurt", new Image()));
    mLpImages["Slime_hurt"]->Init("Image/Enemy/Slime/slime_hurt.bmp", 240 * 2, 39 * 2, 4, 1, 4, true, RGB(0, 170, 255));
    mLpImages.insert(make_pair("Slime_death", new Image()));
    mLpImages["Slime_death"]->Init("Image/Enemy/Slime/slime_death.bmp", 300 * 2, 39 * 2, 5, 1, 5, true, RGB(0, 170, 255));
    mLpImages.insert(make_pair("Slime_revive", new Image()));
    mLpImages["Slime_revive"]->Init("Image/Enemy/Slime/Slime_idle.bmp", 60 * 2, 39 * 2, 1, 1, 1, true, RGB(0, 170, 255));
    // ���� ======================================================================

    // Ÿ�� ======================================================================
    mLpImages.insert(make_pair("possible_tile", new Image()));
    mLpImages["possible_tile"]->Init("Image/Tiles/possible_tile.bmp", 25 * 2, 25 * 2, 1, 1, 1, true, RGB(0, 170, 255));

    mLpImages.insert(make_pair("oblivion_tile", new Image()));
    mLpImages["oblivion_tile"]->Init("Image/Tiles/oblivion_tile.bmp", 25 * 2, 34 * 2, 1, 1, 1, true, RGB(0, 170, 255));
    mLpImages.insert(make_pair("oblivion_tile_select", new Image()));
    mLpImages["oblivion_tile_select"]->Init("Image/Tiles/oblivion_tile_select.bmp", 25 * 2, 34 * 2, 1, 1, 1, true, RGB(0, 170, 255));
    mLpImages.insert(make_pair("oblivion_tile_white", new Image()));
    mLpImages["oblivion_tile_white"]->Init("Image/Tiles/oblivion_tile_white.bmp", 25 * 2, 34 * 2, 1, 1, 1, true, RGB(0, 170, 255));

    mLpImages.insert(make_pair("bandit_tile", new Image()));
    mLpImages["bandit_tile"]->Init("Image/Tiles/bandit_tile.bmp", 25 * 2, 34 * 2, 1, 1, 1, true, RGB(0, 170, 255));
    mLpImages.insert(make_pair("bandit_tile_select", new Image()));
    mLpImages["bandit_tile_select"]->Init("Image/Tiles/bandit_tile_select.bmp", 25 * 2, 34 * 2, 1, 1, 1, true, RGB(0, 170, 255));
    mLpImages.insert(make_pair("bandit_tile_white", new Image()));
    mLpImages["bandit_tile_white"]->Init("Image/Tiles/bandit_tile_white.bmp", 25 * 2, 34 * 2, 1, 1, 1, true, RGB(0, 170, 255));

    mLpImages.insert(make_pair("battlefield_tile", new Image()));
    mLpImages["battlefield_tile"]->Init("Image/Tiles/bandit_tile.bmp", 25 * 2, 34 * 2, 1, 1, 1, true, RGB(0, 170, 255));
    mLpImages.insert(make_pair("battlefield_tile_select", new Image()));
    mLpImages["battlefield_tile_select"]->Init("Image/Tiles/battlefield_tile_select.bmp", 25 * 2, 34 * 2, 1, 1, 1, true, RGB(0, 170, 255));
    mLpImages.insert(make_pair("battlefield_tile_white", new Image()));
    mLpImages["battlefield_tile_white"]->Init("Image/Tiles/battlefield_tile_white.bmp", 25 * 2, 34 * 2, 1, 1, 1, true, RGB(0, 170, 255));

    mLpImages.insert(make_pair("meadom_tile", new Image()));
    mLpImages["meadom_tile"]->Init("Image/Tiles/meadom_tile.bmp", 25 * 2, 34 * 2, 1, 1, 1, true, RGB(0, 170, 255));
    mLpImages.insert(make_pair("meadom_tile_select", new Image()));
    mLpImages["meadom_tile_select"]->Init("Image/Tiles/meadom_tile_select.bmp", 25 * 2, 34 * 2, 1, 1, 1, true, RGB(0, 170, 255));
    mLpImages.insert(make_pair("meadom_tile_white", new Image()));
    mLpImages["meadom_tile_white"]->Init("Image/Tiles/meadom_tile_white.bmp", 25 * 2, 34 * 2, 1, 1, 1, true, RGB(0, 170, 255));

    mLpImages.insert(make_pair("bloommeadom_tile", new Image()));
    mLpImages["bloommeadom_tile"]->Init("Image/Tiles/bandit_tile.bmp", 25 * 2, 34 * 2, 1, 1, 1, true, RGB(0, 170, 255));
    mLpImages.insert(make_pair("bloommeadom_tile_select", new Image()));
    mLpImages["bloommeadom_tile_select"]->Init("Image/Tiles/bloommeadom_tile_select.bmp", 25 * 2, 34 * 2, 1, 1, 1, true, RGB(0, 170, 255));
    mLpImages.insert(make_pair("bloommeadom_tile_white", new Image()));
    mLpImages["bloommeadom_tile_white"]->Init("Image/Tiles/bloommeadom_tile_white.bmp", 25 * 2, 34 * 2, 1, 1, 1, true, RGB(0, 170, 255));

    mLpImages.insert(make_pair("campsite_tile", new Image()));
    mLpImages["campsite_tile"]->Init("Image/Tiles/campsite_tile.bmp", 25 * 2, 34 * 2, 1, 1, 1, true, RGB(0, 170, 255));
    mLpImages.insert(make_pair("campsite_tile_select", new Image()));
    mLpImages["campsite_tile_select"]->Init("Image/Tiles/campsite_tile_select.bmp", 25 * 2, 34 * 2, 1, 1, 1, true, RGB(0, 170, 255));
    mLpImages.insert(make_pair("campsite_tile_white", new Image()));
    mLpImages["campsite_tile_white"]->Init("Image/Tiles/campsite_tile_white.bmp", 25 * 2, 34 * 2, 1, 1, 1, true, RGB(0, 170, 255));

    mLpImages.insert(make_pair("crystal_tile", new Image()));
    mLpImages["crystal_tile"]->Init("Image/Tiles/crystal_tile.bmp", 25 * 2, 34 * 2, 1, 1, 1, true, RGB(0, 170, 255));
    mLpImages.insert(make_pair("crystal_tile_select", new Image()));
    mLpImages["crystal_tile_select"]->Init("Image/Tiles/crystal_tile_select.bmp", 25 * 2, 34 * 2, 1, 1, 1, true, RGB(0, 170, 255));
    mLpImages.insert(make_pair("crystal_tile_white", new Image()));
    mLpImages["crystal_tile_white"]->Init("Image/Tiles/crystal_tile_white.bmp", 25 * 2, 34 * 2, 1, 1, 1, true, RGB(0, 170, 255));

    mLpImages.insert(make_pair("goblin_tile", new Image()));
    mLpImages["goblin_tile"]->Init("Image/Tiles/goblin_tile.bmp", 25 * 2, 34 * 2, 1, 1, 1, true, RGB(0, 170, 255));
    mLpImages.insert(make_pair("goblin_tile_select", new Image()));
    mLpImages["goblin_tile_select"]->Init("Image/Tiles/goblin_tile_select.bmp", 25 * 2, 34 * 2, 1, 1, 1, true, RGB(0, 170, 255));
    mLpImages.insert(make_pair("goblin_tile_white", new Image()));
    mLpImages["goblin_tile_white"]->Init("Image/Tiles/goblin_tile_white.bmp", 25 * 2, 34 * 2, 1, 1, 1, true, RGB(0, 170, 255));

    mLpImages.insert(make_pair("empty_tresury_tile", new Image()));
    mLpImages["empty_tresury_tile"]->Init("Image/Tiles/empty_tresury_tile.bmp", 25 * 2 * 9, 34 * 2, 9, 1, 9, true, RGB(0, 170, 255));
    mLpImages.insert(make_pair("empty_tresury_tile_select", new Image()));
    mLpImages["empty_tresury_tile_select"]->Init("Image/Tiles/empty_tresury_tile_select.bmp", 25 * 2, 34 * 2, 1, 1, 1, true, RGB(0, 170, 255));
    mLpImages.insert(make_pair("empty_tresury_tile_white", new Image()));
    mLpImages["empty_tresury_tile_white"]->Init("Image/Tiles/empty_tresury_tile_white.bmp", 25 * 2, 34 * 2, 1, 1, 1, true, RGB(0, 170, 255));

    mLpImages.insert(make_pair("tresury_tile", new Image()));
    mLpImages["tresury_tile"]->Init("Image/Tiles/empty_tresury_tile.bmp", 25 * 2, 34 * 2, 1, 1, 1, true, RGB(0, 170, 255));
    mLpImages.insert(make_pair("tresury_tile_select", new Image()));
    mLpImages["tresury_tile_select"]->Init("Image/Tiles/tresury_tile_select.bmp", 25 * 2, 34 * 2, 1, 1, 1, true, RGB(0, 170, 255));
    mLpImages.insert(make_pair("tresury_tile_white", new Image()));
    mLpImages["tresury_tile_white"]->Init("Image/Tiles/tresury_tile_white.bmp", 25 * 2, 34 * 2, 1, 1, 1, true, RGB(0, 170, 255));

    mLpImages.insert(make_pair("spider_tile", new Image()));
    mLpImages["spider_tile"]->Init("Image/Tiles/spider_tile.bmp", 25 * 2, 34 * 2, 1, 1, 1, true, RGB(0, 170, 255));
    mLpImages.insert(make_pair("spider_tile_select", new Image()));
    mLpImages["spider_tile_select"]->Init("Image/Tiles/spider_tile_select.bmp", 25 * 2, 34 * 2, 1, 1, 1, true, RGB(0, 170, 255));
    mLpImages.insert(make_pair("spider_tile_white", new Image()));
    mLpImages["spider_tile_white"]->Init("Image/Tiles/spider_tile_white.bmp", 25 * 2, 34 * 2, 1, 1, 1, true, RGB(0, 170, 255));

    mLpImages.insert(make_pair("rock_tile", new Image()));
    mLpImages["rock_tile"]->Init("Image/Tiles/rock_tile.bmp", 25 * 2, 34 * 2, 1, 1, 1, true, RGB(0, 170, 255));
    mLpImages.insert(make_pair("rock_tile_select", new Image()));
    mLpImages["rock_tile_select"]->Init("Image/Tiles/rock_tile_select.bmp", 25 * 2, 34 * 2, 1, 1, 1, true, RGB(0, 170, 255));
    mLpImages.insert(make_pair("rock_tile_white", new Image()));
    mLpImages["rock_tile_white"]->Init("Image/Tiles/rock_tile_white.bmp", 25 * 2, 34 * 2, 1, 1, 1, true, RGB(0, 170, 255));

    mLpImages.insert(make_pair("mountain_tile", new Image()));
    mLpImages["mountain_tile"]->Init("Image/Tiles/mountain_tile.bmp", 25 * 2, 34 * 2, 1, 1, 1, true, RGB(0, 170, 255));
    mLpImages.insert(make_pair("mountain_tile_select", new Image()));
    mLpImages["mountain_tile_select"]->Init("Image/Tiles/mountain_tile_select.bmp", 25 * 2, 34 * 2, 1, 1, 1, true, RGB(0, 170, 255));
    mLpImages.insert(make_pair("mountain_tile_white", new Image()));
    mLpImages["mountain_tile_white"]->Init("Image/Tiles/mountain_tile_white.bmp", 25 * 2, 34 * 2, 1, 1, 1, true, RGB(0, 170, 255));

    mLpImages.insert(make_pair("road_tile", new Image()));
    mLpImages["road_tile"]->Init("Image/Tiles/road_tile.bmp", 25 * 2, 34 * 2 * 7, 1, 7, 7, true, RGB(0, 170, 255));
    mLpImages.insert(make_pair("road_tile_select", new Image()));
    mLpImages["road_tile_select"]->Init("Image/Tiles/road_tile.bmp", 25 * 2, 34 * 2 * 7, 1, 7, 7, true, RGB(0, 170, 255));
    mLpImages.insert(make_pair("road_tile_white", new Image()));
    mLpImages["road_tile_white"]->Init("Image/Tiles/road_tile_white.bmp", 25 * 2, 34 * 2 * 7, 1, 7, 7, true, RGB(0, 170, 255));

    mLpImages.insert(make_pair("mountain_peak_tile", new Image()));
    mLpImages["mountain_peak_tile"]->Init("Image/Tiles/mountain_peak_tile.bmp", 25 * 2, 34 * 2 * 9, 1, 9, 9, true, RGB(0, 170, 255));
    mLpImages.insert(make_pair("mountain_peak_tile_select", new Image()));
    mLpImages["mountain_peak_tile_select"]->Init("Image/Tiles/mountain_peak_tile_select.bmp", 25 * 2, 34 * 2 * 9, 1, 9, 9, true, RGB(0, 170, 255));
    mLpImages.insert(make_pair("mountain_peak_tile_white", new Image()));
    mLpImages["mountain_peak_tile_white"]->Init("Image/Tiles/mountain_peak_tile_white.bmp", 25 * 2, 34 * 2 * 9, 1, 9, 9, true, RGB(0, 170, 255));

    mLpImages.insert(make_pair("bloodroad_tile", new Image()));
    mLpImages["bloodroad_tile"]->Init("Image/Tiles/bloodroad_tile.bmp", 25 * 2, 34 * 2 * 7, 1, 7, 7, true, RGB(0, 170, 255));
    mLpImages.insert(make_pair("bloodroad_tile_select", new Image()));
    mLpImages["bloodroad_tile_select"]->Init("Image/Tiles/bloodroad_tile.bmp", 25 * 2, 34 * 2 * 7, 1, 7, 7, true, RGB(0, 170, 255));
    mLpImages.insert(make_pair("bloodroad_tile_white", new Image()));
    mLpImages["bloodroad_tile_white"]->Init("Image/Tiles/bloodroad_tile_white.bmp", 25 * 2, 34 * 2 * 7, 1, 7, 7, true, RGB(0, 170, 255));

    mLpImages.insert(make_pair("cemetery_tile", new Image()));
    mLpImages["cemetery_tile"]->Init("Image/Tiles/cemetery_tile.bmp", 25 * 2, 34 * 2 * 7, 1, 7, 7, true, RGB(0, 170, 255));
    mLpImages.insert(make_pair("cemetery_tile_select", new Image()));
    mLpImages["cemetery_tile_select"]->Init("Image/Tiles/cemetery_tile_select.bmp", 25 * 2, 34 * 2 * 7, 1, 7, 7, true, RGB(0, 170, 255));
    mLpImages.insert(make_pair("cemetery_tile_white", new Image()));
    mLpImages["cemetery_tile_white"]->Init("Image/Tiles/cemetery_tile_white.bmp", 25 * 2, 34 * 2 * 7, 1, 7, 7, true, RGB(0, 170, 255));

    mLpImages.insert(make_pair("grove_tile", new Image()));
    mLpImages["grove_tile"]->Init("Image/Tiles/grove_tile.bmp", 25 * 2, 34 * 2 * 7, 1, 7, 7, true, RGB(0, 170, 255));
    mLpImages.insert(make_pair("grove_tile_select", new Image()));
    mLpImages["grove_tile_select"]->Init("Image/Tiles/grove_tile_select.bmp", 25 * 2, 34 * 2 * 7, 1, 7, 7, true, RGB(0, 170, 255));
    mLpImages.insert(make_pair("grove_tile_white", new Image()));
    mLpImages["grove_tile_white"]->Init("Image/Tiles/grove_tile_white.bmp", 25 * 2, 34 * 2 * 7, 1, 7, 7, true, RGB(0, 170, 255));

    mLpImages.insert(make_pair("bloodgrove_tile", new Image()));
    mLpImages["bloodgrove_tile"]->Init("Image/Tiles/bloodgrove_tile.bmp", 25 * 2 * 2, 34 * 2, 2, 1, 2, true, RGB(0, 170, 255));
    mLpImages.insert(make_pair("bloodgrove_tile_select", new Image()));
    mLpImages["bloodgrove_tile_select"]->Init("Image/Tiles/bloodgrove_tile_select.bmp", 25 * 2 * 2, 34 * 2, 2, 1, 2, true, RGB(0, 170, 255));
    mLpImages.insert(make_pair("bloodgrove_tile_white", new Image()));
    mLpImages["bloodgrove_tile_white"]->Init("Image/Tiles/bloodgrove_tile_white.bmp", 25 * 2 * 2, 34 * 2, 2, 1, 2, true, RGB(0, 170, 255));

    mLpImages.insert(make_pair("swamp_tile", new Image()));
    mLpImages["swamp_tile"]->Init("Image/Tiles/swamp_tile.bmp", 25 * 2, 34 * 2 * 7, 1, 7, 7, true, RGB(0, 170, 255));
    mLpImages.insert(make_pair("swamp_tile_select", new Image()));
    mLpImages["swamp_tile_select"]->Init("Image/Tiles/swamp_tile_select.bmp", 25 * 2, 34 * 2 * 7, 1, 7, 7, true, RGB(0, 170, 255));
    mLpImages.insert(make_pair("swamp_tile_white", new Image()));
    mLpImages["swamp_tile_white"]->Init("Image/Tiles/swamp_tile_white.bmp", 25 * 2, 34 * 2 * 7, 1, 7, 7, true, RGB(0, 170, 255));

    mLpImages.insert(make_pair("village_tile", new Image()));
    mLpImages["village_tile"]->Init("Image/Tiles/village_tile.bmp", 25 * 2, 34 * 2 * 7, 1, 7, 7, true, RGB(0, 170, 255));
    mLpImages.insert(make_pair("village_tile_select", new Image()));
    mLpImages["village_tile_select"]->Init("Image/Tiles/village_tile_select.bmp", 25 * 2, 34 * 2 * 7, 1, 7, 7, true, RGB(0, 170, 255));
    mLpImages.insert(make_pair("village_tile_white", new Image()));
    mLpImages["village_tile_white"]->Init("Image/Tiles/village_tile_white.bmp", 25 * 2, 34 * 2 * 7, 1, 7, 7, true, RGB(0, 170, 255));

    mLpImages.insert(make_pair("boss_tile", new Image()));
    mLpImages["boss_tile"]->Init("Image/Tiles/boss_tile.bmp", 25 * 2 * 2, 34 * 2, 2, 1, 2, true, RGB(0, 170, 255));
    mLpImages.insert(make_pair("boss_tile_select", new Image()));
    mLpImages["boss_tile_select"]->Init("Image/Tiles/boss_tile_select.bmp", 25 * 2, 34 * 2, 1, 1, 1, true, RGB(0, 170, 255));
    mLpImages.insert(make_pair("boss_tile_white", new Image()));
    mLpImages["boss_tile_white"]->Init("Image/Tiles/boss_tile_white.bmp", 25 * 2, 34 * 2, 1, 1, 1, true, RGB(0, 170, 255));

    mLpImages.insert(make_pair("boss_side_tile", new Image()));
    mLpImages["boss_side_tile"]->Init("Image/Tiles/boss_side_tile.bmp", 25 * 2, 34 * 2 * 9, 1, 9, 9, true, RGB(0, 170, 255));
    mLpImages.insert(make_pair("boss_side_tile_select", new Image()));
    mLpImages["boss_side_tile_select"]->Init("Image/Tiles/boss_side_tile_select.bmp", 25 * 2, 34 * 2 * 9, 1, 9, 9, true, RGB(0, 170, 255));
    mLpImages.insert(make_pair("boss_side_tile_white", new Image()));
    mLpImages["boss_side_tile_white"]->Init("Image/Tiles/boss_side_tile_white.bmp", 25 * 2, 34 * 2 * 9, 1, 9, 9, true, RGB(0, 170, 255));
    // Ÿ�� ======================================================================

    return S_OK;
}

void ImageManager::Release()
{
    for (auto it = mLpImages.begin(); it != mLpImages.end(); ++it)
    {
        if ((it->second))
        {
            (it->second)->Release();
            delete (it->second);
            (it->second) = nullptr;
        }
    }
    mLpImages.clear();
}

Image* ImageManager::AddImage(string key, int width, int height, bool isTrans, COLORREF transColor)
{
    auto it = mLpImages.find(key);
    if (it != mLpImages.end()) return it->second;

    mLpImages.insert(make_pair(key, new Image()));
    mLpImages[key]->Init(width, height, isTrans, transColor);

    return mLpImages[key];
}

Image* ImageManager::FindImage(string key)
{
    if (mLpImages.find(key) == mLpImages.end())
    {
        return nullptr;
    }

    return mLpImages[key];
}
