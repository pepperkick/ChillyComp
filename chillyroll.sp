#define PLUGIN_VERSION  "3.0.2"
#define TEAM_NIL 0
#define TEAM_RED 2
#define TEAM_BLU 3
#define TEAM_SPC 4
#define MAX_PLAYERS 24
#define DEBUG

Handle g_hPlayerPicked 					= INVALID_HANDLE;

//=========================================================================
//		P L U G I N S 		V A R I A B L E S
//=========================================================================

Handle	g_hcEnabled						= INVALID_HANDLE;	//CVar to check if the plugin is enabled 
Handle	g_hcTeamSize					= INVALID_HANDLE;	//CVar to store the value of team size
Handle 	g_hcOnCompleteConfig			= INVALID_HANDLE;	//CVar to store the name of config to run on complete
Handle	g_hcPlayerPenaltyEnable			= INVALID_HANDLE;	//CVar to check if player penalty is enabled
Handle	g_hcPlayerPenaltyTime			= INVALID_HANDLE;	//CVar to store number of seconds for penalty
Handle 	g_hcDebuggingEnable				= INVALID_HANDLE;	//CVar to check if debugging logs is enabled
Handle	g_hcMapTime						= INVALID_HANDLE;	//CVar to store the map time limit
Handle	g_hcRollMode					= INVALID_HANDLE;	//CVar to store the rolling mode

Handle	g_hoHud							= INVALID_HANDLE;	//Handle for HUD text display
Handle	g_hoAdminMenu					= INVALID_HANDLE;	//Handle for Custom Admin Menu
	
Handle 	g_htCheckPlayerCounter			= INVALID_HANDLE;	//Timer to check the player counter
Handle 	g_htCheckCounter				= INVALID_HANDLE;	//Timer to check the counter for melee mode
Handle	g_htRollMessage					= INVALID_HANDLE;	//Timer to display the "ROLL" message

//=========================================================================
//		P L A Y E R S 		V A R I A B L E S
//=========================================================================

int 	g_iPlayerCounter				= 0;		//Number of Players in the RED or BLU team
int 	g_bRollingSequence 				= 0;		//Has the Rolling Sequence started

bool 	g_bPlayerPenalty[MAX_PLAYERS] 	= false;	//Dose Player has roll Penalty

//=========================================================================
//		R O L L I N G 		V A R I A B L E S
//=========================================================================

int 	g_iRollingSeconds				= 0;		//Number of seconds before roll starts
int		g_iMapTimeLimit					= 0;		//Map Time Limit from mp_timelimit
bool 	g_bRollingPick					= false;	//Has the Rolling Pick started
bool 	g_bCheckCounter					= false;	//Has the timer to Check Counter started
bool 	g_bAutoDisable					= false;	//Has the rolling disabled itself

//========================================================================
//		T E A M P I C K 	V A R I A B L E S
//========================================================================

int 	g_iPlayerCount					= 0;	
int 	g_iBluTeamLeader				= -1;		//Leader of BLU Team
int 	g_iRedTeamLeader				= -1;		//Leader of RED Team

//=========================================================================
//		P L U S O N E L I S T		V A R I A B L E S
//=========================================================================

Handle	g_hcPlusOnePlayerID				= INVALID_HANDLE;

int		g_iPlusOneCount					= 0;
int		g_iPlusOneAllowed				= 0;
int		g_iPlusOneBlocked				= 0;

bool 	g_bPlusOneAllowed[MAX_PLAYERS] 	= false;			//Is player allowed to pick

//=========================================================================
//		T E A M L I M I T		V A R I A B L E S
//=========================================================================

Handle	g_hcTeamLimitSize				= INVALID_HANDLE;	//CVar for team limit size varible from TeamLimit Plugin

int		g_iTeamLimitSize				= 0;				//Team Limit Size from TeamLimit Plugin

//=========================================================================
//		P L U G I N	E V E N T	F U N C T I O N S 	S T A R T

//============================================
//	PluginInfo 
//		Info about the plugin is stored here
//============================================

public Plugin:myinfo = {
	name = "ChillyRoll",
	author = "PepperKick",
	description = "",
	version = PLUGIN_VERSION,
	url = ""
}

#include <rolling/headers>

//============================================
//	OnPluginStart
//		Executed when the plugin starts
//============================================

public OnPluginStart() {
	LoadTranslations("chillyroll.phrases");
	DebugLog("Loaded Translation Files");
	
	//Set CVars
	
	//Plugin Version ConVar
    CreateConVar(
		"rolling_version", 
		PLUGIN_VERSION, 
		"ChillyRoll Plugin Version", 
		FCVAR_SPONLY | FCVAR_REPLICATED | FCVAR_NOTIFY
	);	
		
	//Initialize Plus One Block List
	g_hcPlusOnePlayerID = CreateArray(32);
	g_hPlayerPicked = CreateArray();
	
	//Attach HUD Handle
	g_hoHud = HudInit(127, 255, 127);
	
	//Set CommandListeners
	AddCommandListener(Command_JoinTeam, "jointeam");	//Attach Listener to "jointeam" command
	AddCommandListener(Command_JoinSpec, "spectate");	//Attach Listener to "spectate" command
	
	//Match Function
	Match_OnPluginStart();
	
	//Rest Rolling
	RollingReset();	
	
	HookConVarChange(g_hcRollMode, Handle_RollModeChange);
	
	DebugLog("Loaded ChillyRoll plugin, Version %s", PLUGIN_VERSION);
}

