modded class ActionBandageBase: ActionContinuousBase
{
	override void ApplyBandage( ItemBase item, PlayerBase player )
	{	
		player.ApplyAdvMedicineItem(item.GetType(), 1.0);
		PluginTransmissionAgents m_mta = PluginTransmissionAgents.Cast(GetPlugin(PluginTransmissionAgents));
		m_mta.TransmitAgents(item, player, AGT_ITEM_TO_FLESH);
		
		if (item.HasQuantity())
		{
			item.AddQuantity(-1,true);
		}
		else
		{
			item.Delete();
		}
	}
};