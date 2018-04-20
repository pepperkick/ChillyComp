#define PLUGIN_VERSION  "4.0.0"
#define UPDATE_URL      ""
#define TAG             "CHILLY"
#define COLOR_TAG       "{matAmber}"
#define MAX_PLAYERS 24
#define DEBUG

//=========================================================================
//        P L U G I N S         V A R I A B L E S
//=========================================================================

Handle    g_hcDebuggingEnable             = INVALID_HANDLE;    //CVar to check if debugging logs is enabled
Handle    g_hcMapTime                     = INVALID_HANDLE;    //CVar to store the map time limit

Handle    g_hoHud                         = INVALID_HANDLE;    //Handle for HUD text display
Handle    g_hoAdminMenu                   = INVALID_HANDLE;    //Handle for Custom Admin Menu

Handle    g_htCheckPlayerCounter          = INVALID_HANDLE;    //Timer to check the player counter
Handle    g_htCheckCounter                = INVALID_HANDLE;    //Timer to check the counter for melee mode
Handle    g_htRollMessage                 = INVALID_HANDLE;    //Timer to display the "ROLL" message

//=========================================================================
//        P L A Y E R S         V A R I A B L E S
//=========================================================================

int       g_iPlayerCounter                = 0;        //Number of Players in the RED or BLU team

bool      g_bRollingSequence              = false;    //Has the Rolling Sequence started
bool      g_bPlayerPenalty[MAX_PLAYERS]   = false;    //Dose Player has roll Penalty

//=========================================================================s
//        R O L L I N G         V A R I A B L E S
//=========================================================================

int       g_iRollingSeconds               = 0;        //Number of seconds before roll starts
int       g_iMapTimeLimit                 = 0;        //Map Time Limit from mp_timelimit

bool      g_bRollingPick                  = false;    //Has the Rolling Pick started
bool      g_bCheckCounter                 = false;    //Has the timer to Check Counter started
bool      g_bAutoDisable                  = false;    //Has the rolling disabled itself

//========================================================================
//        T E A M P I C K     V A R I A B L E S
//========================================================================

Handle    g_hPlayerPicked                 = INVALID_HANDLE;

int       g_iPlayerCount                  = 0;
int       g_iBluTeamLeader                = -1;        //Leader of BLU Team
int       g_iRedTeamLeader                = -1;        //Leader of RED Team

//=========================================================================
//        P L U S O N E L I S T        V A R I A B L E S
//=========================================================================

Handle    g_hcPlusOnePlayerID             = INVALID_HANDLE;

int       g_iPlusOneCount                 = 0;
int       g_iPlusOneAllowed               = 0;
int       g_iPlusOneBlocked               = 0;

bool      g_bPlusOneAllowed[MAX_PLAYERS]  = false;            //Is player allowed to pick

//=========================================================================
//        T E A M L I M I T        V A R I A B L E S
//=========================================================================

Handle    g_hcTeamLimitSize               = INVALID_HANDLE;    //CVar for team limit size varible from TeamLimit Plugin

int       g_iTeamLimitSize                = 0;                //Team Limit Size from TeamLimit Plugin

//=========================================================================
//        P L U G I N    E V E N T    F U N C T I O N S     S T A R T

//============================================
//    PluginInfo
//        Info about the plugin is stored here
//============================================

public Plugin:myinfo = {
    name = "ChillyComp",
    author = "PepperKick",
    description = "A plugin to manage competitive matches",
    version = PLUGIN_VERSION,
    url = ""
}

#include <headers>

//============================================
//    OnPluginStart
//        Executed when the plugin starts
//============================================