//		P L U G I N	E V E N T	F U N C T I O N S 	E N D
//=========================================================================

//=========================================================================
//		E V E N T				F U N C T I O N S 	S T A R T

//============================================
//	OnClientDisconnect
//		Executed when a client disconnets
//============================================

/*
public OnClientDisconnect(client) {			
	if(g_bRollingPick && (client == g_iBluTeamLeader || client == g_iRedTeamLeader)) {
		//If a team leader leaves while picking is going on
		SetHudText("A team leader has left the game during rolling, Rolling Canceled");
		RollingReset();						//Reset Rolling
	} else if((g_bRollingSequence || g_bRollingPick) && g_hPlayerCounted[client] && g_iPlayerCounter < (GetConVarInt(g_hcTeamSize) * 2)) {
		//If a player leaves while either rolling sequence or team picking is going on and there are not enough players
		SetHudText("A player has left the game during rolling, Rolling Canceled");
		RollingReset();						//Reset Rolling
		
		return;
	}
	
	g_hPlayerCounted[client] = false;		//Set false for player counted
	
	if(g_iPickingTeam = TEAM_BLU) {
		RollingMenu(g_iRedTeamLeader);
	} else {	
		RollingMenu(g_iBluTeamLeader);
	}
}
*/

//============================================
//	OnAllPluginsLoaded
//		Executed when all plugins loaded
//============================================

public OnAllPluginsLoaded() {	
	DebugLog("Plugins Loaded");

    if (FindConVar("sm_teamlimit_version") != INVALID_HANDLE) {
		g_hcTeamLimitSize = FindConVar("sm_teamlimit");
    }
}  

//============================================
//	OnMapStart
//		Executed when a map starts
//============================================

public OnMapStart() {		
	Match_OnMapStart();	
	
	DebugLog("Map Started");
		
	if(g_hcTeamLimitSize != INVALID_HANDLE && RollAutoEnable()) {
		g_iTeamLimitSize = GetConVarInt(g_hcTeamLimitSize);
		
		DebugLog("Found team limit size: %d", g_iTeamLimitSize);
			
		SetConVarInt(g_hcTeamLimitSize, 12, false, false);
	}
}

//============================================
//	OnRollComplete
//		Executed when roll completed
//============================================

public void	OnRollComplete() {
	if(FindConVar("sm_teamlimit_version") != INVALID_HANDLE) {
		DebugLog("Reset team limit size", g_iTeamLimitSize);
			
		ClearArray(g_hcPlusOnePlayerID);
		g_hcPlusOnePlayerID = CreateArray(32);
			
		SetConVarInt(g_hcTeamLimitSize, g_iTeamLimitSize, false, false);		
	} 
}

//============================================
//	OnMapEnd
//		Executed when a map ends
//============================================

public void OnMapEnd() {
	DebugLog("Map Ended");
	
	Match_OnMapEnd();
}

//============================================
//	StartMatch
//		Executed when a match starts
//============================================

StartMatch() {
	DebugLog("Match Started");
	
	// SetPlusOneList();
}

//============================================
//	ResetMatch
//		Executed when a match resets
//============================================

ResetMatch() {
	DebugLog("Match Reset");
}

//============================================
//	EndMatch
//		Executed when a match ends
// 	Parameters
//		bool 	Did the match end midgame
//============================================

EndMatch(bool:endedMidgame) {
	DebugLog("Match Ended");
}

//		E V E N T				F U N C T I O N S 	E N D
//=========================================================================


//=========================================================================
//		R O L L I N G		  	F U N C T I O N S 	S T A R T	

public bool RollAutoEnable() {
	if(g_bAutoDisable) {
		PrintToServer("[ROLL] Auto Enabling Plugin");
			
		g_bAutoDisable = false;
		
		SetConVarInt(g_hcEnabled, 1, false, false);
	}
	
	if(GetConVarInt(g_hcEnabled) == 1) {
		RollingReset();
		
		return true;
	}
	
	return false;
}

//		R O L L I N G		  	F U N C T I O N S 	E N D
//=========================================================================

public DebugLog(const char[] myString, any ...) {
	#if defined DEBUG
		int len = strlen(myString) + 255;
		char[] myFormattedString = new char[len];
		VFormat(myFormattedString, len, myString, 2);
	 
		PrintToServer("[ROLL] %s", myFormattedString);
	#endif
}