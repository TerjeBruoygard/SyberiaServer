modded class ActionCutBookPage
{
	override void OnFinishProgressServer( ActionData action_data )
	{
		float decreaseHealth = action_data.m_MainItem.GetMaxHealth() / GetSyberiaConfig().m_bookPagesCountToCut;
		action_data.m_MainItem.AddHealth("", "", decreaseHealth * -1);
		
		Paper paper = Paper.Cast( GetGame().CreateObjectEx("Paper", action_data.m_Player.GetPosition(), ECE_PLACE_ON_SURFACE) );
		paper.SetQuantity(1, false);
		
		SyberiaSoundEmitter.Spawn("CutPage_SoundEmitter", action_data.m_Player.GetPosition());
	}
};