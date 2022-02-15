modded class PluginLogicPDA
{	
	ref PdaServerConfig m_serverConfig;
	string confPath = "$profile:Syberia\\PdaConfig.json";
	
	override void OnInit()
	{
		super.OnInit();
		
		MakeDirectory("$profile:Syberia");
		if (!FileExist(confPath))
		{
			m_serverConfig = new PdaServerConfig();
			JsonFileLoader<ref PdaServerConfig>.JsonSaveFile(confPath, m_serverConfig);
		}
		else
		{
			JsonFileLoader<ref PdaServerConfig>.JsonLoadFile(confPath, m_serverConfig);
		}
	}
	
	override void SendGlobalMessage( ParamsReadContext ctx, PlayerIdentity sender )
	{
        Param1< string > serverData;
        if ( !ctx.Read( serverData ) ) return;
        
        if (!sender) return;
        
        if (!m_serverConfig.m_enableGlobalChat) return;
        
		int errorCode = 0;
		ref CharProfile profile = GetSyberiaCharacters().Get(sender, errorCode, true);
		if (!profile) return;
		
        GetSyberiaRPC().SendToAll( SyberiaRPC.SYBRPC_PDA_SEND_GLOBAL_MESSAGE, new Param2< string, string >( profile.m_name, serverData.param1 ) );
	}
	
	override void SendGroupMessage( ParamsReadContext ctx, PlayerIdentity sender )
	{
        Param1< string > serverData;			
        if ( !ctx.Read( serverData ) ) return;
        
        if (!sender) return;

		int errorCode = 0;        
		ref CharProfile profile = GetSyberiaCharacters().Get(sender, errorCode, true);
		if (!profile) return;
		
        ref PluginSyberiaOptions_GroupFaction group = GetSyberiaOptions().FindGroupByMember(profile.m_id);
        if (!group) return;
        
        foreach (ref SyberiaPdaGroupMember member : group.m_members)
        {
			ref PlayerBase memberPlayer = GetPlayerByCharId(member.m_id); 
            if (memberPlayer)
            {
                GetSyberiaRPC().SendToClient( SyberiaRPC.SYBRPC_PDA_SEND_GROUP_MESSAGE, memberPlayer.GetIdentity(), new Param2< string, string >( profile.m_name, serverData.param1 ) );
            }
        }
	}
	
	override void SendMessage( ParamsReadContext ctx, PlayerIdentity sender )
    { 
        Param2< int, string > serverData;			
        if ( !ctx.Read( serverData ) ) return;

		int errorCode = 0;
		ref CharProfile senderProfile = GetSyberiaCharacters().Get(sender, errorCode, true);
		if (!senderProfile) return;
		
        int senderId = senderProfile.m_id;
        string senderName = senderProfile.m_name;
        array<Man> players = new array<Man>();
        GetGame().GetPlayers(players);
        for (int q = 0; q < players.Count(); q++)
        {
            ref PlayerBase player = PlayerBase.Cast(players[q]);
            if (player)
            {
				ref CharProfile receiverProfile = player.m_charProfile;
				if (receiverProfile)
				{				
	                PlayerIdentity identity = player.GetIdentity();
	                int identityId = receiverProfile.m_id;
	                if (serverData.param1 == identityId)
	                {
	                    if (HasWorkingPDA(player))
	                    {
							
	                        string identityName = receiverProfile.m_name;
	                        GetSyberiaRPC().SendToClient( SyberiaRPC.SYBRPC_PDA_SEND_MESSAGE, sender, new Param4< int, string, int, string >( identityId, identityName, senderId, serverData.param2 ) );
	                        
	                        if (!(senderId == identityId))
	                        {
	                            GetSyberiaRPC().SendToClient( SyberiaRPC.SYBRPC_PDA_SEND_MESSAGE, identity, new Param4< int, string, int, string >( senderId, senderName, senderId, serverData.param2 ) );
	                        }
	                        
	                        return;
	                    }
	                }
				}
            }
        }
	}
	
	override void CheckContacts( ParamsReadContext ctx, PlayerIdentity sender )
    { 
        Param1< array<int> > serverData;			
        if ( !ctx.Read( serverData ) ) return;
        
        array<int> request = new array<int>();
        request.Copy(serverData.param1);
        
        array<Man> players = new array<Man>();
        GetGame().GetPlayers(players);
        
        array<int> result = new array<int>();
        for (int i = 0; i < request.Count(); i++)
        {
            int id = request[i];				
            for (int q = 0; q < players.Count(); q++)
            {
                ref PlayerBase player = PlayerBase.Cast(players[q]);
                if (player && player.m_charProfile)
                {
                    ref CharProfile profile = player.m_charProfile;
                    if (id == profile.m_id && HasWorkingPDA(player))
                    {
                        result.Insert(id);
                    }
                }
            }
        }
        
        CheckContactsResponse(sender, result);
	}
	
	void CheckContactsResponse(PlayerIdentity sender, ref array<int> contacts)
	{
		ref PluginSyberiaOptions_GroupFaction leadedGroup = GetSyberiaOptions().FindGroupByLeader(sender);
		ref array<ref SyberiaPdaGroupMember> groupMembers = null;
		string infoText = "";
		bool useGroupManagenemt = false;
		
		if (leadedGroup)
		{
			groupMembers = leadedGroup.m_members;
			infoText = leadedGroup.m_displayName + " (" + leadedGroup.m_members.Count() + "/" + leadedGroup.m_maxMembers + ")";
			useGroupManagenemt = true;
		}
		
		string groupChatName = "";
		int errorCode = 0;
		ref CharProfile profile = GetSyberiaCharacters().Get(sender, errorCode, true);
		if (!profile) return;
		
		ref PluginSyberiaOptions_GroupFaction memberGroup = GetSyberiaOptions().FindGroupByMember(profile.m_id);
		if (memberGroup)
		{
			groupChatName = memberGroup.m_displayName;
		}
		
		GetSyberiaRPC().SendToClient( SyberiaRPC.SYBRPC_PDA_CHECK_CONTACT, sender, new Param5< ref array<int>, bool, ref array<ref SyberiaPdaGroupMember>, string, string >( contacts, useGroupManagenemt, groupMembers, infoText, groupChatName ) );
	}
	
	override void InitPdaProfile( ParamsReadContext ctx, PlayerIdentity sender )
    {   
        
	}
	
	void SendPdaUserState(PlayerIdentity identity, ref CharProfile profile)
	{
		string instanceName = m_serverConfig.m_serverInstanceName + "_" + profile.m_id; 
		GetSyberiaRPC().SendToClient( SyberiaRPC.SYBRPC_PDA_USER_STATE, identity, new Param4<string, string, bool, bool>( profile.m_name, instanceName, m_serverConfig.m_enableGlobalChat, m_serverConfig.m_enableMapPage ) );
	}
	
	override void AddContact( ParamsReadContext ctx, PlayerIdentity sender )
    {        
        Param1< string > serverData;
        if ( !ctx.Read( serverData ) ) return;
        string requestName = serverData.param1;
		requestName.ToLower();
        
		int errorCode = 0;
		ref CharProfile profile = GetSyberiaCharacters().Get(sender, errorCode, true);
		if (!profile) return;
		
        array<Man> players = new array<Man>();
        GetGame().GetPlayers(players);
        for (int i = 0; i < players.Count(); i++)
        {
            PlayerBase player = PlayerBase.Cast(players[i]);
            if (player && player.m_charProfile)
            {
                int contactId = player.m_charProfile.m_id;
                string contactName = player.m_charProfile.m_name + "";
				contactName.ToLower();
				
                if ( contactName == requestName ) 
                {
                    if (profile.m_id != contactId)
                    {
                        GetSyberiaRPC().SendToClient( SyberiaRPC.SYBRPC_PDA_ADD_CONTACT, sender, new Param2<int, string>( contactId, player.m_charProfile.m_name ) );
                        return;
                    }
                }
            }
        }
    }
	
	override void GroupCommand( ParamsReadContext ctx, PlayerIdentity sender )
	{
        if (sender == null) return;
        
        Param2< int, string > serverData;
        if ( !ctx.Read( serverData ) ) return;
        
		int errorCode = 0;
        int action = serverData.param1;
        string fid = serverData.param2;
        ref PluginSyberiaOptions_GroupFaction group = GetSyberiaOptions().FindGroupByLeader(sender);        
        if (group == null) return;	
        
        if (action == 0)
        {		
            if (group.m_members.Count() >= group.m_maxMembers) return;
            
            array<PlayerIdentity> identities = new array<PlayerIdentity>;
            GetGame().GetPlayerIndentities(identities);
            PlayerIdentity newMemberIdentity = null;
            
			fid.ToLower();
			
            foreach (PlayerIdentity identity : identities)
            {
				errorCode = 0;
				ref CharProfile profile = GetSyberiaCharacters().Get(identity, errorCode, true);
				if (!profile) continue;
				
				string pname = profile.m_name + "";
				pname.ToLower();	
			
                if (pname == fid)
                {
                    newMemberIdentity = identity;
                    break;
                }
            }
            
            if (newMemberIdentity == null) return;
            
			errorCode = 0;
			ref CharProfile newMemberProfile = GetSyberiaCharacters().Get(newMemberIdentity, errorCode, true);
			if (!newMemberProfile) return;
			
            ref PluginSyberiaOptions_GroupFaction groupForNewMember = GetSyberiaOptions().FindGroupByMember(newMemberProfile.m_id);
            if (groupForNewMember != null) return;
  
            ref SyberiaPdaGroupMember newMember = new SyberiaPdaGroupMember;
			newMember.m_id = newMemberProfile.m_id;
            newMember.m_name = newMemberProfile.m_name;
            group.AddMember(newMember);
			
			SybLogSrv("PDA GROUP '" + group.m_name + "' ADD MEMBER '" + newMemberProfile.m_name + "'");
        }
        else if (action == 1)
        {
            int memberId = fid.ToInt();
            for (int i = 0; i < group.m_members.Count(); i++)
            {
				ref SyberiaPdaGroupMember oldMember = group.m_members.Get(i);
                if (oldMember.m_id == memberId)
                {
					SybLogSrv("PDA GROUP '" + group.m_name + "' REMOVE MEMBER '" + oldMember.m_name + "'");
                    group.RemoveMember(oldMember);
                    break;
                }
            }
        }
        
        CheckContactsResponse(sender, null);
	}
};

class PdaServerConfig
{
	bool m_enableGlobalChat = true;
	bool m_enableMapPage = true;
	string m_serverInstanceName = "";
};