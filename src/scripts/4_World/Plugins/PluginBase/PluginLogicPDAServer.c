modded class PluginLogicPDA
{	
	ref PdaGroupsConfig m_groupsConfig;
	ref PdaServerConfig m_serverConfig;
	string mainPath = "$profile:SyberiaPDA\\groups.json";
	string confPath = "$profile:SyberiaPDA\\config.json";
	
	override void OnInit()
	{
		super.OnInit();
		
		MakeDirectory("$profile:SyberiaPDA");
		if (!FileExist(confPath))
		{
			m_serverConfig = new PdaServerConfig();
			JsonFileLoader<ref PdaServerConfig>.JsonSaveFile(confPath, m_serverConfig);
		}
		else
		{
			JsonFileLoader<ref PdaServerConfig>.JsonLoadFile(confPath, m_serverConfig);
		}
		
		if (!FileExist(mainPath))
		{
			ref PdaGroupConfig sampleElement = new PdaGroupConfig();
			sampleElement.m_name = "sampleGroup";
			sampleElement.m_maxMembers = 50;
			
			sampleElement.m_leaders = new array<string>();
			sampleElement.m_leaders.Insert("Leader Steam ID");
			
			sampleElement.m_members = new array<ref GroupMember>();
			
			m_groupsConfig = new PdaGroupsConfig();
			m_groupsConfig.m_groups = new ref array<ref PdaGroupConfig>;
			m_groupsConfig.m_groups.Insert(sampleElement);
			
			JsonFileLoader<ref PdaGroupsConfig>.JsonSaveFile(mainPath, m_groupsConfig);
		}
		else
		{
			JsonFileLoader<ref PdaGroupsConfig>.JsonLoadFile(mainPath, m_groupsConfig);
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
        
        ref PdaGroupConfig group = FindGroupByMember(sender.GetId());
        if (!group) return;
        
        ref array<PlayerIdentity> identities = new array<PlayerIdentity>();
        GetGame().GetPlayerIndentities(identities);
        foreach (ref PlayerIdentity identity : identities)
        {
            foreach (ref GroupMember member : group.m_members)
            {
                if (identity.GetId() == member.m_guid)
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
		ref PdaGroupConfig leadedGroup = FindGroupByLeader(sender);
		ref array<ref GroupMember> groupMembers = null;
		string infoText = "";
		bool useGroupManagenemt = false;
		
		if (leadedGroup)
		{
			groupMembers = leadedGroup.m_members;
			infoText = leadedGroup.m_name + "(" + leadedGroup.m_members.Count() + "/" + leadedGroup.m_maxMembers + ")";
			useGroupManagenemt = true;
		}
		
		string groupChatName = "";
		ref PdaGroupConfig memberGroup = FindGroupByMember(sender.GetId());
		if (memberGroup)
		{
			groupChatName = memberGroup.m_name;
		}
		
		GetSyberiaRPC().SendToClient( SyberiaRPC.SYBRPC_PDA_CHECK_CONTACT, sender, new Param5< ref array<string>, bool, ref array<ref GroupMember>, string, string >( contacts, useGroupManagenemt, groupMembers, infoText, groupChatName ) );
	}
	
	override void GetVisualUserId( ref ParamsReadContext ctx, ref PlayerIdentity sender )
    {   
        string userVisualId = sender.GetPlainId();
        GetSyberiaRPC().SendToClient( SyberiaRPC.SYBRPC_PDA_USER_STATE, sender, new Param3<string, bool, bool>( userVisualId, m_serverConfig.m_enableGlobalChat, m_serverConfig.m_enableMapPage ) );
	}
	
	override void AddContact( ref ParamsReadContext ctx, ref PlayerIdentity sender )
    {        
        Param1< string > serverData;
        if ( !ctx.Read( serverData ) ) return;
        string requestName = serverData.param1;
        
        ref array<Man> players = new array<Man>();
        GetGame().GetPlayers(players);
        for (int i = 0; i < players.Count(); i++)
        {
            PlayerBase player = PlayerBase.Cast(players[i]);
            if (player)
            {
                PlayerIdentity identity = player.GetIdentity();
                string contactPlainId = identity.GetPlainId();
                string contactSteamId = identity.GetId();
                string contactName = identity.GetName();
                if ( (contactPlainId == requestName) || (contactName == requestName) ) 
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
	
	ref PdaGroupConfig FindGroupByLeader(ref PlayerIdentity identity)
	{
		foreach (ref PdaGroupConfig group : m_groupsConfig.m_groups)
		{
			foreach (string leader : group.m_leaders)
			{
				string value = identity.GetPlainId();
				if (leader == value)
				{
					return group;
				}
				
				value = identity.GetId();
				if (leader == value)
				{
					return group;
				}
			}
		}
		
		return null;
	}
	
	ref PdaGroupConfig FindGroupByMember(string guid)
	{
		foreach (ref PdaGroupConfig group : m_groupsConfig.m_groups)
		{
			foreach (ref GroupMember member : group.m_members)
			{
				if (guid == member.m_guid)
				{
					return group;
				}
			}
		}
		
		return null;
	}
	
	bool FindGroupMember(string guid, out ref GroupMember groupMember, out ref PdaGroupConfig groupResult)
	{
		foreach (ref PdaGroupConfig group : m_groupsConfig.m_groups)
		{
			foreach (ref GroupMember member : group.m_members)
			{
				if (guid == member.m_guid)
				{
					groupMember = member;
					groupResult = group;
					return true;
				}
			}
		}
		
		return false;
	}
	
	override void GroupCommand( ref ParamsReadContext ctx, ref PlayerIdentity sender )
	{
        if (sender == null) return;
        
        Param3< int, string, string > serverData;
        if ( !ctx.Read( serverData ) ) return;
        
        int action = serverData.param1;
        string fid = serverData.param2;
        ref PdaGroupConfig group = FindGroupByLeader(sender);
        
        if (group == null) return;	
        
        if (action == 0)
        {		
            if (group.m_members.Count() >= group.m_maxMembers) return;
            
            ref array<PlayerIdentity> identities = new array<PlayerIdentity>;
            GetGame().GetPlayerIndentities(identities);
            ref PlayerIdentity newMemberIdentity = null;
            
            foreach (ref PlayerIdentity identity : identities)
            {
                string fvalue = identity.GetName();
                if (fvalue == fid)
                {
                    newMemberIdentity = identity;
                    break;
                }
                
                fvalue = identity.GetPlainId();
                if (fvalue == fid)
                {
                    newMemberIdentity = identity;
                    break;
                }
            }
            
            if (newMemberIdentity == null) return;
            
            ref PdaGroupConfig groupForNewMember = FindGroupByMember(newMemberIdentity.GetId());
            if (groupForNewMember != null) return;
            
            ref GroupMember newMember = new GroupMember();
            newMember.m_guid = newMemberIdentity.GetId();
            newMember.m_uid = newMemberIdentity.GetPlainId();
            newMember.m_name = newMemberIdentity.GetName();
            
            group.m_members.Insert(newMember);
        }
        else if (action == 1)
        {
            int memberId = -1;
            for (int i = 0; i < group.m_members.Count(); i++)
            {
                if (group.m_members.Get(i).m_guid == fid)
                {
                    memberId = i;
                    break;
                }
            }
            
            if (memberId < 0) return;
            group.m_members.Remove(memberId);
        }
        
        JsonFileLoader<ref PdaGroupsConfig>.JsonSaveFile(mainPath, m_groupsConfig);
        CheckContactsResponse(sender, null);
	}
};

class PdaGroupsConfig
{
	ref array<ref PdaGroupConfig> m_groups;
};

class PdaGroupConfig
{
	string m_name;
	int m_maxMembers;
	ref array<string> m_leaders;
	ref array<ref GroupMember> m_members;
};

class PdaServerConfig
{
	bool m_enableGlobalChat = true;
	bool m_enableMapPage = true;
};