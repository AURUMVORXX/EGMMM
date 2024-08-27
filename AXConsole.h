// Supported with union (c) 2020 Union team
// Union HEADER file

namespace GOTHIC_ENGINE {
	
	static zBOOL AXConsole_Eval(const zSTRING& s, zSTRING& msg);

	class AXConsole
	{
	private:

		typedef void(AXConsole::* commandFunction)(Array<CString>, zSTRING&);

		struct consoleCommand
		{
			CStringA name;
			uint argNum;
			commandFunction execFunction;
		};

		std::vector<consoleCommand> m_commands;

		AXConsole() {};
		AXConsole(const AXConsole&) = delete;
		AXConsole& operator=(const AXConsole&) = delete;

	public:

		static AXConsole& get() { static AXConsole instance; return instance; }

		void init();
		
		void registerCommand(const CStringA, uint, commandFunction);
		const int processCommand(CStringA&, zSTRING&);

		// ------------------------------

		void cmd_gotoLocation(Array<CString>, zSTRING&);
		void cmd_gotoNewworld(Array<CString>, zSTRING&);
		void cmd_gotoOldworld(Array<CString>, zSTRING&);
		void cmd_gotoAddonworld(Array<CString>, zSTRING&);
		void cmd_gotoDragonisland(Array<CString>, zSTRING&);
		void cmd_gotoKolonie(Array<CString>, zSTRING&);

		void cmd_gotoFocuswaypoint(Array<CString>, zSTRING&);

		void cmd_setSpeed(Array<CString>, zSTRING&);
		void cmd_setIgnorefocusitem(Array<CString>, zSTRING&);
		void cmd_setHand(Array<CString>, zSTRING&);
		void cmd_setFreeze(Array<CString>, zSTRING&);

		void cmd_clearHands(Array<CString>, zSTRING&);
		void cmd_clearBlood(Array<CString>, zSTRING&);

		void cmd_savePosition(Array<CString>, zSTRING&);
		void cmd_loadPosition(Array<CString>, zSTRING&);
		void cmd_loadFocusposition(Array<CString>, zSTRING&);

		void cmd_holdNpc(Array<CString>, zSTRING&);

		void cmd_playTrigger(Array<CString>, zSTRING&);
		void cmd_playFocusani(Array<CString>, zSTRING&);

		void cmd_deleteNpc(Array<CString>, zSTRING&);

		void cmd_createBlood(Array<CString>, zSTRING&);
	};

}