public OnPluginStart() {
    LoadTranslations("chillycomp.phrases");
    DebugLog("Loaded Translation Files");

    //Set CVars
    CreateCvars();

    if (LibraryExists("updater"))
        Updater_AddPlugin(UPDATE_URL)

    //Initialize Plus One Block List
    g_hPlayerPicked = CreateArray(MAX_PLAYERS);

    //Attach HUD Handle
    g_hoHud = HudInit(127, 255, 127);

    // Add Extra Colors
    AddColors();

    //Set CommandListeners
    AddCommandListener(Command_JoinTeam, "jointeam");    //Attach Listener to "jointeam" command
    AddCommandListener(Command_JoinSpec, "spectate");    //Attach Listener to "spectate" command

    // Set Console Commands
    RegConsoleCmd("pick", Command_PickPlayer, "Pick player through chat during picking process");
    RegConsoleCmd("p",    Command_PickPlayer, "Pick player through chat during picking process");
    RegConsoleCmd("list", Command_PickList,   "Shows the list of rolled and plus one players during picking process");

    // Set Admin Commands
    RegAdminCmd("startroll",        Command_StartRolling,       ADMFLAG_BAN, "Start rolling process");
    RegAdminCmd("mark",             Command_MarkPlusOne,        ADMFLAG_BAN, "Mark a player as plus one");
    RegAdminCmd("restartpicking",   Command_RestartPicking,     ADMFLAG_BAN, "Restart the picking stage of rolling");
    RegAdminCmd("rspicking",        Command_RestartPicking,     ADMFLAG_BAN, "Restart the picking stage of rolling");
    RegAdminCmd("changecaptainred", Command_SwapCaptainRed,     ADMFLAG_BAN, "Change RED team captain");
    RegAdminCmd("changecapred",     Command_SwapCaptainRed,     ADMFLAG_BAN, "Change RED team captain");
    RegAdminCmd("chgcapred",        Command_SwapCaptainRed,     ADMFLAG_BAN, "Change RED team captain");
    RegAdminCmd("ccr",              Command_SwapCaptainRed,     ADMFLAG_BAN, "Change RED team captain");
    RegAdminCmd("changecaptainblu", Command_SwapCaptainBlu,     ADMFLAG_BAN, "Change BLU team captain");
    RegAdminCmd("changecapblu",     Command_SwapCaptainBlu,     ADMFLAG_BAN, "Change BLU team captain");
    RegAdminCmd("chgcapblu",        Command_SwapCaptainBlu,     ADMFLAG_BAN, "Change BLU team captain");
    RegAdminCmd("ccb",              Command_SwapCaptainBlu,     ADMFLAG_BAN, "Change BLU team captain");
    RegAdminCmd("changeroll",       Command_ChangeRollStatus,   ADMFLAG_BAN, "Change the roll status for a player");

    //Match Function
    Match_OnPluginStart();

    //Rest Rolling
    RollingReset();

    // Attach cvar change hooks
    HookConVarChange(g_hcRollMode, Handle_RollModeChange);
	HookConVarChange(g_hcGameStatus, Hande_GameStatusChanged);

    HookEvent("teamplay_round_start", Event_RoundStart);

    DebugLog("Loaded ChillyRoll plugin, Version %s", PLUGIN_VERSION);
}

//        P L U G I N    E V E N T    F U N C T I O N S     E N D
//=========================================================================

//=========================================================================
//        E V E N T     F U N C T I O N S     S T A R T

//============================================
//    OnClientDisconnect
//        Executed when a client disconnets
//============================================

public OnClientDisconnect(client) {
    if(GetStatus() > STATE_INITIAL && GetStatus() < STATE_SETUP && (client == g_iBluTeamLeader || client == g_iRedTeamLeader)) {
        //If a team leader leaves while picking is going on

        new String:hudmsg[128];
        Format(hudmsg, sizeof(hudmsg), "%T", "Rolling-Canceled-HUD", LANG_SERVER);

        HudSetColor(255, 152, 0);
        HudSetText(hudmsg);
        CPrintToChatAll("%s[%s] %t", COLOR_TAG, TAG, "Rolling-Canceled-MSG-LeaderLeft");

        RollingReset();                        //Reset Rolling

        return;
    } else if(GetStatus() > STATE_INITIAL && GetStatus() < STATE_SETUP && RollingCheckPlayer(client) && CountPlayersInAnyTeam() < (GetConVarInt(g_hcTeamSize) * 2)) {
        //If a player leaves while either rolling sequence or team picking is going on and there are not enough players

        new String:hudmsg[128];
        Format(hudmsg, sizeof(hudmsg), "%T", "Rolling-Canceled-HUD", LANG_SERVER);

        HudSetColor(255, 152, 0);
        HudSetText(hudmsg);
        CPrintToChatAll("%s[%s] %t", COLOR_TAG, TAG, "Rolling-Canceled-MSG-PlayerLeft");

        RollingReset();                        //Reset Rolling

        return;
    }
}

