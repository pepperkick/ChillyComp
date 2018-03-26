int retry[MAX_PLAYERS] = 0;

PickingShowPlayerMenu(client, Handle players) {
    g_mPickMenu = new Menu(PickPanelHandler);

    char clientId[128];
    char name[1024];
    char itemName[1036];

    g_mPickMenu.SetTitle(g_sPickMenuTitle);

    DebugLog("[MENU] Showing menu to client %d", client);

    for(int i = 0; i < GetArraySize(players); i++) {
        bool isRolled = GetArrayCell(players, i);       // Is player rolled
        bool isPlusOne = IsPlayerPlusOne(i);            // Is player plus one marked

        if (!isPlusOne || !isRolled) continue;

        DebugLog("[MENU] Player %d menu status %d", i, isPlusOne);

        GetClientName(i, name, sizeof(name));
        IntToString(i, clientId, sizeof(clientId));

        Format(itemName, sizeof(itemName), "%s (+1)", name);

        DebugLog("[MENU] Player %s added to menu as plus one", name);

        g_mPickMenu.AddItem(clientId, itemName);
    }

    for(int i = 0; i < GetArraySize(players); i++) {
        bool isRolled = GetArrayCell(players, i);       // Is player rolled
        bool isPlusOne = IsPlayerPlusOne(i);            // Is player plus one marked

        if (!isRolled || isPlusOne) continue;

        DebugLog("[MENU] Player %d menu status %d", i, isRolled);

        GetClientName(i, name, sizeof(name));
        IntToString(i, clientId, sizeof(clientId));

        DebugLog("[MENU] Player %s added to menu", name);

        g_mPickMenu.AddItem(clientId, name);
    }

    g_mPickMenu.ExitButton = false;
    g_mPickMenu.Display(client, MENU_TIME_FOREVER);
}

public PickPanelHandler(Menu:menu, MenuAction:action, param1, param2) {
    if (action == MenuAction_Select) {
        char clientId[128];

        //TODO: Add confirm menu.

        menu.GetItem(param2, clientId, sizeof(clientId));
        int client = StringToInt(clientId, 10);

        OnPick(client);
    } else if (action == MenuAction_Cancel) {
        DebugLog("[ROLL] Client %d's menu was cancelled. Reason: %d", param1, param2);

        if (!(param2 == -1 || param2 == -4) && retry[param1] < 5) {
            retry[param1]++;

            PickingShowPlayerMenu(param1, g_hPickRolledPlayers);
        } else {
            DebugLog("[ROLL] Failed to show pick menu properly to client %d!", param1)

            CPrintToChat(param1, "%s[%s] %t", COLOR_TAG, TAG, "Pick-Menu-FailedToShow");
        }
    }
}

ResetPickMenuRetries() {
    for (int i = 0; i < MAX_PLAYERS; i++) {
        retry[i] = 0;
    }
}