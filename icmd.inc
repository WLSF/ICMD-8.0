/*
 * Copyright (C) 2012 Incognito
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// Definitions

#define GLOBAL_VARTYPE_NONE (0)
#define GLOBAL_VARTYPE_INT (1)
#define GLOBAL_VARTYPE_STRING (2)
#define GLOBAL_VARTYPE_FLOAT (3)

// Natives

native SetGVarInt(const name[], value, id = 0);
native GetGVarInt(const name[], id = 0);
native SetGVarString(const name[], const value[], id = 0);
native GetGVarString(const name[], dest[], maxlength = sizeof dest, id = 0);
native SetGVarFloat(const name[], Float:value, id = 0);
native Float:GetGVarFloat(const name[], id = 0);
native DeleteGVar(const name[], id = 0);
native GetGVarsUpperIndex(id = 0);
native GetGVarNameAtIndex(index, dest[], maxlength = sizeof dest, id = 0);
native GetGVarType(const name[], id = 0);


/*
*        _
*		(_)
*		 _     ____    ___  ___   ____
*		| |   /  __|  |   \/   | |  _ \
*		| |  |  /     |        | | | \ |
*		| |  |  \__   |  /\/\  | | | / |
*		|_|   \____|  |_/    \_| |____/
*
*
*       ICMD 8.0 Por Willian Luigi
*
*   -LOG:
*   Data de inicio: 07/10/2013 ICMD versão 7.0 beta
*                   11/10/2013 ICMD versão 7.1 alpha
*
*   Ultima atualização: 24/12/2014 ICMD versão 8.0 beta
*						08/01/2015 ICMD versão 8.0 oficial
*
*
*   - CODE.ME, 2013.1 - Willian Luigi
*
*
*/

//thanks to original creator (isnull)
#if !defined isnull
    #define isnull(%1) \
                ((!(%1[0])) || (((%1[0]) == '\1') && (!(%1[1]))))
#endif
#define get_cmd_params(%0) 	___@ICMD_SAVE_ADDR(#%0);playerid=_wrefPlayerId;if(!isnull(_wrefCmdParams))strcat((params[0]=EOS,params),_wrefCmdParams, 128);if(_wrefHash(#%0)==_wrefCmdHash)
#define get_cmd_noparams(%0) ___@ICMD_SAVE_ADDR(#%0);playerid=_wrefPlayerId;if(_wrefHash(#%0)==_wrefCmdHash)
#define cmd_exists(%0) (GetGVarType(%0)!=GLOBAL_VARTYPE_NONE)
#if defined CMD
	#undef CMD
#endif
#if defined COMMAND
	#undef COMMAND
#endif
#define CMD:%0(%1) get_cmd_params(%0)
#define COMMAND:%0(%1) get_cmd_params(%0)
#define cmd_%0(%1) ___@ICMD_GOTO_ADDR(#%0)
#define call(%0) ___@ICMD_GOTO_ADDR(%0)
new _wrefPlayerId, _wrefCmdHash, _wrefCmdParams[128], _wrefReturnValue;
#pragma unused _wrefPlayerId
#pragma unused _wrefCmdHash
#pragma unused _wrefCmdParams
#pragma unused _wrefReturnValue
#define ICMD_ERROR 2
#define ICMD_DEFAULT 0
#define ICMD_OKAY 1
stock ___@ICMD_SAVE_ADDR(name[])
{
 	static addr;
	#emit load.s.pri 4 
	#emit stor.pri addr
	//setproperty(.id = 0, .name = name, .value = addr);
	SetGVarInt(name, addr);
}
stock ___@ICMD_GOTO_ADDR(label[])
{
	static addr;
	//addr = getproperty(0,label);
	addr = GetGVarInt(label);
	#emit load.pri addr
	#emit sctrl 6
}
stock _wrefHash(s[])
{
	return s[0];
}
public OnPlayerCommandText(playerid, cmdtext[])
{
    	static __@cmd[24];
    	_wrefCmdParams[0]=EOS;
	new __@i = strfind(cmdtext, " ");
    	_wrefPlayerId = playerid;
    	_wrefCmdHash = cmdtext[1];

	if (__@i != -1)
	{
		strmid(__@cmd, cmdtext, 1, __@i);
		#if defined OnCommandCalled
	    	switch (OnCommandCalled(playerid, __@cmd))
	    	{
			case 0: return ICMD_DEFAULT;
			case 2: return ICMD_ERROR;
		}
		#endif
		if (cmd_exists(__@cmd)) 
		{
			while(cmdtext[++__@i] == 0x20){}
			strcat(_wrefCmdParams, cmdtext[__@i]);
			___@ICMD_GOTO_ADDR(__@cmd);
			return 1;
		}
		return 0;
	}
	else
	{
		#if defined OnCommandCalled
		    	switch (OnCommandCalled(playerid, cmdtext[1]))
		    	{
				case 0: return ICMD_DEFAULT;
				case 2: return ICMD_ERROR;
			}
		#endif
		if (cmd_exists(cmdtext[1])) 
		{
			___@ICMD_GOTO_ADDR(cmdtext[1]);
			return 1;
		}
		return 0;
	}
}

