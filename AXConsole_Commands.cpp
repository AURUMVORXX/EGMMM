// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
	
	void AXConsole::init()
	{
		registerCommand("GOTO LOCATION", 0, &AXConsole::cmd_gotoLocation);

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
}