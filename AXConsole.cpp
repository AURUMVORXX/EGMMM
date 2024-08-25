// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {

	void AXConsole::init()
	{
		zcon->AddEvalFunc(AXConsole_Eval);
	}

	void AXConsole::registerCommand(CStringA name, int numArgs, commandFunction func)
	{
		zcon->Register(zSTRING(name), "");
		m_commands.insert(std::make_pair(std::make_pair(name, numArgs), func));
	}

	const int AXConsole::processCommand(CStringA& command, zSTRING& message)
	{
		command.Shrink(' ');

		for (auto element : m_commands)
		{
			if (command.StartWith(element.first.first))
			{
				CStringA argline = command;
				argline.Cut(0, element.first.first.Length()).Shrink(' ');
				Array<CString> arglist = argline.Split(" ");

				if (arglist.GetNum() >= element.first.second)
					(this->*element.second)(arglist, message);
				else
					message = zSTRING(CStringA::Combine("%s - wrong number of arguments (%i got, %i expected)", command, arglist.GetNum(), element.first.second));

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