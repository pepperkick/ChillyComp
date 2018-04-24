Menu g_mChangeRollStatusMenu;

int g_iServingChangeRollStatusMenu = -1;

public ChangeRollStatusMenu(client) {
    g_mChangeRollStatusMenu = new Menu(ChangeRollStatusHandler);

    g_mChangeRollStatusMenu.SetTitle("Pick Player to change roll status");

    for(int i = 1; i < MaxClients; i++) {
        if(IsClientInGame(i) && !IsFakeClient(i)) {
            char name[1024], clientId[4], listName[1036];

            GetClientName(i, name, sizeof(name));
            IntToString(i, clientId, sizeof(clientId));
            Format(listName, sizeof(listName), "%s (%s)", name, IsPlayerRolled(i) ? "o" : "x");

            g_mChangeRollStatusMenu.AddItem(clientId, name);
        }
    }

    g_iServingChangeRollStatusMenu = client;

    g_mChangeRollStatusMenu.ExitButton = true;
    g_mChangeRollStatusMenu.Display(client, MENU_TIME_FOREVER);
}

public ChangeRollStatusHandler(Menu:menu, MenuAction:action, param1, param2) {
    if (action == MenuAction_Select) {
        char clientId[128];

        menu.GetItem(param2, clientId, sizeof(clientId));
        int client = StringToInt(clientId, 10);

        if (IsPlayerRolled(client)) {
            SetPlayerRolled(client, false);
        } else {
            SetPlayerRolled(client, true);
        }

        if (g_iServingChangeRollStatusMenu > -1) {
            ChangeRollStatusMenu(g_iServingChangeRollStatusMenu);
        }
    } else if (action == MenuAction_Cancel) {
        if (param2 == MenuCancel_Exit) {
            DebugLog("Client %d exited change roll status menu", param1);

            g_iServingChangeRollStatusMenu = -1;

            delete g_mChangeRollStatusMenu;
        }

        DebugLog("Client %d's change roll status was cancelled. Reason: %d", param1, param2);
    }
}

public Action:Command_ChangeRollStatus(client, args) {
    if (GetStatus() > STATE_ROLLING && GetStatus() < STATE_SETUP) {
        CPrintToChat(client, "%s[%s] %t", COLOR_TAG, TAG, "Module-Admin-ChangeRollStatus-CommandBlocked");

        return Plugin_Handled;
    }

    ChangeRollStatusMenu(client);
    
    return Plugin_Continue;
}