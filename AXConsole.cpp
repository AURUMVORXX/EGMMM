// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {

	void AXConsole::registerCommand(CStringA name, int numArgs, commandFunction func)
	{
		zcon->Register(zSTRING(name), "");
		m_commands.insert(std::make_pair(std::make_pair(name, numArgs), func));
	}

	const int AXConsole::processCommand(CStringA& command, zSTRING& message)
	{
		CString			commandName;
		int				commandArgNum;
		commandFunction commandFunc;

		command.Shrink(' ');

		for (auto element : m_commands)
		{
			commandName		= element.first.first;
			commandArgNum	= element.first.second;
			commandFunc		= element.second;

			if (command.StartWith(commandName))
			{
				command.Cut(0, commandName.Length()).Shrink(' ');	// cut command name from full command, leaving only command arguments
				Array<CString> args = command.Split(" ");			// split arguments into array by SPACE delimiter

				if (args.GetNum() >= commandArgNum)
					(this->*commandFunc)(args, message);
				else
					message = zSTRING(CStringA::Combine("%s - wrong number of arguments (%i got, %i expected)", command, args.GetNum(), commandArgNum));

				return TRUE;
			}
		}

		return FALSE;
	}

	// -----------------------------------------------------------
	static zBOOL AXConsole_Eval(const zSTRING& s, zSTRING& msg) {

		if (!zcon->GetWorld()) {
			msg = "ERROR: no zCWorld registered to zcon !";
			return FALSE;
		};

		msg = "ok";

		return AXConsole::get().processCommand(CStringA(s), msg);
	}
}