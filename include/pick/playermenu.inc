int g_iPickingMenuClient = -1;

PickingShowPlayerMenu(client, players) {
    g_mPickMenu = new Menu(PickPanelHandler);
    g_iPickingMenuClient = client;

    char clientId[128];
    char name[1024];
    char itemName[1036];

    g_mPickMenu.SetTitle(g_sPickMenuTitle);

    DebugLog("[MENU] Showing menu to client %d", client);

    #if defined DEBUG
        DebugLog("Players List");
        for(int i = 0; i < GetArraySize(players); i++) {
            int rolled = GetArrayCell(players, i);
            DebugLog("%d - %d", i, rolled);
        }
    #endif

    int count = GetArrayTrueCount(players);

    if(count == 0 || players >= (GetConVarInt(g_hcTeamSize) * 2)) return PickingComplete();

    for(int i = 0; i < GetArraySize(players); i++) {
        bool isRolled = GetArrayCell(players, i);       // Is player rolled
        bool isPlusOne = isPlusOnePlayer(i);            // Is player plus one marked

        DebugLog("[MENU] Player %d menu status %d %d", i, isRolled, isPlusOne);

        if (!isRolled && !isPlusOne) continue;

        GetClientName(i, name, sizeof(name));
        IntToString(i, clientId, sizeof(clientId));

        if (isPlusOne) {
            Format(itemName, sizeof(itemName), "%s (+1)", name);

            DebugLog("[MENU] Player %s added to menu as plus one", name);

            g_mPickMenu.InsertItem(0, clientId, itemName);
        } else {
            DebugLog("[MENU] Player %s added to menu", name);

            g_mPickMenu.AddItem(clientId, name);
        }
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

        if (param2 != 0 && g_iPickingMenuClient != -1) {
            PickingShowPlayerMenu(g_iPickingMenuClient, g_hPickRolledPlayers);
        }
    }
}