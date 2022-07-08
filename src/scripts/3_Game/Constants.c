const string SyberiaServer_ModPreffix = "[SYBERIA SERVER] ";
const bool SyberiaServer_DebugMode = true;

void SybLogSrv(string message)
{
	if (SyberiaServer_DebugMode) Print(SyberiaServer_ModPreffix + message);
}