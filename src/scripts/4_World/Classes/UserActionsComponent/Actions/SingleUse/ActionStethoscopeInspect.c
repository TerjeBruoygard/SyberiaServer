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
		data.m_knifeWounds = ntarget.m_knifeHits;
		data.m_bulletWounds = ntarget.m_bulletHits;
		data.m_hematomaWounds = ntarget.m_hematomaHits;
		data.m_pain = ntarget.GetCurrentPainLevel();
		data.m_influenza = ntarget.m_influenzaLevel;
		data.m_stomatchPoison = ntarget.m_stomatchpoisonLevel;
		data.m_brokenLegs = (ntarget.m_BrokenLegState > 0);
		data.m_viscera = (ntarget.m_visceraHit > 0);
		data.m_sepsis = ntarget.HasVisibleSepsis();
		data.m_concussion = ntarget.m_concussionHit;
		data.m_virusZ = ntarget.HasVisibleZVirus();
		
		if (!ntarget.IsAlive())
		{
			data.m_health = -1;
		}
		
		GetSyberiaRPC().SendToClient(SyberiaRPC.SYBRPC_STETHOSCOPE_INSPECT, action_data.m_Player.GetIdentity(), new Param1<ref ActionStethoscopeInspect_Data>(data));
		delete data;
	}
};