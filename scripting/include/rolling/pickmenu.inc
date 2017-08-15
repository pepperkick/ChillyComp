
public RollingMenu(client) {	
	g_mPickMenu = new Menu(PickPanelHandler);
	
	char clientId[128];
	char name[1024];
		
	g_mPickMenu.SetTitle("Pick Player");
	
	if(g_iPlayerCount + 1 != g_iPlayerCounter) {
		for(int i = 1; i < MaxClients; i++) {
			if(IsClientInGame(i) && !IsFakeClient(i) && g_hPlayerRolled[i]) {
				GetClientName(i, name, sizeof(name));				
				IntToString(i, clientId, sizeof(clientId));
				
				if(g_bPlusOneAllowed[i] && g_iPlusOneAllowed != 0) {
					g_mPickMenu.AddItem(clientId, name);					
				}
			}		
		}
	}
		
	g_mPickMenu.ExitButton = false;
	g_mPickMenu.Display(client, 300);
}

public PickPanelHandler(Menu:menu, MenuAction:action, param1, param2) {	
	if(g_iPlayerCounter >= (GetConVarInt(g_hcTeamSize) * 2)) {		
		// RollingComplete();
	}
	
	if (action == MenuAction_Select) {
		char clientId[128];
		
		menu.GetItem(param2, clientId, sizeof(clientId));
		int client = StringToInt(clientId, 10);		
		
		g_hPlayerRolled[client] = false;	
		
		if(g_bPlusOneAllowed[client]) {
			g_iPlusOneAllowed--;
		}
		
		if(g_iPickingTeam == 3) {
			ChangeClientTeam(client, 3);
			g_iPlayerPicked[client] = 3;
			
			g_iPickingTeam = TEAM_RED;
			SetHudText("RED Pick!");
			
			RollingMenu(g_iRedTeamLeader);
		} else if(g_iPickingTeam == 2) {
			ChangeClientTeam(client, 2);
			g_iPlayerPicked[client] = 2;
			
			g_iPickingTeam = TEAM_BLU;
			SetHudText("BLU Pick!");
			
			RollingMenu(g_iBluTeamLeader)
		}
		
		g_iPlayerCounter++;
	} else if (action == MenuAction_Cancel) {
		PrintToServer("[ROLL] Client %d's menu was cancelled. Reason: %d", param1, param2);
	}
}
