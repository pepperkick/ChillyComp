Handle g_bPlayerReadyStatus = INVALID_HANDLE;

public Action:Command_PlayerReady(client, args) {
    if (GetConVarInt(g_hcPerPlayerReady) == 0) {
        CPrintToChat(client, "%s[%s] %t", COLOR_TAG, TAG, "Module-PlayerReadyUp-CommandDisabled");

        return Plugin_Handled;
    }

    if (GetStatus() != STATE_SETUP) {
        CPrintToChat(client, "%s[%s] %t", COLOR_TAG, TAG, "Module-PlayerReadyUp-CommandBlocked");

        return Plugin_Handled;
    }

    if (IsPlayerReady(client) == true) {
        CPrintToChat(client, "%s[%s] %t", COLOR_TAG, TAG, "Module-PlayerReadyUp-AlreadyReady");

        return Plugin_Handled;
    }

    if (IsClientInAnyTeam(client)) {
        CPrintToChat(client, "%s[%s] %t", COLOR_TAG, TAG, "Module-PlayerReadyUp-NotPicked");

        return Plugin_Handled;
    }

    SetPlayerReady(client, true);

    CheckReadyStatus();

    return Plugin_Continue;
}

public Action:Command_PlayerUnReady(client, args) {
    if (GetConVarInt(g_hcPerPlayerReady) == 0) {
        CPrintToChat(client, "%s[%s] %t", COLOR_TAG, TAG, "Module-PlayerReadyUp-CommandDisabled");

        return Plugin_Handled;
    }

    if (GetStatus() != STATE_SETUP) {
        CPrintToChat(client, "%s[%s] %t", COLOR_TAG, TAG, "Module-PlayerReadyUp-CommandBlocked");

        return Plugin_Handled;
    }    

    if (IsPlayerReady(client) == false) {
        CPrintToChat(client, "%s[%s] %t", COLOR_TAG, TAG, "Module-PlayerReadyUp-AlreadyUnReady");

        return Plugin_Handled;
    }

    if (IsClientInAnyTeam(client)) {
        CPrintToChat(client, "%s[%s] %t", COLOR_TAG, TAG, "Module-PlayerReadyUp-NotPicked");

        return Plugin_Handled;
    }

    SetPlayerReady(client, false);

    GameRules_SetProp("m_bTeamReady", 0, _ , GetClientTeam(client), true);

    CheckReadyStatus();
    
    return Plugin_Continue;
}

public CheckReadyStatus() {
    if (TotalPlayersReady() == (GetConVarInt(g_hcTeamSize) * 2)) {
        CPrintToChatAll("%s[%s] %t", COLOR_TAG, TAG, "Module-PlayerReadyUp-AllReady");
    }

	if (TotalPlayersReadyInTeam(TEAM_RED) == GetConVarInt(g_hcTeamSize)) {
        CPrintToChatAll("%s[%s] %t", COLOR_TAG, TAG, "Module-PlayerReadyUp-TeamReady-RED");
		GameRules_SetProp("m_bTeamReady", 1, _ , TEAM_RED, true);
	}

	if (TotalPlayersReadyInTeam(TEAM_BLU) == GetConVarInt(g_hcTeamSize)) {
        CPrintToChatAll("%s[%s] %t", COLOR_TAG, TAG, "Module-PlayerReadyUp-TeamReady-BLU");
		GameRules_SetProp("m_bTeamReady", 1, _ , TEAM_BLU, true);
	}
}

public Action:Event_TournamentStateUpdate(Handle:event, const String:name[], bool:dontBroadcast) {
    if (GetConVarInt(g_hcEnabled) == 0) return Plugin_Continue;
    
    if (GetConVarInt(g_hcPerPlayerReady) == 0) return Plugin_Continue;

	if (GetEventBool(event, "namechange")) return Plugin_Continue;
	
	if (TotalPlayersReadyInTeam(TEAM_RED) == GetConVarInt(g_hcTeamSize)) {
		GameRules_SetProp("m_bTeamReady", 0, _ , TEAM_RED, true);
	}

	if (TotalPlayersReadyInTeam(TEAM_BLU) == GetConVarInt(g_hcTeamSize)) {
		GameRules_SetProp("m_bTeamReady", 0, _ , TEAM_BLU, true);
	}

	return Plugin_Changed;
}

ClearReadyStatus() {
    for (int i = 1; i < MaxClients; i++) {
        SetPlayerReady(i, false);
    }
}