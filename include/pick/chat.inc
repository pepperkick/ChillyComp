int g_iChatPickClient = -1;

SetChatPickClient(client) {
    g_iChatPickClient = client;
}

public Action:Command_PickPlayer(client, args) {
    DebugLog("Recieved Pick Player Command during state %d", GetStatus());

    if (GetStatus() != STATE_PICKING) {
        CPrintToChat(client, "%s[%s] %t", COLOR_TAG, TAG, "Pick-Chat-Disabled");

        return Plugin_Handled;
    }

    if (g_iChatPickClient != client) {
        CPrintToChat(client, "%s[%s] %t", COLOR_TAG, TAG, "Pick-Chat-NotUsable");

        return Plugin_Handled;
    }

    char arg[32];
    int target = -1;
    GetCmdArg(1, arg, sizeof(arg));

    DebugLog("Argument: %s", arg);

    if (IsInteger(arg)) {
        target = StringToInt(arg);
    } else {
        char target_name[MAX_TARGET_LENGTH];
        int targets[MAX_PLAYERS];
        bool tn_is_ml;

        int traget_count = ProcessTargetString(arg, client, targets, MAX_PLAYERS, COMMAND_FILTER_NO_BOTS, target_name, sizeof(target_name), tn_is_ml);

        if (traget_count == 0) {
            CPrintToChat(client, "%s[%s] %t", COLOR_TAG, TAG, "Pick-Chat-NotFound", arg);

            return Plugin_Handled;
        } else if (traget_count > 1) {
            CPrintToChat(client, "%s[%s] %t", COLOR_TAG, TAG, "Pick-Chat-ManyFound");

            return Plugin_Handled;
        }

        target = targets[0];
    }

    if (!IsClientProper(target)) {
        CPrintToChat(client, "%s[%s] %t", COLOR_TAG, TAG, "Pick-Chat-NotFound", arg);

        return Plugin_Handled;
    }

    bool isRolled = IsPlayerRolled(target);
    bool isPlusOne = IsPlayerPlusOne(target);

    if (!isRolled && !isPlusOne) {
        char name[1024];
        GetClientName(target, name, sizeof(name));

        CPrintToChat(client, "%s[%s] %t", COLOR_TAG, TAG, "Pick-Chat-NotRolled", name);

        return Plugin_Handled;
    }

    OnPick(target);

    return Plugin_Continue;
}

public Action:Command_PickList(client, args) {
    if (GetStatus() != STATE_PICKING) {
        CPrintToChat(client, "%s[%s] %t", COLOR_TAG, TAG, "Pick-Chat-Disabled");

        return Plugin_Handled;
    }

    char message[8192];
    char name[1024];

    Format(message, sizeof(message), "%s[%s]\n%t\n{matYellow}", COLOR_TAG, TAG, "Pick-List-Heading");

    for(int i = 0; i < MAX_PLAYERS; i++) {
        bool isRolled = IsPlayerRolled(i);        // Is player rolled
        bool isPlusOne = IsPlayerPlusOne(i);      // Is player plus one marked

        if (!isPlusOne || !isRolled) continue;

        GetClientName(i, name, sizeof(name));

        Format(message, sizeof(message), "%s%d: %s (+1)\n", message, i, name);
    }

    for(int i = 0; i < MAX_PLAYERS; i++) {
        bool isRolled = IsPlayerRolled(i);        // Is player rolled
        bool isPlusOne = IsPlayerPlusOne(i);      // Is player plus one marked

        if (!isRolled || isPlusOne) continue;

        GetClientName(i, name, sizeof(name));

        Format(message, sizeof(message), "%s%d: %s\n", message, i, name);
    }

    CPrintToChat(client, "%s", message);

    return Plugin_Continue;
}