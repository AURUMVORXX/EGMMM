// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {

	const CStringA SAVEDALIAS_LOCAL_FILENAME	= "EGMMM_ALIAS";
	const CStringA SAVEDALIAS_GLOBAL_FILENAME	= "EGMMM_ALIAS_GLOBAL";

	// ---------------------------------------------

	bool AXConsole::isAlias(const CStringA command)
	{
		for (auto cmd : m_aliases)
		{
			if (cmd.alias == command)
				return true;
		}

		return false;
	}

	void AXConsole::unregisterCommand(CStringA name)
	{
		zCConDat* command;

		for (int i = 0; i < zcon->list.Count(); i++)
		{
			command = zcon->list.Get(i);
			if (command != NULL && command->name == zSTRING(name))
				zcon->list.Remove(command);
		}
	}

	void AXConsole::registerCommand(CStringA name, uint numArgs, commandFunction func)
	{
		unregisterCommand(name);				// override existing command if needed

		zcon->Register(zSTRING(name), "");
		m_commands.push_back({name, numArgs, func});
	}

	void AXConsole::registerAlias(CStringA alias, std::vector<CStringA> commands, bool global)
	{
		zcon->Register(zSTRING(alias), "");

		consoleAlias newAlias{ alias, commands };

		if (!global)
			m_aliases.push_back(newAlias);
		else
		{
			m_aliasesG.push_back(newAlias);
			archiveAlias(-1, true);
		}
	}

	const int AXConsole::processCommand(CStringA& command, zSTRING& message)
	{
		command.Shrink(' ');

		if (!player)
		{
			message = zSTRING("[ERROR]: No valid player to execute the command");
			return FALSE;
		}

		for (auto cmd : m_commands)
		{
			if (command.StartWith(cmd.name))
			{
				command.Cut(0, cmd.name.Length()).Shrink(' ');		// cut command name from full command, leaving only command arguments
				Array<CString> args = command.Split(" ");			// split arguments into array by SPACE delimiter

				if (args.GetNum() >= cmd.argNum)
					(this->*cmd.execFunction)(args, message);
				else
					message = zSTRING(CStringA::Combine("[ERROR]: %s - wrong number of arguments (%i got, %i expected)", command, args.GetNum(), cmd.argNum));

				return TRUE;
			}
		}

		return FALSE;
	}

	bool AXConsole::processAlias(zSTRING& command)
	{
		CStringA strCommand = CStringA(command).Shrink(' ');

		// Local aliases have higher priority over global aliases
		for (auto cmd : m_aliases)
		{
			if (strCommand == cmd.alias)
			{
				for (CStringA aliasCommand : cmd.commands)
					zcon->Evaluate(zSTRING(aliasCommand));
				
				return true;
			}
		}

		for (auto cmd : m_aliasesG)
		{
			if (strCommand == cmd.alias)
			{
				for (CStringA aliasCommand : cmd.commands)
					zcon->Evaluate(zSTRING(aliasCommand));

				return true;
			}
		}

		return false;
	}

	void AXConsole::archiveAlias(int slot, bool global)
	{
		zSTRING targetDir = ogame->savegameManager->GetSlotDirName(slot);
		CStringA fileName = "";

		if (global)
			fileName = CStringA(CStringA(zoptions->GetDirString(DIR_SAVEGAMES)) + SAVEDALIAS_GLOBAL_FILENAME + ".SAV");
		else
			fileName = CStringA(CStringA(zoptions->GetDirString(DIR_SAVEGAMES)) + CStringA(targetDir) + SAVEDALIAS_LOCAL_FILENAME + ".SAV");

		zCArchiver* archiver = zarcFactory->CreateArchiverWrite(zSTRING(fileName), zARC_MODE_ASCII, TRUE, 0);

		if (global)
		{
			archiver->WriteInt("numEntries", int(m_aliasesG.size()));
			for (auto alias : m_aliasesG)
			{
				archiver->WriteString("aliasName", alias.alias);
				archiver->WriteInt("numCommands", int(alias.commands.size()));
				for (int i = 0; i < int(alias.commands.size()); i++)
					archiver->WriteString("command" + zSTRING(i), alias.commands[i]);
			}
		}
		else
		{
			archiver->WriteInt("numEntries", int(m_aliases.size()));
			for (auto alias : m_aliases)
			{
				archiver->WriteString("aliasName", alias.alias);
				archiver->WriteInt("numCommands", int(alias.commands.size()));
				for (int i = 0; i < int(alias.commands.size()); i++)
					archiver->WriteString("command" + zSTRING(i), alias.commands[i]);
			}
		}

		archiver->Close();
		zRELEASE(archiver);
	}

	void AXConsole::unarchiveAlias(int slot, bool global)
	{
		zSTRING targetDir = ogame->savegameManager->GetSlotDirName(slot);
		CStringA fileName = "";

		if (global)
			fileName = CStringA(CStringA(zoptions->GetDirString(DIR_SAVEGAMES)) + SAVEDALIAS_GLOBAL_FILENAME + ".SAV");
		else
			fileName = CStringA(CStringA(zoptions->GetDirString(DIR_SAVEGAMES)) + CStringA(targetDir) + SAVEDALIAS_LOCAL_FILENAME + ".SAV");

		zFILE* file = zfactory->CreateZFile(fileName);
		if (!file->Exists())
		{
			delete file;
			return;
		}

		zCArchiver* archiver = zarcFactory->CreateArchiverRead(fileName, 0);

		std::vector<CStringA> commands;
		zSTRING aliasName;
		int numCommands;

		int numEntries = archiver->ReadInt("numEntries");

		for (int i = 0; i < numEntries; i++)
		{
			aliasName = archiver->ReadString("aliasName");
			numCommands = archiver->ReadInt("numCommands");

			for (int j = 0; j < numCommands; j++)
				commands.push_back(archiver->ReadString("command" + zSTRING(j)));

			if (global)
				m_aliasesG.push_back({ aliasName, commands });
			else
				m_aliases.push_back({ aliasName, commands });

			zcon->Register(aliasName, "");
			commands.clear();
		}

		archiver->Close();
		zRELEASE(archiver);
	}

	void AXConsole::clearAlias(bool global)
	{
		if (global)
		{
			for (auto alias : m_aliasesG)
				unregisterCommand(alias.alias);

			m_aliasesG.clear();
		}
		else
		{
			for (auto alias : m_aliases)
				unregisterCommand(alias.alias);

			m_aliases.clear();
		}
	}

	// -----------------------------------------------------------
	static zBOOL AXConsole_Eval(const zSTRING& s, zSTRING& msg) {

		if (!zcon->GetWorld()) {
			msg = "ERROR: no zCWorld registered to zcon !";
			return FALSE;
		};

		msg = "ok";

		zSTRING& cmd = const_cast<zSTRING&>(s);

		if (AXConsole::get().processAlias(cmd))
			return TRUE;

		return AXConsole::get().processCommand(CStringA(s), msg);
	}
}