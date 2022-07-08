modded class Environment
{
	override protected float GetEnvironmentTemperature()
	{
		float temperature = super.GetEnvironmentTemperature();

		if (IsWinter())
		{			
			vector player_pos = m_Player.GetPosition();
			string surface_type = "";
			GetGame().SurfaceGetType(player_pos[0], player_pos[2], surface_type);
			
			if (surface_type == "nam_seaice" || surface_type == "nam_lakeice_ext") 
			{
				temperature += GetSyberiaConfig().m_temperatureIceDrain;
			}
			else if (surface_type == "nam_snow") 
			{
				temperature += GetSyberiaConfig().m_temperatureSnowDrain;
			}
		}

		return temperature;
	}
	
		override void AddToEnvironmentTemperature(float pTemperature)
	{
		m_HeatSourceTemp = Math.Max(m_HeatSourceTemp, pTemperature);
	}
	
	override protected bool IsRaining()
	{
		return !IsWinter() && super.IsRaining();
	}
	
	override protected float GetTemperatureHeightCorrection()
	{
		float temperature_reduction = Math.Max(0, (m_PlayerHeightPos * GetSyberiaConfig().m_temperatureHeightReduction));
		return temperature_reduction;
	}
	
	bool IsWinter()
	{
		return GetSyberiaConfig().m_isWinterMap;
	}
	
	override protected void BodyPartHeatProperties(array<int> pBodyPartIds, float pCoef, out float pHeatComfort, out float pHeat)
	{
		int attCount;
		
		EntityAI attachment;
		ItemBase item;
		
		pHeat = 0;
		pHeatComfort = -1;
		attCount = m_Player.GetInventory().AttachmentCount();
		
		float envTemperature = 0;
		Mission mission = GetGame().GetMission();
		if (mission && mission.GetWorldData())
		{
			envTemperature = mission.GetWorldData().GetBaseEnvTemperature();
		}
		
		for ( int attIdx = 0; attIdx < attCount; attIdx++ )
		{
			attachment = m_Player.GetInventory().GetAttachmentFromIndex( attIdx );
			if ( attachment.IsClothing() )
			{
				item = ItemBase.Cast( attachment );
				int attachmentSlot = attachment.GetInventory().GetSlotId( 0 );

				//! go through all body parts we've defined for that zone (ex.: head, body, feet)
				for ( int i = 0; i < pBodyPartIds.Count(); i++ )
				{
					if ( attachmentSlot == pBodyPartIds.Get( i ) )
					{
						bool fullIsolation = false; 
						float heatIsoMult = GetSyberiaConfig().m_clothingHeatIsolationGlobal;
						if ( attachmentSlot == InventorySlots.VEST )
						{
							heatIsoMult = GameConstants.ENVIRO_HEATISOLATION_VEST_WEIGHT;
							fullIsolation = true;
						}
						else if ( attachmentSlot == InventorySlots.BACK )
						{
							heatIsoMult = GameConstants.ENVIRO_HEATISOLATION_BACK_WEIGHT;
							fullIsolation = true;
						}
						
						if (!fullIsolation)
						{
							if (item.GetWet() > GameConstants.STATE_DRENCHED)
							{
								pHeat += GetSyberiaConfig().m_clothingDrenchedHeatDrain;
							}
							else if (item.GetWet() > GameConstants.STATE_SOAKING_WET)
							{
								pHeat += GetSyberiaConfig().m_clothingWetHeatDrain;
							}
						}
						
						pHeatComfort += heatIsoMult * ( MiscGameplayFunctions.GetCurrentItemHeatIsolation( item ) );
						
						// go through any attachments and cargo (only current level, ignore nested containers - they isolate)
						int inAttCount = item.GetInventory().AttachmentCount();
						if ( inAttCount > 0 )
						{
							for ( int inAttIdx = 0; inAttIdx < inAttCount; inAttIdx++ )
							{
								EntityAI inAttachment = item.GetInventory().GetAttachmentFromIndex( inAttIdx );
								ItemBase itemAtt = ItemBase.Cast( inAttachment );
								if ( itemAtt )
								{
									if (fullIsolation)
									{
										ProcessItemHeat(itemAtt, 1, envTemperature);
									}
									else
									{
										pHeat += itemAtt.GetTemperature();
										ProcessItemHeat(itemAtt, 1, GameConstants.TEMPERATURE_ITEMS_HEAT_IN_INVENTORY_FROM_BODY);
									}
								}
							}
						}
						if ( item.GetInventory().GetCargo() )
						{
							int inItemCount = item.GetInventory().GetCargo().GetItemCount();
							
							for ( int j = 0; j < inItemCount; j++ )
							{
								ItemBase inItem;
								if ( Class.CastTo( inItem, item.GetInventory().GetCargo().GetItem( j ) ) )
								{
									if (fullIsolation)
									{
										ProcessItemHeat(inItem, 1, envTemperature);
									}
									else
									{
										pHeat += inItem.GetTemperature();
										ProcessItemHeat(inItem, 1, GameConstants.TEMPERATURE_ITEMS_HEAT_IN_INVENTORY_FROM_BODY);
									}
								}
							}
						}
					}
				}
			}
		}

		pHeatComfort = ( pHeatComfort / pBodyPartIds.Count() ) * pCoef;
		pHeat = ( pHeat / pBodyPartIds.Count() ) * pCoef;
		pHeat += m_Player.GetPerkFloatValue(SyberiaPerkType.SYBPERK_SURVIVAL_FROST_RESIST, 0, 0) * GetSyberiaConfig().m_additionalTemperatureResistForSurvivalSkill;
	}
	
	protected void ProcessItemHeat(ItemBase item, int depth, float envTemperature)
	{
		if (item.IsTemperatureVisible())
		{
			float itemTemperature = item.GetTemperature();
			if (itemTemperature > envTemperature) item.AddTemperature( GameConstants.TEMPERATURE_ITEM_HEAT_TRANSFER_COEF * -1.0 );
			else item.AddTemperature( GameConstants.TEMPERATURE_ITEM_HEAT_TRANSFER_COEF );
		}
		
		if (!item.GetInventory())
			return;
		
		int inAttCount = item.GetInventory().AttachmentCount();
		if ( inAttCount > 0 )
		{
			for ( int inAttIdx = 0; inAttIdx < inAttCount; inAttIdx++ )
			{
				EntityAI inAttachment = item.GetInventory().GetAttachmentFromIndex( inAttIdx );
				ItemBase itemAtt = ItemBase.Cast( inAttachment );
				if ( itemAtt )
				{
					ProcessItemHeat(itemAtt, depth + 1, envTemperature);
				}
			}
		}
		
		if ( item.GetInventory().GetCargo() )
		{
			int inItemCount = item.GetInventory().GetCargo().GetItemCount();			
			for ( int j = 0; j < inItemCount; j++ )
			{
				ItemBase inItem;
				if ( Class.CastTo( inItem, item.GetInventory().GetCargo().GetItem( j ) ) )
				{
					ProcessItemHeat(inItem, depth + 1, envTemperature);
				}
			}
		}
	}
};