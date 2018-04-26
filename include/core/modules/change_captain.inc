Menu g_mChangeCaptainMenu;

int g_iServingChangeCaptainMenu = -1;
int g_iServingChangeCaptainTeam = -1;

public ChangeCaptainMenu(client, team) {
    g_mChangeCaptainMenu = new Menu(ChangeCaptainHandler);

    g_mChangeCaptainMenu.SetTitle("Pick Player to swap as captain");

    DebugLog("Creating captain swap menu");

    for(int i = 1; i < MaxClients; i++) {        
        DebugLog("Checking player %d: %d", i, IsPlayerRolled(i));
        
        if(IsClientInGame(i) && IsPlayerRolled(i)) {
            char name[1024], clientId[4];

            GetClientName(i, name, sizeof(name));
            IntToString(i, clientId, sizeof(clientId));

            g_mChangeCaptainMenu.AddItem(clientId, name);
        }
    }

    g_iServingChangeCaptainMenu = client;
    g_iServingChangeCaptainTeam = team;

    g_mChangeCaptainMenu.ExitButton = true;
    g_mChangeCaptainMenu.Display(client, MENU_TIME_FOREVER);
}

public ChangeCaptainHandler(Menu:menu, MenuAction:action, param1, param2) {
    if (action == MenuAction_Select) {
        char clientId[128];

        menu.GetItem(param2, clientId, sizeof(clientId));
        int client = StringToInt(clientId, 10);

        SwapTeamCaptain(client, g_iServingChangeCaptainTeam);

        g_iServingChangeCaptainTeam = -1;

        delete g_mChangeCaptainMenu;
    } else if (action == MenuAction_Cancel) {
        if (param2 == MenuCancel_Exit) {
            DebugLog("Client %d exited change captain menu", param1);

            g_iServingChangeCaptainMenu = -1;

            delete g_mChangeCaptainMenu;
        }

        DebugLog("Client %d's change captain was cancelled. Reason: %d", param1, param2);
    }
}

public Action:Command_SwapCaptainRed(client, args) {
    DebugLog("Recieved Command to Change Captain for RED team");

    if (GetStatus() != STATE_FIGHTING) {
        CPrintToChat(client, "%s[%s] %t", COLOR_TAG, TAG, "Module-ChangeCaptain-CommandBlocked");

        return Plugin_Handled;
    }

    ChangeCaptainMenu(client, TEAM_RED);

    return Plugin_Continue;
}

public Action:Command_SwapCaptainBlu(client, args) {
    DebugLog("Recieved Command to Change Captain for BLU team");

    if (GetStatus() != STATE_FIGHTING) {
        CPrintToChat(client, "%s[%s] %t", COLOR_TAG, TAG, "Module-ChangeCaptain-CommandBlocked");

        return Plugin_Handled;
    }

    ChangeCaptainMenu(client, TEAM_BLU);

    return Plugin_Continue;
}

SwapTeamCaptain(client, team) {
    if (team == TEAM_RED) {
        TF2_ChangeClientTeam(g_iFightRedLeader, TFTeam_Spectator);
        ChangeClientTeam(client, TEAM_RED);      

        SetPlayerRolled(g_iFightRedLeader, true);
        SetPlayerRolled(client, false);
    } else if (team == TEAM_BLU) {
        TF2_ChangeClientTeam(g_iFightBluLeader, TFTeam_Spectator);  
        ChangeClientTeam(client, TEAM_BLU);      

        SetPlayerRolled(g_iFightBluLeader, true);
        SetPlayerRolled(client, false)
    }
 
    FightReset();
    FightingStart();
}