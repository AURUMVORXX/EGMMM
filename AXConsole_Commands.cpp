// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
	
	void AXConsole::init()
	{
		registerCommand("GOTO LOCATION", 0, &AXConsole::cmd_gotoLocation);
		registerCommand("GOTO NEWWORLD", 0, &AXConsole::cmd_gotoNewworld);
		registerCommand("GOTO OLDWORLD", 0, &AXConsole::cmd_gotoOldworld);
		registerCommand("GOTO ADDONWORLD", 0, &AXConsole::cmd_gotoAddonworld);
		registerCommand("GOTO DRAGONISLAND", 0, &AXConsole::cmd_gotoDragonisland);
		registerCommand("GOTO KOLONIE", 0, &AXConsole::cmd_gotoKolonie);

		zcon->AddEvalFunc(AXConsole_Eval);
	}

	// ----------------------------------------------------

	void AXConsole::cmd_gotoLocation(Array<CString> args, zSTRING& message)
	{
		CStringA worldName	= args.GetNum() >= 1 ? args[0] : "CHANGELEVEL.ZEN";
		CStringA startpoint = args.GetNum() >= 2 && !args[1].IsEmpty() ? args[1] : "START";

		zoptions->ChangeDir(DIR_WORLD);
		zFILE* world = zfactory->CreateZFile(worldName);

		if (world->Exists())
		{
			if (ogame->GetGameWorld()->GetWorldFilename() != zSTRING(worldName))
				ogame->ChangeLevel(worldName, startpoint);
		}
		else
			message = zSTRING(CStringA::Combine("[ERROR]: World \'%s\' doesn't exist!", worldName));

		delete world;
	}

	void AXConsole::cmd_gotoNewworld(Array<CString> args, zSTRING& message)
	{
		CStringA startpoint = args.GetNum() >= 1 && !args[1].IsEmpty() ? args[1] : "START";

		zSTRING worldName = "LOC_NEWWORLD.ZEN";

		args.Clear();
		args.Insert(worldName);
		args.Insert(startpoint);

		cmd_gotoLocation(args, message);
	}

	void AXConsole::cmd_gotoDragonisland(Array<CString> args, zSTRING& message)
	{
		CStringA startpoint = args.GetNum() >= 1 && !args[1].IsEmpty() ? args[1] : "START";

		zSTRING worldName = "LOC_DRAGONISLAND.ZEN";

		args.Clear();
		args.Insert(worldName);
		args.Insert(startpoint);

		cmd_gotoLocation(args, message);
	}

	void AXConsole::cmd_gotoOldworld(Array<CString> args, zSTRING& message)
	{
		CStringA startpoint = args.GetNum() >= 1 && !args[1].IsEmpty() ? args[1] : "START";

		zSTRING worldName = "LOC_OLDWORLD.ZEN";

		args.Clear();
		args.Insert(worldName);
		args.Insert(startpoint);

		cmd_gotoLocation(args, message);
	}

	void AXConsole::cmd_gotoAddonworld(Array<CString> args, zSTRING& message)
	{
		CStringA startpoint = args.GetNum() >= 1 && !args[1].IsEmpty() ? args[1] : "START";

		zSTRING worldName = "LOC_ADDONWORLD.ZEN";

		args.Clear();
		args.Insert(worldName);
		args.Insert(startpoint);

		cmd_gotoLocation(args, message);
	}

	void AXConsole::cmd_gotoKolonie(Array<CString> args, zSTRING& message)
	{
		CStringA startpoint = args.GetNum() >= 1 && !args[1].IsEmpty() ? args[1] : "START";

		zSTRING worldName = "LOC_COLONY.ZEN";

		args.Clear();
		args.Insert(worldName);
		args.Insert(startpoint);

		cmd_gotoLocation(args, message);
	}
}