const string SyberiaServer_ModPreffix = "[SYBERIA SERVER] ";
const bool SyberiaServer_DebugMode = true;

void SybLogSrv(string message)
{
	if (SyberiaServer_DebugMode) Print(SyberiaServer_ModPreffix + message);
}

const float DIRTY_SYRINGE_BLOOD_INFECTION_CHANCE = 0.2;