
//=========================================================================
//		P L A Y E R S		  	F U N C T I O N S 	S T A R T

public SetPlusOneList() {
	PrintToServer("[ROLL] Getting Plus One List");
	
	for(new i = 1; i <= MaxClients; i++) {
		if(IsClientInGame(i)) {
			if(GetClientTeam(i) == TEAM_BLU || GetClientTeam(i) == TEAM_RED) {		
				char temp[1024];
				
				GetClientAuthId(i, AuthId_SteamID64, temp, sizeof(temp), true);
				
				PushArrayString(g_hcPlusOnePlayerID, temp);
				
				PrintToServer("[ROLL] Added to Block Plus One List: %s", temp); 
				
				g_iPlusOneCount++;
			} 
		}	
	}
}

public CountPlusOnePlayers() {
	PrintToServer("[ROLL] Counting Plus One Players");
	
	char steamId[128];
	
	int AllowedPlayers = 0;
	int BlockedPlayers = 0;
	
	for(int i = 1; i < MaxClients; i++) {
		if(IsClientInGame(i) && !IsFakeClient(i) && g_hPlayerRolled[i]) {
			GetClientAuthId(i, AuthId_SteamID64, steamId, sizeof(steamId), true);
				
			int index = FindStringInArray(g_hcPlusOnePlayerID, steamId);
				
			if(index == -1) {
				g_bPlusOneAllowed[i] = true;
				
				PrintToServer("[ROLL] Player %s has been allowed in picking", steamId);
				
				AllowedPlayers++;
			} else {
				g_bPlusOneAllowed[i] = false;
				
				PrintToServer("[ROLL] Player %s has been blocked from picking", steamId);
				
				BlockedPlayers++;
			}
		}
	}
	
	g_iPlusOneAllowed = AllowedPlayers;
	g_iPlusOneBlocked = BlockedPlayers;
}

//		P L A Y E R S		  	F U N C T I O N S 	E N D
//=========================================================================