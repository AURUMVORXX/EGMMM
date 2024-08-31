// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {

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
		zCConDat* element = zcon->list.Get(0);
		while (element != NULL)
		{
			if (element->name == zSTRING(name))
			{
				zcon->list.Remove(element);
				return;
			}

			element = element->next;
		}
	}

	void AXConsole::registerCommand(CStringA name, uint numArgs, commandFunction func)
	{
		unregisterCommand(name);				// override existing command if needed

		zcon->Register(zSTRING(name), "");
		m_commands.push_back({name, numArgs, func});
	}

	void AXConsole::registerAlias(CStringA alias, CStringA command)
	{
		zcon->Register(zSTRING(alias), "");
		m_aliases.push_back({alias, command});
	}

	const int AXConsole::processCommand(CStringA& command, zSTRING& message)
	{
		command.Shrink(' ');

		if (!player)
		{
			message = zSTRING("[ERROR]: No valid player to execute the command");
			return;
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

	void AXConsole::processAlias(zSTRING& command)
	{
		CStringA strCommand = CStringA(command).Shrink(' ');

		for (auto cmd : m_aliases)
		{
			if (strCommand == cmd.alias)
			{
				command = zSTRING(cmd.original);
				return;
			}
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
		AXConsole::get().processAlias(cmd);

		return AXConsole::get().processCommand(CStringA(s), msg);
	}
}