// Supported with union (c) 2020 Union team
// Union HEADER file

namespace GOTHIC_ENGINE {
	
	static zBOOL AXConsole_Eval(const zSTRING& s, zSTRING& msg);

	class AXConsole
	{
	private:

		typedef void(AXConsole::* commandFunction)(Array<CString>, zSTRING&);
		std::map<std::pair<CStringA, int>, commandFunction> m_commands;

		AXConsole() {};
		AXConsole(const AXConsole&) = delete;
		AXConsole& operator=(const AXConsole&) = delete;

	public:

		static AXConsole& get() { static AXConsole instance; return instance; }

		void init();
		
		void registerCommand(const CStringA, int, commandFunction);
		const int processCommand(CStringA&, zSTRING&);

	};

}