
//=========================================================================
//		C A P T A I N 		  	F U N C T I O N S 	S T A R T

public StartCaptainSequence() {
	if(GetConVarInt(g_hcEnabled) == 0) return;	//Checks if rolling plugin is enabled or not
	
	MoveAllToSpec();
	
	g_bRollingSequence 	= true;	//Sets Rolling Sequence started to True
	g_iPlayerCount 		= 0;	//Sets Player Count to Total Players in Server
}

public MoveAllToSpec() {
	for(new i = 1; i <= MaxClients; i++) {
		if(IsClientInGame(i) && !IsFakeClient(i) && (GetClientTeam(i) == TEAM_RED || GetClientTeam(i) == TEAM_BLU)) {
			ChangeClientTeam(i, TEAM_SPEC);
		}
	}	
}

//		C A P T A I N 		  	F U N C T I O N S 	E N D
//=========================================================================