//_ALS_ hook method 7, Thanks to Y_Less and ipsBruno
#if defined FILTERSCRIPT
	public OnFilterScriptInit()
	{
		Listeners(0, "");
		#if defined icmd_OnFilterScriptInit
			icmd_OnFilterScriptInit();
		#endif
		return 1;
	}
	#if defined _ALS_OnFilterScriptInit
	    #undef OnFilterScriptInit
	#else
	    #define _ALS_OnFilterScriptInit
	#endif
	#define OnFilterScriptInit icmd_OnFilterScriptInit
	forward icmd_OnFilterScriptInit();
	
	public OnFilterScriptExit()
	{
		//deleting all the GVars allocated till the moment. Thanks to Incognito
		new name[24];
		for (new i = 0, j = GetGVarsUpperIndex(); i < j; i++)
		{
		    if (GetGVarNameAtIndex(i, name))
		    {
		        DeleteGVar(name);
		    }
		}
	
		#if defined icmd_OnFilterScriptExit
			icmd_OnFilterScriptExit();
		#endif
		return 1;
	}
	#if defined _ALS_OnFilterScriptExit
	    #undef OnFilterScriptExit
	#else
	    #define _ALS_OnFilterScriptExit
	#endif
	#define OnFilterScriptExit icmd_OnFilterScriptExit
	forward icmd_OnFilterScriptExit();

#else
	public OnGameModeInit()
	{
		Listeners(0, "");
		#if defined icmd_OnGameModeInit
			icmd_OnGameModeInit();
		#endif
		return 1;
	}
	#if defined _ALS_OnGameModeInit
	    #undef OnGameModeInit
	#else
	    #define _ALS_OnGameModeInit
	#endif
	#define OnGameModeInit icmd_OnGameModeInit
	#if defined icmd_OnGameModeInit
		forward icmd_OnGameModeInit();
	#endif
	public OnGameModeExit()
	{
		//deleting all the GVars allocated till the moment. Thanks to Incognito
		new name[24];
		for (new i = 0, j = GetGVarsUpperIndex(); i < j; i++)
		{
		    if (GetGVarNameAtIndex(i, name))
		    {
		        DeleteGVar(name);
		    }
		}
	
		#if defined icmd_OnGameModeExit
			icmd_OnGameModeExit();
		#endif
		return 1;
	}
	#if defined _ALS_OnGameModeExit
	    #undef OnGameModeExit
	#else
	    #define _ALS_OnGameModeExit
	#endif
	#define OnGameModeExit icmd_OnGameModeExit
	#if defined icmd_OnGameModeExit
		forward icmd_OnGameModeExit();
	#endif
#endif

forward Listeners(playerid, params[]);

#if defined OnCommandCalled
forward OnCommandCalled(playerid, cmd[]);
#endif
