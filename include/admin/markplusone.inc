bool g_bMarkedPlusOnePlayers[32];

int g_iServingMarkMenu = -1;

Menu g_mPlusOneMenu;

public MarkPlusOneMenu(client) {
    g_mPlusOneMenu = new Menu(MarkPlusOneHandler);

    char clientId[128];
    char name[1024];

    g_mPlusOneMenu.SetTitle("Pick Player to toggle Mark");

    if(g_iPlayerCount + 1 != g_iPlayerCounter) {
        for(int i = 1; i < MaxClients; i++) {
            if(IsClientInGame(i) && !IsFakeClient(i)) {
                GetClientName(i, name, sizeof(name));
                IntToString(i, clientId, sizeof(clientId));

                char listName[1036];

                Format(listName, sizeof(listName), "%s (%s)", name, g_bMarkedPlusOnePlayers[i] ? "o" : "x");

                g_mPlusOneMenu.AddItem(clientId, listName);
            }
        }
    }

    g_iServingMarkMenu = client;

    g_mPlusOneMenu.ExitButton = true;
    g_mPlusOneMenu.Display(client, MENU_TIME_FOREVER);
}

public MarkPlusOneHandler(Menu:menu, MenuAction:action, param1, param2) {
    if (action == MenuAction_Select) {
        char clientId[128];

        menu.GetItem(param2, clientId, sizeof(clientId));
        int client = StringToInt(clientId, 10);

        if (g_bMarkedPlusOnePlayers[client]) {
            g_bMarkedPlusOnePlayers[client] = false;
        } else {
            g_bMarkedPlusOnePlayers[client] = true;
        }

        if (g_iServingMarkMenu > -1) {
            MarkPlusOneMenu(g_iServingMarkMenu);
        }
    } else if (action == MenuAction_Cancel) {
        if (param2 == MenuCancel_Exit) {
            DebugLog("Client %d exited mark menu", param1);

            g_iServingMarkMenu = -1;

            g_mPlusOneMenu = INVALID_HANDLE;
        }

        DebugLog("Client %d's mark menu was cancelled. Reason: %d", param1, param2);
    }
}

isPlusOnePlayer(client) {
    return g_bMarkedPlusOnePlayers[client];
}

public Action:Command_MarkPlusOne(client, args) {
    MarkPlusOneMenu(client);
}