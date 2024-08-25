// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {

	void AXConsole::init()
	{
		registerCommand("TEST COMMAND", 0, &AXConsole::testCommand);
		zcon->AddEvalFunc(AXConsole_Eval);
	}

	void AXConsole::registerCommand(CStringA name, int numArgs, commandFunction func)
	{
		zcon->Register(zSTRING(name), "");
		m_commands.insert(std::make_pair(std::make_pair(name, numArgs), func));
	}

	const int AXConsole::processCommand(CStringA command, zSTRING& message)
	{
		Message::Box("Y");
		for (auto element : m_commands)
		{
			if (element.first.first.StartWith(command.Upper()))
			{
				Message::Box("Found");
				CStringA argline = element.first.first;
				argline.Cut(command.Length(), argline.Length() - command.Length());
				Array<CString> arglist = argline.Split(" ");

				if (arglist.GetNum() >= element.first.second)
					(this->*element.second)(arglist, message);
				else
					message = zSTRING(CStringA::Combine("%s - not enough arguments (%d got, %d expected)", command, arglist.GetNum(), element.first.second));

				return TRUE;
			}
		}

		return FALSE;
	}

	void AXConsole::testCommand(Array<CStringA> args, zSTRING& msg)
	{
		msg = "Rabotaet!";
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