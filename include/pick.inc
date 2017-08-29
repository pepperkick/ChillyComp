Menu 	g_mPickMenu;

char	g_sPickMenuTitle[128];

Handle 	g_hPickRolledPlayers;

int		g_iPickingTeam = TEAM_NIL;

PickingStart(winner, loser, RolledPlayers) {
	DebugLog("Started Picking");

	g_sPickMenuTitle = "Pick a Player";
	
	g_iPickingTeam = GetClientTeam(winner);
	
	g_hPickRolledPlayers = RolledPlayers;
	
	if(g_iPickingTeam == TEAM_RED) {
		HudSetColor(255, 127, 127);
		HudSetText("RED Pick!");		
	} else if(g_iPickingTeam == TEAM_BLU) {
		HudSetColor(127, 127, 255);
		HudSetText("BLU Pick!");			
	}
	
	PickingShowPlayerMenu(winner, g_hPickRolledPlayers);
}

OnPick(client) {
	SetArrayCell(g_hPickRolledPlayers, client, false);	
		
	int count = GetArrayTrueCount(g_hPickRolledPlayers);
	int players = CountPlayersInTeam();
	
	DebugLog("Roll Count: %d, Players in team: %d", count, players);
	
	if(count == 0 || players >= (GetConVarInt(g_hcTeamSize) * 2)) PickingComplete();
		
	if(g_iPickingTeam == TEAM_BLU) {
		ChangeClientTeam(client, TEAM_BLU);
		SetArrayCell(g_hPlayerPicked, client, TEAM_BLU);
			
		g_iPickingTeam = TEAM_RED;
		
		HudSetColor(255, 127, 127);
		HudSetText("RED Pick!");	
			
		PickingShowPlayerMenu(g_iFightRedLeader, g_hPickRolledPlayers);
	} else if(g_iPickingTeam == TEAM_RED) {
		ChangeClientTeam(client, TEAM_RED);
		SetArrayCell(g_hPlayerPicked, client, TEAM_RED);
		
		g_iPickingTeam = TEAM_BLU;
		
		HudSetColor(127, 127, 255);
		HudSetText("BLU Pick!");	
		
		PickingShowPlayerMenu(g_iFightBluLeader, g_hPickRolledPlayers)
	}
}

PickingComplete() {
	DebugLog("Picking Complete");
}