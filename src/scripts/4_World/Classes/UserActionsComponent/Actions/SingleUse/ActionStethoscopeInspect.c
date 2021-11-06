modded class ActionStethoscopeInspect
{
	override void OnEndServer( ActionData action_data )
	{
		PlayerBase ntarget = PlayerBase.Cast( action_data.m_Target.GetObject() );
		if (!ntarget) return;
		if (ntarget.IsGhostBody()) return;
		if (!ntarget.m_charProfile) return;
		
		ref ActionStethoscopeInspect_Data data = new ActionStethoscopeInspect_Data;
		data.m_health = ntarget.GetHealth("GlobalHealth", "Health");
		data.m_blood = ntarget.GetHealth("GlobalHealth", "Blood");
		data.m_food = ntarget.GetStatEnergy().Get();
		data.m_water = ntarget.GetStatWater().Get();
		data.m_temperature = ntarget.GetStatHeatComfort().Get();
		data.m_sleeping = ntarget.m_sleepingValue;
		data.m_mind = ntarget.m_mindStateValue;
		data.m_overdose = ntarget.m_overdosedValue;
		data.m_cutWounds = ntarget.m_BleedingManagerServer.GetBleedingSourcesCount();
		data.m_knifeWounds = ntarget.GetSybStats().m_knifeHits;
		data.m_bulletWounds = ntarget.GetSybStats().m_bulletHits;
		data.m_hematomaWounds = ntarget.GetSybStats().m_hematomaHits;
		data.m_pain = ntarget.GetCurrentPainLevel();
		data.m_influenza = ntarget.GetSybStats().m_influenzaLevel;
		data.m_stomatchPoison = ntarget.GetSybStats().m_stomatchpoisonLevel;
		data.m_brokenLegs = (ntarget.m_BrokenLegState > 0);
		data.m_viscera = (ntarget.GetSybStats().m_visceraHit > 0);
		data.m_sepsis = ntarget.HasVisibleSepsis();
		data.m_concussion = ntarget.GetSybStats().m_concussionHit;
		data.m_virusZ = ntarget.HasVisibleZVirus();
		data.m_radiationSickness = ntarget.GetRadiationSicknessLevel();
		
		if (!ntarget.IsAlive())
		{
			data.m_health = -1;
		}
		
		GetSyberiaRPC().SendToClient(SyberiaRPC.SYBRPC_STETHOSCOPE_INSPECT, action_data.m_Player.GetIdentity(), new Param1<ref ActionStethoscopeInspect_Data>(data));
		delete data;
	}
};