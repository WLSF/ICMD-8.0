// This is a comment
// uncomment the line below if you want to write a filterscript
//#define FILTERSCRIPT

#include <a_samp>

//THIS TELL THE COMMAND PROCESSOR THAT YOU WANT TO USE THE PREPROCESSOR CALLBACK.
#define ICMD_CALLBACK

#include <icmd>

#if defined FILTERSCRIPT

public OnFilterScriptInit()
{
	print("\n--------------------------------------");
	print(" Blank Filterscript by your name here");
	print("--------------------------------------\n");
	return 1;
}

public OnFilterScriptExit()
{
	return 1;
}

#else

main()
{
	print("\n----------------------------------");
	print(" Blank Gamemode by your name here");
	print("----------------------------------\n");
}

#endif

public OnGameModeInit()
{
	// Don't use these lines if it's a filterscript
	SetGameModeText("Blank Script");
	AddPlayerClass(0, 1958.3783, 1343.1572, 15.3746, 269.1425, 0, 0, 0, 0, 0, 0);
	return 1;
}

//callback to process somethings before the command get called.
public OnCommandCalled(playerid, cmd[])
{
	//method to verify if the command typed has a listener (exists)
	if (!cmd_exists(cmd))
	{
		SendClientMessage(playerid, -1, "Comando inexistente.");
	    return ICMD_ERROR;
		//ICMD_ERROR = 2, command not found, just close all the executions.
		//ICMD_DEFAULT = 0, command not found, send the pattern message "Unknown command"
	}
	return ICMD_OKAY;
	//ICMD_OKAY = 1, let the command listener begin.
}

//callback to set all the command listeners you want to.
public Listeners(playerid, params[])
{
	//setting a new listener, which is waiting for "/cv"
	get_cmd_params(cv)
	{
	    new Float:x, Float:y, Float:z, Float:a;
		GetPlayerPos(playerid, x, y, z);
		GetPlayerFacingAngle(playerid, a);
		PutPlayerInVehicle(playerid, CreateVehicle(strval(params), x, y, z, a, -1, -1, cellmax), 0);

		//this return value doesn't means anything, but you've to use it to avoid
		//conflicts with others commands listeners.
		return 1;
	}
	
	//this return doesn't means anything, but you've to use it to avoid warnings.
	return 0;
}
