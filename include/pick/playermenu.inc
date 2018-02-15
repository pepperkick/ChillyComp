PickingShowPlayerMenu(client, players) {
    g_mPickMenu = new Menu(PickPanelHandler);

    char clientId[128];
    char name[1024];

    g_mPickMenu.SetTitle(g_sPickMenuTitle);

    DebugLog("[MENU] Showing menu to client %d", client);

    #if defined DEBUG
        DebugLog("Players List");
        for(int i = 0; i < GetArraySize(players); i++) {
            int rolled = GetArrayCell(players, i);
            DebugLog("%d - %d", i, rolled);
        }
    #endif

    for(int i = 0; i < GetArraySize(players); i++) {
        int toAdd = GetArrayCell(players, i);

        DebugLog("[MENU] Player %d menu status %d", i, toAdd);

        if(toAdd == 0) continue;

        GetClientName(i, name, sizeof(name));
        IntToString(i, clientId, sizeof(clientId));

        DebugLog("[MENU] Player %s added to menu", name);

        g_mPickMenu.AddItem(clientId, name);
    }

    g_mPickMenu.ExitButton = false;
    g_mPickMenu.Display(client, 300);
}

public PickPanelHandler(Menu:menu, MenuAction:action, param1, param2) {
    if (action == MenuAction_Select) {
        char clientId[128];

        //TODO: Add confirm menu.

        menu.GetItem(param2, clientId, sizeof(clientId));
        int client = StringToInt(clientId, 10);

        OnPick(client);
    } else if (action == MenuAction_Cancel) {
        PrintToServer("[ROLL] Client %d's menu was cancelled. Reason: %d", param1, param2);
    }
}