/*
	Include file for Spec Roll functions
	In this mode all players roll to spec and last 2 are chosen as capt6ains.
		Callers
			SpecRollStart
			
		Timers
			Timer_PrintToHud
			Timer_CheckCounter
*/

int 	g_iSpecRollPlayerCount 			= 0;
int 	g_iSpecRollSeconds				= 0;
int 	g_iSpecRollRedLeader			= 0;
int 	g_iSpecRollBluLeader			= 0;

bool	g_iSpecRollPreRollDone			= false;
bool 	g_bSpecRollPlayer[MAX_PLAYERS] 	= false;

Handle 	g_bSpecRollCount	= INVALID_HANDLE;

//============================================
//	SpecRollStart
//		Starts the spec roll sequence
//============================================

public SpecRollStart(Handle:CountedPlayers) {
	DebugLog("Started Spec Roll Sequence");
	
	g_iSpecRollPreRollDone = false;
	
	g_bSpecRollCount = CloneArray(CountedPlayers);
	
	//Sets the timer to say the "ROLL!" message in HUD
	CreateTimer(GetRandomInt(4, 9) + 0.0, Timer_PrintToHud);
	
	//Sets HUD message roll ready
	HudSetText("Ready to roll!\nJoin Spec when 'ROLL' appears");
}

SpecRollReset() {	
	g_iSpecRollPlayerCount 	= 0;
	g_iSpecRollSeconds 		= 0;
	g_iSpecRollRedLeader 	= 0;
	g_iSpecRollBluLeader	= 0;
	g_iSpecRollPreRollDone 	= false;
	
	for(int i = 0; i < MAX_PLAYERS; i++) {
		g_bSpecRollPlayer[i] = false;	
		
		if(g_bSpecRollCount == INVALID_HANDLE) continue;
		
		SetArrayCell(g_bSpecRollCount, i, false);
	}
	
	DebugLog("Reset Spec Roll");
}

//============================================
//	SpecRollTeamConditions
//		Checks the team join conditions
// 	Parameters
//		int 	Client to check conditions for
//		string 	Which team is the client trying
//============================================	

public Action:SpecRollTeamConditions(client, team) {	
	DebugLog("[SPECROLL] Handling %d joining team %d", client, team);
		
	if (g_iSpecRollPreRollDone) {
		if (team == TEAM_SPC) {
			if (CountPlayersInTeam() > 2) {
				g_bSpecRollPlayer[client] = true;
	
				PrintToChat(client, "[ChillyRoll] You have successfully rolled");
				
				return Plugin_Continue;
			} else {				
				PrintToChat(client, "[ChillyRoll] Last 2 players cannot join spectators!");
					
				return Plugin_Handled;
			}
		} else if (team == TEAM_BLU && GetArrayCell(g_hPlayerPicked, client) == TEAM_RED) {
			PrintToChat(client, "[ChillyRoll] You are picked by RED team!");
			
			return Plugin_Handled;
		} else if (team == TEAM_RED && GetArrayCell(g_hPlayerPicked, client) == TEAM_BLU) {
			PrintToChat(client, "[ChillyRoll] You are picked by BLU team!");

			return Plugin_Handled;
		} else if ((team == TEAM_BLU || team == TEAM_RED) && GetArrayCell(g_hPlayerPicked, client) == 0) {
			PrintToChat(client, "[ChillyRoll] Wait until you are picked!");
							
			return Plugin_Handled;
		}
	} else {	
		if(GetArrayCell(g_bSpecRollCount, client)) {
			if (team == TEAM_SPC) {
				PrintToChat(client, "[ChillyRoll] You cannot join spec yet!");
				
				return Plugin_Handled;
			}
		}
	}
	
	return Plugin_Continue;
}

public Action:Timer_PrintToHud(Handle timer) {
	g_iSpecRollPreRollDone = true;
	CreateTimer(0.1, Timer_SpecRollCheckCounter,  _, TIMER_REPEAT);
	HudSetText("ROLL!\nJoin Spec Now!");
}

public Action:Timer_SpecRollCheckCounter(Handle timer) {
	if(CountPlayersInTeam() <= 2) {
		DebugLog("[SPECROLL] Rolling Complete");
		
		Handle RolledPlayers = CreateArray();
		
		for(int i = 0; i < MAX_PLAYERS; i++) {
			PushArrayCell(RolledPlayers, g_bSpecRollPlayer[i]);
		}
		
		OnRollingComplete(RolledPlayers);
		
		return Plugin_Stop;
	}
	
	return Plugin_Continue;		
}