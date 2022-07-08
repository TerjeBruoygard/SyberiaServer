modded class ActionUseSalveBase
{
    override void ApplyUseSalve( ItemBase item, PlayerBase player, bool self)
    {
        SalveBase salveItem = SalveBase.Cast(item);
        if (salveItem)
        {
            salveItem.OnApply(player);
        }

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