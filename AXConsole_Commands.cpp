// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
	
	const float motionFactorMin = 0.1f;
	const float motionFactorMax = 5.0f;

	// ----------------------------------------------------

	void AXConsole::init()
	{
		registerCommand("GOTO LOCATION",		0, &AXConsole::cmd_gotoLocation);
		registerCommand("GOTO NEWWORLD",		0, &AXConsole::cmd_gotoNewworld);
		registerCommand("GOTO OLDWORLD",		0, &AXConsole::cmd_gotoOldworld);
		registerCommand("GOTO ADDONWORLD",		0, &AXConsole::cmd_gotoAddonworld);
		registerCommand("GOTO DRAGONISLAND",	0, &AXConsole::cmd_gotoDragonisland);
		registerCommand("GOTO KOLONIE",			0, &AXConsole::cmd_gotoKolonie);

		registerCommand("GOTO FOCUSWAYPOINT",	1, &AXConsole::cmd_gotoFocuswaypoint);

		registerCommand("SET SPEED",			1, &AXConsole::cmd_setSpeed);
		registerCommand("SET IGNOREFOCUSITEM",	0, &AXConsole::cmd_setIgnorefocusitem);
		registerCommand("SET HAND",				1, &AXConsole::cmd_setHand);
		registerCommand("SET FREEZE",			0, &AXConsole::cmd_setFreeze);

		registerCommand("CLEAR HANDS",			0, &AXConsole::cmd_clearHands);

		registerCommand("SAVE POSITION",		1, &AXConsole::cmd_savePosition);
		registerCommand("LOAD POSITION",		1, &AXConsole::cmd_loadPosition);
		registerCommand("LOAD FOCUSPOSITION",	1, &AXConsole::cmd_loadFocusposition);

		registerCommand("HOLDNPC",				0, &AXConsole::cmd_holdNpc);

		registerCommand("PLAY TRIGGER",			1, &AXConsole::cmd_playTrigger);
		registerCommand("PLAY FOCUSANI",		1, &AXConsole::cmd_playFocusani);

		zcon->AddEvalFunc(AXConsole_Eval);
	}

	// ----------------------------------------------------

	void AXConsole::cmd_gotoLocation(Array<CString> args, zSTRING& message)
	{
		CStringA worldName	= args.GetNum() >= 1 ? args[0] : "CHANGELEVEL.ZEN";					// string - world path relative to 'Worlds/' folder (including .zen)
		CStringA startpoint = args.GetNum() >= 2 && !args[1].IsEmpty() ? args[1] : "START";		// string - WP/FP name

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
		CStringA startpoint = args.GetNum() >= 1 && !args[1].IsEmpty() ? args[1] : "START";		// string - WP/FP name

		zSTRING worldName = "LOC_NEWWORLD.ZEN";

		args.Clear();
		args.Insert(worldName);
		args.Insert(startpoint);

		cmd_gotoLocation(args, message);
	}

	void AXConsole::cmd_gotoDragonisland(Array<CString> args, zSTRING& message)
	{
		CStringA startpoint = args.GetNum() >= 1 && !args[1].IsEmpty() ? args[1] : "START";		// string - WP/FP name

		zSTRING worldName = "LOC_DRAGONISLAND.ZEN";

		args.Clear();
		args.Insert(worldName);
		args.Insert(startpoint);

		cmd_gotoLocation(args, message);
	}

	void AXConsole::cmd_gotoOldworld(Array<CString> args, zSTRING& message)
	{
		CStringA startpoint = args.GetNum() >= 1 && !args[1].IsEmpty() ? args[1] : "START";		// string - WP/FP name

		zSTRING worldName = "LOC_OLDWORLD.ZEN";

		args.Clear();
		args.Insert(worldName);
		args.Insert(startpoint);

		cmd_gotoLocation(args, message);
	}

	void AXConsole::cmd_gotoAddonworld(Array<CString> args, zSTRING& message)
	{
		CStringA startpoint = args.GetNum() >= 1 && !args[1].IsEmpty() ? args[1] : "START";		// string - WP/FP name

		zSTRING worldName = "LOC_ADDONWORLD.ZEN";

		args.Clear();
		args.Insert(worldName);
		args.Insert(startpoint);

		cmd_gotoLocation(args, message);
	}

	void AXConsole::cmd_gotoKolonie(Array<CString> args, zSTRING& message)
	{
		CStringA startpoint = args.GetNum() >= 1 && !args[1].IsEmpty() ? args[1] : "START";		// string - WP/FP name

		zSTRING worldName = "LOC_COLONY.ZEN";

		args.Clear();
		args.Insert(worldName);
		args.Insert(startpoint);

		cmd_gotoLocation(args, message);
	}

	void AXConsole::cmd_gotoFocuswaypoint(Array<CString> args, zSTRING& message)
	{
		if (!player) return;

		CStringA waypoint	= args[0];				// string - WP/FP name
		oCNpc* focusNpc		= player->GetFocusNpc();
		
		focusNpc->BeamTo(waypoint);
	}

	void AXConsole::cmd_setSpeed(Array<CString> args, zSTRING& message)
	{
		float motionFactor = args[0].ToReal32();	// float

		motionFactor < motionFactorMin ? motionFactor = motionFactorMin : false;
		motionFactor > motionFactorMax ? motionFactor = motionFactorMax : false;

		ztimer->factorMotion = motionFactor;
	}

	void AXConsole::cmd_setIgnorefocusitem(Array<CString> args, zSTRING& message)
	{

	}

	void AXConsole::cmd_setHand(Array<CString> args, zSTRING& message)
	{
		CStringA hand = args[0];					// string - `LEFT` / `RIGHT`

		if (!player) return;

		zCVob* focusVob = player->GetFocusVob();

		if (focusVob)
		{
			oCItem* item = zDYNAMIC_CAST<oCItem>(focusVob);

			if (item)
			{
				ogame->GetGameWorld()->DisableVob(item);
				hand == "LEFT" ? player->SetLeftHand(item) : player->SetRightHand(item);
			}
		}
	}

	void AXConsole::cmd_setFreeze(Array<CString> args, zSTRING& message)
	{
		if (!player) return;

		oCNpc* focusNpc = player->GetFocusNpc();
		if (!focusNpc) focusNpc = player;

		focusNpc->sleepingMode == 0 ? focusNpc->SetSleeping(FALSE) : focusNpc->SetSleeping(TRUE);
	}

	void AXConsole::cmd_clearHands(Array<CString> args, zSTRING& message)
	{
		if (!player) return;

		player->RemoveFromSlot(NPC_NODE_LEFTHAND, TRUE, FALSE);
		player->RemoveFromSlot(NPC_NODE_RIGHTHAND, TRUE, FALSE);
	}

	void AXConsole::cmd_savePosition(Array<CString> args, zSTRING& message)
	{

	}

	void AXConsole::cmd_loadPosition(Array<CString> args, zSTRING& message)
	{

	}

	void AXConsole::cmd_loadFocusposition(Array<CString> args, zSTRING& message)
	{

	}

	void AXConsole::cmd_holdNpc(Array<CString> args, zSTRING& message)
	{

	}

	void AXConsole::cmd_playTrigger(Array<CString> args, zSTRING& message)
	{
		zSTRING vobName		= args[0];					// string

		zCVob* targetVob	= ogame->GetWorld()->SearchVobByName(vobName);
		if (!targetVob) return;

		targetVob->GetEM(0)->OnTrigger(targetVob, targetVob);
	}

	void AXConsole::cmd_playFocusani(Array<CString> args, zSTRING& message)
	{
		if (!player) return;

		CStringA aniName	= args[0];					// string - animation name

		oCNpc* focusNpc		= player->GetFocusNpc();
		if (!focusNpc) return;

		int aniId = focusNpc->GetModel()->GetAniIDFromAniName(aniName);
		if (aniId == -1)
		{
			message = zSTRING(CStringA::Combine("[ERROR]: Animation %s not found", aniName));
			return;
		}
		
		focusNpc->GetModel()->StartAni(aniId, FALSE);
	}
}