#define TEAM_NIL 0
#define TEAM_SPE 1
#define TEAM_RED 2
#define TEAM_BLU 3
#define TEAM_SPC 4

#define STATE_INITIAL  0
#define STATE_ROLLING  1
#define STATE_FIGHTING 2
#define STATE_PICKING  3
#define STATE_SETUP    4
#define STATE_WARMUP   5
#define STATE_LIVE     6
#define STATE_POST     7

#define CLASS_UNKNOWN  0
#define CLASS_SCOUT	   1
#define CLASS_SNIPER   2
#define CLASS_SOLDIER  3
#define CLASS_DEMOMAN  4
#define CLASS_MEDIC	   5
#define CLASS_HEAVY	   6
#define CLASS_PYRO	   7
#define CLASS_SPY	   8
#define CLASS_ENGI	   9

// Core Includes
#include <sourcemod>
#include <sdkhooks>
#include <morecolors>
#include <tf2>
#include <tf2_stocks>
#include <match>

#include <cvars>

// Library Includes
#include <libs/hud>
#include <libs/minhud>
#include <libs/colors>

// Module Includes
#include <core/roll>
#include <core/fight>
#include <core/pick>

// Module Includes
#include <core/modules/team_limit>
#include <core/modules/warmup_restart>
#include <core/modules/config>
#include <core/modules/chat_control>
#include <core/modules/mark_plusone>
#include <core/modules/change_captain>
#include <core/modules/change_rollstatus>
#include <core/modules/player_readyup>

// Mode Module Includes
#include <roll/spec-roll>
#include <roll/captain>

// Fight Module Includes
#include <fight/melee>

// Pick Module Includes
#include <pick/chat>
#include <pick/menu>

// Utils Includes
#include <player>
#include <utils>

#undef REQUIRE_PLUGIN
#include <updater>