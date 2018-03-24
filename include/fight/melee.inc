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

    HookEvent("player_death", event_Death);        //Hook Player Death Event

    HudSetColor(0, 255, 0);
    HudSetText("Melee Fight as Medics!");

    char bluLeaderName[1024];
    char redLeaderName[1024];

    GetClientName(bluLeader, bluLeaderName, sizeof(bluLeaderName));
    GetClientName(redLeader, redLeaderName, sizeof(redLeaderName));

    CPrintToChatAll("%s[%s] %t", COLOR_TAG, TAG, "Fight-Start-MSG", bluLeaderName, redLeaderName)
}

public Action:event_Death(Handle:event, const String:name[], bool:dontBroadcast) {
    new killer     = GetClientOfUserId(GetEventInt(event, "attacker"));
    new victim     = GetClientOfUserId(GetEventInt(event, "userid"));
    new team     = GetClientTeam(killer);

    if(victim == killer) {
        DebugLog("[MELEEFIGHT] Captain Died due to unknow reason, switching teams!");

        team = team == 3 ? 2 : 3;

        killer = g_iMeleeFightRedLeader == victim ? g_iMeleeFightBluLeader : g_iMeleeFightRedLeader;
    }

    FightingComplete(killer, victim);

    DebugLog("[MELEEFIGHT] Killer: %d! Victim: %d! Team: %d!", killer, victim, team);

    UnhookEvent("player_death", event_Death);
}