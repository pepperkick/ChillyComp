//============================================
//    MeleeMode
//        Brings the last two players to melee mode
//============================================

int g_iMeleeFightBluLeader = 0;
int g_iMeleeFightRedLeader = 0;

MeleeFightStart(bluLeader, redLeader) {
    DebugLog("[MELEEFIGHT] Start Melee Fight Sequence");

    g_iMeleeFightBluLeader = bluLeader;
    g_iMeleeFightRedLeader = redLeader;

    // TODO: Strip captians to melee only
    // TODO: Set captian's health to full if already in proper class

    if(TF2_GetPlayerClass(redLeader) != TFClass_Medic) {
        TF2_SetPlayerClass(redLeader, TFClass_Medic);
        SDKHooks_TakeDamage(redLeader, redLeader, redLeader, 1000.0);
    }

    if(TF2_GetPlayerClass(bluLeader) != TFClass_Medic) {
        TF2_SetPlayerClass(bluLeader, TFClass_Medic);
        SDKHooks_TakeDamage(bluLeader, bluLeader, bluLeader, 1000.0);
    }

    SetEntityHealth(redLeader, GetConVarInt(g_hcFightHealth))
    SetEntityHealth(bluLeader, GetConVarInt(g_hcFightHealth))

    HookEvent("player_death", Event_PlayerDeath);        // Hook Player Death Event
    HookEvent("player_class", Event_PlayerClassChange);  // Hook Player Class Change Event

    HudSetColor(139, 195, 74);
    HudSetText("Fight as Medics!");

    char bluLeaderName[1024];
    char redLeaderName[1024];

    GetClientName(bluLeader, bluLeaderName, sizeof(bluLeaderName));
    GetClientName(redLeader, redLeaderName, sizeof(redLeaderName));

    CPrintToChatAll("%s[%s] %t", COLOR_TAG, TAG, "Fight-Start-MSG", bluLeaderName, redLeaderName)
}

MeleeFightReset() {
    g_iMeleeFightBluLeader = 0;
    g_iMeleeFightRedLeader = 0;
}

public Action:Event_PlayerDeath(Handle:event, const String:name[], bool:dontBroadcast) {
    new killer = GetClientOfUserId(GetEventInt(event, "attacker"));
    new victim = GetClientOfUserId(GetEventInt(event, "userid"));
    new team   = GetClientTeam(killer);

    if(victim == killer) {
        DebugLog("[MELEEFIGHT] Captain Died due to unknow reason, switching teams!");

        team = team == 3 ? 2 : 3;

        killer = g_iMeleeFightRedLeader == victim ? g_iMeleeFightBluLeader : g_iMeleeFightRedLeader;
    }

    FightingComplete(killer, victim);

    DebugLog("[MELEEFIGHT] Killer: %d! Victim: %d! Team: %d!", killer, victim, team);

    UnhookEvent("player_death", Event_PlayerDeath);
}

public Action:Event_PlayerClassChange(Handle:event, const String:name[], bool:dontBroadcast) {
    int client = GetClientOfUserId(GetEventInt(event, "userid"));
    int class = GetEventInt(event, "class");

    if (class != CLASS_MEDIC) {
        SDKHooks_TakeDamage(client, client, client, 1000.0);
    }
 }