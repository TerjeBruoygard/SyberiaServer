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
	
	override void SendGlobalMessage( ref ParamsReadContext ctx, ref PlayerIdentity sender )
	{
        Param1< string > serverData;
        if ( !ctx.Read( serverData ) ) return;
        
        if (!sender) return;
        
        if (!m_serverConfig.m_enableGlobalChat) return;
        
        GetSyberiaRPC().SendToAll( SyberiaRPC.SYBRPC_PDA_SEND_GLOBAL_MESSAGE, new Param2< string, string >( sender.GetName(), serverData.param1 ) );
	}
	
	override void SendGroupMessage( ref ParamsReadContext ctx, ref PlayerIdentity sender )
	{
        Param1< string > serverData;			
        if ( !ctx.Read( serverData ) ) return;
        
        if (!sender) return;
        
		ref CharProfile profile = GetSyberiaCharacters().Get(sender, true);
		if (!profile) return;
		
        ref PluginSyberiaOptions_GroupFaction group = GetSyberiaOptions().FindGroupByMember(profile.m_id);
        if (!group) return;
        
        ref array<PlayerIdentity> identities = new array<PlayerIdentity>();
        GetGame().GetPlayerIndentities(identities);
        foreach (ref PlayerIdentity identity : identities)
        {
            foreach (ref SyberiaPdaGroupMember member : group.m_members)
            {
                if (identity.GetId() == member.m_uid)
                {
                    GetSyberiaRPC().SendToClient( SyberiaRPC.SYBRPC_PDA_SEND_GROUP_MESSAGE, identity, new Param2< string, string >( sender.GetName(), serverData.param1 ) );
                }
            }
        }
            
        delete identities;
	}
	
	override void SendMessage( ref ParamsReadContext ctx, ref PlayerIdentity sender )
    { 
        Param2< string, string > serverData;			
        if ( !ctx.Read( serverData ) ) return;

        string senderId = sender.GetId();
        string senderName = sender.GetName();
        ref array<Man> players = new array<Man>();
        GetGame().GetPlayers(players);
        for (int q = 0; q < players.Count(); q++)
        {
            ref PlayerBase player = PlayerBase.Cast(players[q]);
            if (player)
            {
                ref PlayerIdentity identity = player.GetIdentity();
                string identityId = identity.GetId();
                if (serverData.param1 == identityId)
                {
                    if (HasWorkingPDA(player))
                    {
                        string identityName = identity.GetName();
                        GetSyberiaRPC().SendToClient( SyberiaRPC.SYBRPC_PDA_SEND_MESSAGE, sender, new Param4< string, string, string, string >( identityId, identityName, senderId, serverData.param2 ) );
                        
                        if (!(senderId == identityId))
                        {
                            GetSyberiaRPC().SendToClient( SyberiaRPC.SYBRPC_PDA_SEND_MESSAGE, identity, new Param4< string, string, string, string >( senderId, senderName, senderId, serverData.param2 ) );
                        }
                        
                        return;
                    }
                }
            }
        }
        
        //GetSyberiaRPC().SendToClient( SyberiaRPC.SYBRPC_PDA_SEND_MESSAGE, sender, new Param4< string, string, string, string >( "", "", "", "" ) );
	}
	
	override void CheckContacts( ref ParamsReadContext ctx, ref PlayerIdentity sender )
    { 
        Param1< array<string> > serverData;			
        if ( !ctx.Read( serverData ) ) return;
        
        ref array<string> request = new array<string>();
        request.Copy(serverData.param1);
        
        ref array<Man> players = new array<Man>();
        GetGame().GetPlayers(players);
        
        ref array<string> result = new array<string>();
        for (int i = 0; i < request.Count(); i++)
        {
            string uid = request[i];				
            for (int q = 0; q < players.Count(); q++)
            {
                ref PlayerBase player = PlayerBase.Cast(players[q]);
                if (player)
                {
                    ref PlayerIdentity identity = player.GetIdentity();
                    string identityId = identity.GetId();
                    if (uid == identityId && HasWorkingPDA(player))
                    {
                        result.Insert(uid);
                    }
                }
            }
        }
        
        CheckContactsResponse(sender, result);
	}
	
	void CheckContactsResponse(ref PlayerIdentity sender, ref array<string> contacts)
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
		
		ref CharProfile profile = GetSyberiaCharacters().Get(sender, true);
		if (!profile) return;
		
		ref PluginSyberiaOptions_GroupFaction memberGroup = GetSyberiaOptions().FindGroupByMember(profile.m_id);
		if (memberGroup)
		{
			groupChatName = memberGroup.m_displayName;
		}
		
		GetSyberiaRPC().SendToClient( SyberiaRPC.SYBRPC_PDA_CHECK_CONTACT, sender, new Param5< ref array<string>, bool, ref array<ref SyberiaPdaGroupMember>, string, string >( contacts, useGroupManagenemt, groupMembers, infoText, groupChatName ) );
	}
	
	override void GetVisualUserId( ref ParamsReadContext ctx, ref PlayerIdentity sender )
    {   
        
	}
	
	void SendPdaUserState(ref PlayerIdentity identity, ref CharProfile profile)
	{
		GetSyberiaRPC().SendToClient( SyberiaRPC.SYBRPC_PDA_USER_STATE, identity, new Param3<string, bool, bool>( profile.m_name, m_serverConfig.m_enableGlobalChat, m_serverConfig.m_enableMapPage ) );
	}
	
	override void AddContact( ref ParamsReadContext ctx, ref PlayerIdentity sender )
    {        
        Param1< string > serverData;
        if ( !ctx.Read( serverData ) ) return;
        string requestName = serverData.param1;
		requestName.ToLower();
        
        ref array<Man> players = new array<Man>();
        GetGame().GetPlayers(players);
        for (int i = 0; i < players.Count(); i++)
        {
            PlayerBase player = PlayerBase.Cast(players[i]);
            if (player)
            {
                PlayerIdentity identity = player.GetIdentity();
                string contactSteamId = identity.GetId();
                string contactName = player.m_charProfile.m_name + "";
				contactName.ToLower();
				
                if ( contactName == requestName ) 
                {
                    if (sender.GetId() != contactSteamId)
                    {
                        GetSyberiaRPC().SendToClient( SyberiaRPC.SYBRPC_PDA_ADD_CONTACT, sender, new Param2<string, string>( contactSteamId, contactName ) );
                        return;
                    }
                }
            }
        }
        
        //GetRPCManager().SendRPC( GearPDAModPreffix, "AddContact", new Param2<string, string>( "", "" ), true, sender );
    }
	
	override void GroupCommand( ref ParamsReadContext ctx, ref PlayerIdentity sender )
	{
        if (sender == null) return;
        
        Param2< int, string > serverData;
        if ( !ctx.Read( serverData ) ) return;
        
        int action = serverData.param1;
        string fid = serverData.param2;
        ref PluginSyberiaOptions_GroupFaction group = GetSyberiaOptions().FindGroupByLeader(sender);        
        if (group == null) return;	
        
        if (action == 0)
        {		
            if (group.m_members.Count() >= group.m_maxMembers) return;
            
            ref array<PlayerIdentity> identities = new array<PlayerIdentity>;
            GetGame().GetPlayerIndentities(identities);
            ref PlayerIdentity newMemberIdentity = null;
            
			fid.ToLower();
			
            foreach (ref PlayerIdentity identity : identities)
            {
				ref CharProfile profile = GetSyberiaCharacters().Get(identity, true);
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
            
			ref CharProfile newMemberProfile = GetSyberiaCharacters().Get(newMemberIdentity, true);
			if (!newMemberProfile) return;
			
            ref PluginSyberiaOptions_GroupFaction groupForNewMember = GetSyberiaOptions().FindGroupByMember(newMemberProfile.m_id);
            if (groupForNewMember != null) return;
  
            ref SyberiaPdaGroupMember newMember = new SyberiaPdaGroupMember;
			newMember.m_id = newMemberProfile.m_id;
            newMember.m_uid = newMemberProfile.m_uid;
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
};