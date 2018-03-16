public CreateCvars() {
    //Enabled ConVar
    g_hcEnabled =
        CreateConVar(
            "sm_rolling_enable",
            "1",
            "0=Disable Rolling, 1=Enable Rolling",
            FCVAR_NOTIFY | FCVAR_DONTRECORD
        );

    //Team Size ConVar
    g_hcTeamSize =
        CreateConVar(
            "sm_rolling_team_size",
            "6",
            "Number of Players for each team before rolling starts",
            FCVAR_DONTRECORD
        );

    // Auto start eolling ConVar
    g_hcAutoStart =
        CreateConVar(
            "sm_rolling_autostart",
            "0",
            "0=Disable auto start, 1=Enable auto start",
            FCVAR_NOTIFY | FCVAR_DONTRECORD
        );

    //On Complete Config ConVar
    g_hcOnCompleteConfig =
        CreateConVar(
            "sm_rolling_on_complete",
            "rolled",
            "Execute config on roll complete",
            FCVAR_DONTRECORD
        );

    //Penalty Enable ConVar
    g_hcPlayerPenaltyEnable =
        CreateConVar(
            "sm_rolling_penalty_enable",
            "0",
            "Should players get penalty for trying to join spectator before the roll message",
            FCVAR_NOTIFY | FCVAR_DONTRECORD
        );

    //Penalty Time ConVar
    g_hcPlayerPenaltyTime =
        CreateConVar(
            "sm_rolling_penalty_time",
            "2",
            "Number of seconds of penalty",
            FCVAR_DONTRECORD
        );

    //Mode ConVar
    g_hcRollMode =
        CreateConVar(
            "sm_rolling_mode",
            "1",
            "1: Spec Roll (Last 2 players become captain)\n2: Captain (All players are moved to spec, 2 players voluntarily join to become captains)",
            FCVAR_DONTRECORD
        );
}