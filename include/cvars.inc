Handle g_hcEnabled              = INVALID_HANDLE;
Handle g_hcAutoStart            = INVALID_HANDLE;
Handle g_hcConfigPath           = INVALID_HANDLE;
Handle g_hcPlayerPenaltyEnable  = INVALID_HANDLE;
Handle g_hcPlayerPenaltyTime    = INVALID_HANDLE;
Handle g_hcFightHealth          = INVALID_HANDLE;
Handle g_hcRollMode             = INVALID_HANDLE;
Handle g_hcGameStatus           = INVALID_HANDLE;
Handle g_hcTeamSize             = INVALID_HANDLE;
Handle g_hcTeamLimitEnable      = INVALID_HANDLE;
Handle g_hcLiveRestarts         = INVALID_HANDLE;
Handle g_hcRestartGameCvar      = INVALID_HANDLE;

public CreateCvars() {
    //Plugin Version ConVar
    CreateConVar(
        "chillycomp_version",
        PLUGIN_VERSION,
        "ChillyComp Plugin Version",
        FCVAR_SPONLY | FCVAR_REPLICATED | FCVAR_NOTIFY
    );

    // CVar to store if plugin is enabled
    g_hcEnabled =
        CreateConVar(
            "cc_enable",
            "1",
            "Enables or disables the whole plugin\n0 = Disabled\n1 = Enabled",
            FCVAR_NOTIFY | FCVAR_DONTRECORD
        );

    // CVar to store if rolling should auto start when enough players are in
    g_hcAutoStart =
        CreateConVar(
            "cc_rolling_autostart",
            "0",
            "0=Disable auto start, 1=Enable auto start",
            FCVAR_NOTIFY | FCVAR_DONTRECORD
        );

    // CVar to store the config's name that gets executed when rolling completes
    g_hcConfigPath =
        CreateConVar(
            "cc_config_path",
            "compconfigs",
            "Execute configs depending on events",
            FCVAR_DONTRECORD
        );

    // CVar to store if penalty is enabled
    g_hcPlayerPenaltyEnable =
        CreateConVar(
            "cc_rolling_penalty_enable",
            "0",
            "Should players get penalty for trying to join spectator before the roll message",
            FCVAR_NOTIFY | FCVAR_DONTRECORD
        );

    // CVar to store the time of penalty
    g_hcPlayerPenaltyTime =
        CreateConVar(
            "cc_rolling_penalty_time",
            "2",
            "Number of seconds of penalty",
            FCVAR_DONTRECORD
        );

    // CVar to store the health of leaders during melee fight
    g_hcFightHealth =
        CreateConVar(
            "cc_rolling_fight_health",
            "150",
            "Health of captains during melee fighting process (Default: 150)",
            FCVAR_DONTRECORD
        );

    // CVar to store the mode that rolling will follow
    g_hcRollMode =
        CreateConVar(
            "cc_rolling_mode",
            "1",
            "1: Spec Roll (Last 2 players become captain)\n2: Captain (All players are moved to spec, 2 players voluntarily join to become captains)",
            FCVAR_DONTRECORD
        );

    // CVar to store the status of the game
    g_hcGameStatus =
        CreateConVar(
            "cc_match_status",
            "0",
            "Status of the match",
            FCVAR_NOTIFY | FCVAR_DONTRECORD
        );

    // CVar to store the number of players per team
    g_hcTeamSize =
        CreateConVar(
            "cc_match_teamsize",
            "6",
            "Number of Players for each team",
            FCVAR_DONTRECORD
        );

    // CVar to store if teams should be limited during match
    g_hcTeamLimitEnable =
        CreateConVar(
            "cc_match_teamlimit",
            "1",
            "0=Disable\n1=Enable",
            FCVAR_DONTRECORD
        );

    g_hcLiveRestarts =
        CreateConVar(
            "cc_liverestart",
            "1",
            "Restart the match number of times in spawn",
            FCVAR_NOTIFY | FCVAR_DONTRECORD
        );

    g_hcRestartGameCvar = FindConVar("mp_restartgame");
}