//============================================
//    OnAllPluginsLoaded
//        Executed when all plugins loaded
//============================================

public OnAllPluginsLoaded() {
    DebugLog("Plugins Loaded");

    if (FindConVar("sm_teamlimit_version") != INVALID_HANDLE) {
        g_hcTeamLimitSize = FindConVar("sm_teamlimit");
    }
}

//============================================
//    OnMapStart
//        Executed when a map starts
//============================================

public OnMapStart() {
    Match_OnMapStart();

    SetStatus(STATE_INITIAL);

    DebugLog("Map Started");
}

//============================================
//    OnMapEnd
//        Executed when a map ends
//============================================

public void OnMapEnd() {
    DebugLog("Map Ended");

    Match_OnMapEnd();

    RollingReset();
}

//============================================
//    StartMatch
//        Executed when a match starts
//============================================

StartMatch() {
    DebugLog("Match Started");

    // SetPlusOneList();
}

//============================================
//    ResetMatch
//        Executed when a match resets
//============================================

ResetMatch() {
    DebugLog("Match Reset");

    RollingReset();
}

//============================================
//    EndMatch
//        Executed when a match ends
//     Parameters
//        bool     Did the match end midgame
//============================================

EndMatch(bool:endedMidgame) {
    DebugLog("Match Ended");

	SetStatus(STATE_POST);
}

public Action:Command_JoinTeam(client, const String:command[], argc) {
    char team[128];
    GetCmdArg(1, team, sizeof(team));

    if (GetStatus() > STATE_INITIAL && GetStatus() < STATE_SETUP) {
        return RollingConditions(client, team);
    } else if (GetStatus() > STATE_LIVE && GetStatus() < STATE_POST) {
        return TeamLimitConditions(client, team);
    }

    return Plugin_Continue;
}

public void Event_RoundStart(Event event, const char[] name, bool dontBroadcast) {
    if (!g_bWarmupRestartChecked) {
         CheckWarmupRestart();
    }

    if (g_bWarmupRestartRequired && g_bWarmupRestartCounter > 0) {
        DoWarmupRestart();
    } else {
        ResetWarmupResatrt();

        SetStatus(STATE_LIVE);
    }
}

public void Hande_GameStatusChanged(ConVar convar, const char[] oldValue, const char[] newValue) {
    int value = StringToInt(newValue);

    CheckStatusChange(value);
}

//        E V E N T     F U N C T I O N S     E N D
//=========================================================================

//=========================================================================
//        R O L L I N G     F U N C T I O N S     S T A R T

//============================================
//    SetStatus
//        Sets the status of the game
//     Parameters
//        int     Status of the game
//============================================

public void SetStatus(int status) {
	DebugLog("Setting status to %s", status);

	SetConVarInt(g_hcGameStatus, status, false, false);
}

//============================================
//    GetStatus
//        Returns the status of the game
//     Returns int
//============================================

public int GetStatus() {
    return GetConVarInt(g_hcGameStatus);
}

//        R O L L I N G     F U N C T I O N S     E N D
//=========================================================================

//=========================================================================
//         U P D A T E R   F U N C T I O N S    S T A R T

public OnLibraryAdded(const String:name[]) {
    if (StrEqual(name, "updater"))
        Updater_AddPlugin(UPDATE_URL)
}

//       U P D A T E R   F U N C T I O N S     E N D
//=========================================================================

public DebugLog(const char[] myString, any ...) {
    #if defined DEBUG
        int len = strlen(myString) + 255;
        char[] myFormattedString = new char[len];
        VFormat(myFormattedString, len, myString, 2);

        PrintToServer("[COMP] %s", myFormattedString);
    #endif
}