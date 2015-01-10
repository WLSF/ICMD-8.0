/*
*
*
*       ICMD 8.0 Por Willian Luigi
*
*   -LOG:
*   Data de inicio: 07/10/2013 ICMD versão 7.0 beta
*                   11/10/2013 ICMD versão 7.1 alpha
*
*   Ultima atualização: 24/12/2014 ICMD versão 8.0 beta
*
*
*   - CODE.ME, 2013.1 - Willian Luigi
*
*
*/

#if !defined isnull
    #define isnull(%1) \
                ((!(%1[0])) || (((%1[0]) == '\1') && (!(%1[1]))))
#endif
#define g<%0> gotoaddr(%0)
#define gl<%0> saveaddr(#%0)
#define set(%0) gotoaddr(%0)
#define get_cmd_params(%0) gl<%0>;playerid=_wrefPlayerId;r(1);if(!isnull(_wrefCmdParams))strcat((params[0]=EOS,params),_wrefCmdParams, 128);if(_wrefHash(#%0)==_wrefCmdHash)
#define get_cmd_noparams(%0) gl<%0>;playerid=_wrefPlayerId;r(1);if(_wrefHash(#%0)==_wrefCmdHash)
#define F_VALUE _wrefReturnValue
#define r(%0) F_CMD(%0)
new _wrefPlayerId, _wrefCmdHash, _wrefCmdParams[128], _wrefReturnValue;
#pragma unused _wrefPlayerId
#pragma unused _wrefCmdHash
#pragma unused _wrefCmdParams
stock saveaddr(name[])
{
    static addr;
    #emit load.s.pri 4
    #emit stor.pri addr
    setproperty(.id = 0, .name = name, .value = addr);
}
stock gotoaddr(label[])
{
    static addr;
    addr = getproperty(0,label);
    #emit load.pri addr
    #emit sctrl 6
}
stock _wrefHash(s[])
{
    return s[0];
}
stock F_CMD(value)
{
    _wrefReturnValue = value;
    return value;
}

public OnPlayerCommandText(playerid, cmdtext[])
{
    _wrefCmdParams[0]=EOS;
    new __@i = 2;
    __@i = strfind(cmdtext, " ");
    _wrefPlayerId = playerid;
    _wrefCmdHash = cmdtext[1];

    if (__@i != -1)
    {
        new __@cmd[24];
        strmid(__@cmd, cmdtext, 1, __@i);
        while(cmdtext[++__@i] == 0x20){}
        strcat(_wrefCmdParams, cmdtext[__@i]);
        g<__@cmd>;
    }
    else
    {

        g<cmdtext[1]>;
    }

    return F_VALUE;
}

public OnGameModeInit()
{

    OnCommandCalled(0, "");

    if (funcidx("icmd_OnGameModeInit") != -1)
    {
        return CallLocalFunction("icmd_OnGameModeInit", "");
    }
    return 1;
}
#if defined _ALS_OnGameModeInit
    #undef OnGameModeInit
#else
    #define _ALS_OnGameModeInit
#endif
#define OnGameModeInit icmd_OnGameModeInit
forward icmd_OnGameModeInit();
forward OnCommandCalled(playerid, params[]);
