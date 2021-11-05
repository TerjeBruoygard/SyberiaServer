modded class AITargetCallbacksPlayer
{
    override float GetMaxVisionRangeModifier(EntityAI pApplicant)
	{ 
		HumanMovementState state = new HumanMovementState;
		m_Player.GetMovementState(state);
		
		float mod = 1.0;
		float speedCoef = PlayerConstants.AI_VISIBILITY_RUN;
		float stanceCoef = PlayerConstants.AI_VISIBILITY_STANDING;
		float playerVisCoef = m_Player.GetVisibilityCoef();
        
		//! player speed mofifications
		switch(AITargetCallbacksPlayer.StanceToMovementIdxTranslation(state))
		{
			case DayZPlayerConstants.MOVEMENTIDX_WALK:
				speedCoef = PlayerConstants.AI_VISIBILITY_WALK;
				break;
				
			case DayZPlayerConstants.MOVEMENTIDX_IDLE:
				speedCoef = PlayerConstants.AI_VISIBILITY_IDLE;
				break;
		}
		
		//! stance modification
		switch(state.m_iStanceIdx)
		{
			case DayZPlayerConstants.STANCEIDX_CROUCH:
			case DayZPlayerConstants.STANCEIDX_RAISEDCROUCH:
				stanceCoef = PlayerConstants.AI_VISIBILITY_CROUCH;
				break;
				
			case DayZPlayerConstants.STANCEIDX_PRONE:
			case DayZPlayerConstants.STANCEIDX_RAISEDPRONE:
				stanceCoef = PlayerConstants.AI_VISIBILITY_PRONE;
				break;
		}
		
		//! mean value of the coefs
		mod = ((speedCoef + stanceCoef) / 2) * playerVisCoef;
		
		return mod;
	}

};