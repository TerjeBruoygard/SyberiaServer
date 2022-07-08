class CfgPatches {
	class SyberiaServer {
        units[] = {};
		requiredAddons[] = {"DZ_Data", "DZ_Scripts", "SyberiaScripts"};
	};
};

class CfgMods {
	class SyberiaServer {
		type = "mod";

		class defs {
			class gameScriptModule {
				value = "";
				files[] = {"SyberiaServer/scripts/3_Game"};
			};

			class worldScriptModule {
				value = "";
				files[] = {"SyberiaServer/scripts/4_World"};
			};

			class missionScriptModule {
				value = "";
				files[] = {"SyberiaServer/scripts/5_Mission"};
			};

			class imageSets {
				files[] = {};
			};
		};
	};
};