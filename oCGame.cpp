// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
	
	const zSTRING SAVEDPOSITIONS_FILENAME = "EGMMM_POSITIONS.SAV";

	// ------------------------------------------

	void oCGame::setIgnoreFocusItems(bool toggle)	{ oCNpcFocus::focus->i_prio = toggle ? -1 : 1; }
	const bool oCGame::isIgnoreFocusItems()			{ return oCNpcFocus::focus->i_prio == -1; }

	void oCGame::setInfiniteRings(bool toggle)		{ m_infiniteRings = toggle; }
	const bool oCGame::isInfiniteRings()			{ return m_infiniteRings; }

	void oCGame::setHoldTime(bool toggle)			{ m_holdTime = toggle; }
	const bool oCGame::isHoldTime()					{ return m_holdTime; }

	template <typename T>
	T oCGame::findSavedPosition(CStringA positionName)
	{
		CStringA elementName;
		CStringA elementWorld;
		zSTRING worldName = GetGameWorld()->GetWorldName();

		for (auto it = V_SavedPosition.begin(); it != V_SavedPosition.end(); it++)
		{
			elementName = it->name;
			elementWorld = it->worldName;

			if (elementName == positionName && zSTRING(elementWorld) == worldName)
				return it;
		}

		return V_SavedPosition.end();
	}

	void oCGame::addSavedPosition(CStringA positionName, oCNpc* savedNpc)
	{
		zSTRING worldName	= GetGameWorld()->GetWorldName();

		removeSavedPosition(positionName);
		V_SavedPosition.push_back(positionEntry{ worldName, positionName, savedNpc->trafoObjToWorld });
	}

	void oCGame::removeSavedPosition(CStringA positionName)
	{
		auto foundElement = findSavedPosition<std::vector<positionEntry>::iterator>(positionName);

		if (foundElement != V_SavedPosition.end())
			V_SavedPosition.erase(foundElement);
	}

	zMAT4 oCGame::getSavedPosition(CStringA positionName)
	{
		auto position = findSavedPosition<std::vector<positionEntry>::iterator>(positionName);
		return position != V_SavedPosition.end() ? position->trafoObjToWorld : zMAT4(0);
	}

	std::map<CStringA, int> oCGame::getSavedPositionsEntries()
	{
		std::map<CStringA, int>				entries;
		std::map<CStringA, int>::iterator	foundEntry;

		for (auto entry : V_SavedPosition)
		{
			foundEntry = entries.find(entry.worldName);

			if (foundEntry != entries.end())
				foundEntry->second++;
			else
				entries.insert(std::make_pair(entry.worldName, 1));
		}

		return entries;
	}

	void oCGame::archiveSavedPositions(int slot)
	{
		zSTRING targetDir = savegameManager->GetSlotDirName(slot);

		zCArchiver* archiver = zarcFactory->CreateArchiverWrite(zoptions->GetDirString(DIR_SAVEGAMES) + targetDir + SAVEDPOSITIONS_FILENAME, zARC_MODE_BINARY_SAFE, TRUE, 0);

		std::sort(V_SavedPosition.begin(), V_SavedPosition.end());
		std::map<CStringA, int> entries = getSavedPositionsEntries();

		CStringA currentWorld = "";
		zMAT4 trafo;

		archiver->WriteInt("numWorlds", int(entries.size()));

		for (auto position : V_SavedPosition)
		{
			if (position.worldName != currentWorld)
			{
				currentWorld = position.worldName;
				archiver->WriteString("world", zSTRING(currentWorld));
				archiver->WriteInt("numEntries", entries.at(currentWorld));
			}

			trafo = position.trafoObjToWorld;

			archiver->WriteString("name", position.name);

			archiver->WriteFloat("x", trafo.GetTranslation()[0]);
			archiver->WriteFloat("y", trafo.GetTranslation()[1]);
			archiver->WriteFloat("z", trafo.GetTranslation()[2]);

			archiver->WriteFloat("x", trafo.GetAtVector()[0]);
			archiver->WriteFloat("y", trafo.GetAtVector()[1]);
			archiver->WriteFloat("z", trafo.GetAtVector()[2]);

			archiver->WriteFloat("x", trafo.GetUpVector()[0]);
			archiver->WriteFloat("y", trafo.GetUpVector()[1]);
			archiver->WriteFloat("z", trafo.GetUpVector()[2]);

			archiver->WriteFloat("x", trafo.GetRightVector()[0]);
			archiver->WriteFloat("y", trafo.GetRightVector()[1]);
			archiver->WriteFloat("z", trafo.GetRightVector()[2]);
		}

		archiver->Close();
		zRELEASE(archiver);
	}

	void oCGame::unarchiveSavedPositions(int slot)
	{
		zSTRING targetDir = savegameManager->GetSlotDirName(slot);

		zSTRING saveFile = zoptions->GetDirString(DIR_SAVEGAMES) + targetDir + SAVEDPOSITIONS_FILENAME;
		zFILE* file = zfactory->CreateZFile(saveFile);
		if (!file->Exists())
		{
			delete file;
			return;
		}

		zCArchiver* archiver = zarcFactory->CreateArchiverRead(zoptions->GetDirString(DIR_SAVEGAMES) + targetDir + SAVEDPOSITIONS_FILENAME, 0);

		int numWorlds = archiver->ReadInt("numWorlds");
		
		zSTRING currentWorld,  positionName;
		int numEntries;
		zMAT4 trafo(0);
		float x, y, z;

		for (int i = 0; i < numWorlds; i++)
		{
			currentWorld		= archiver->ReadString("world");
			numEntries			= archiver->ReadInt("numEntries");

			for (int j = 0; j < numEntries; j++)
			{
				positionName	= archiver->ReadString("name");

				// Translation
				x				= archiver->ReadFloat("x");
				y				= archiver->ReadFloat("y");
				z				= archiver->ReadFloat("z");
				trafo.SetTranslation(zVEC3(x, y, z));

				// At Vector
				x = archiver->ReadFloat("x");
				y = archiver->ReadFloat("y");
				z = archiver->ReadFloat("z");
				trafo.SetAtVector(zVEC3(x, y, z));

				// Up Vector
				x = archiver->ReadFloat("x");
				y = archiver->ReadFloat("y");
				z = archiver->ReadFloat("z");
				trafo.SetUpVector(zVEC3(x, y, z));

				// Right Vector
				x = archiver->ReadFloat("x");
				y = archiver->ReadFloat("y");
				z = archiver->ReadFloat("z");
				trafo.SetRightVector(zVEC3(x, y, z));

				V_SavedPosition.push_back(positionEntry{ currentWorld, positionName, trafo});
			}
		}

		archiver->Close();
		zRELEASE(archiver);
	}

	// ------------------------------------------

	HOOK Ivk_oCGame_ShowDebugInfos AS(&oCGame::ShowDebugInfos, &oCGame::ShowDebugInfos_IVK);
	void oCGame::ShowDebugInfos_IVK()
	{
		int savedShowTime = game_showtime;
		game_showtime = FALSE;

		THISCALL(Ivk_oCGame_ShowDebugInfos)();

		game_showtime = savedShowTime;
		
		if (game_showtime)
		{
			zSTRING timeString		= GetWorldTimer()->GetTimeString();
			int stringWidth			= screen->FontSize(timeString);

			if (isHoldTime())
				screen->SetFontColor(zCOLOR(255, 255, 0));

			screen->Print(8191 - stringWidth, 0, timeString);
			screen->SetFontColor(zCOLOR(255, 255, 255));
		}
	}

	HOOK Ivk_oCGame_UpdatePlayerStatus AS(&oCGame::UpdatePlayerStatus, &oCGame::UpdatePlayerStatus_IVK);
	void oCGame::UpdatePlayerStatus_IVK()
	{
		if (!player) return;

		oCNpc* focusNpc = player->GetFocusNpc();

		if (focusNpc && focusNpc->isHolded())
			screen->SetFontColor(zCOLOR(255, 115, 0));

		THISCALL(Ivk_oCGame_UpdatePlayerStatus)();
		screen->SetFontColor(zCOLOR(255, 255, 255));
	}

	HOOK Ivk_oCGame_WriteSavegame AS(&oCGame::WriteSavegame, &oCGame::WriteSavegame_IVK);
	void oCGame::WriteSavegame_IVK(int slot, zBOOL saveGlobals)
	{
		THISCALL(Ivk_oCGame_WriteSavegame)(slot, saveGlobals);

		if (saveGlobals) archiveSavedPositions(slot);
	}

	HOOK Ivk_oCGame_LoadSavegame AS(&oCGame::LoadSavegame, &oCGame::LoadSavegame_IVK);
	void oCGame::LoadSavegame_IVK(int slot, zBOOL loadGlobals)
	{
		THISCALL(Ivk_oCGame_LoadSavegame)(slot, loadGlobals);

		if (loadGlobals) unarchiveSavedPositions(slot);
	}
}