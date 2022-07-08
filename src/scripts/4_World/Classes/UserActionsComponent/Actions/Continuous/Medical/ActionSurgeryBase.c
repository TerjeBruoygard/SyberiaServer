modded class ActionSurgeryBase
{
    override void ApplySurgery( ItemBase item, PlayerBase player, bool self)
    {
        if ( (GetSyberiaOptions().m_client.m_operateVisceraHimself || !self) && player.m_visceraHit)
        {
            player.m_BleedingManagerServer.RemoveVisceraHit();
        }
        else if (player.m_bulletHits > 0)
        {
            player.m_BleedingManagerServer.RemoveBulletHit(false);
        }
        else if (player.m_knifeHits > 0)
        {
            player.m_BleedingManagerServer.RemoveKnifeHit(false);
        }
        
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