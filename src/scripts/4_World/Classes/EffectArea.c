modded class EffectArea
{
    override void InitZone()
    {
        GetGame().ObjectDelete(this);
    }
};