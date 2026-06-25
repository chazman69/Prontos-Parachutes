modded class SCR_CharacterCameraHandlerComponent
{
    override void OnAlphatestChange(int a)
    {
        super.OnAlphatestChange(a);

        if (!m_LoadoutStorage)
            return;

        array<typename> areas = {
            Pronto_Parachute
        };

        for (int i = 0; i < areas.Count(); i++)
        {
            IEntity entity = m_LoadoutStorage.GetClothFromArea(areas[i]);
            if (!entity) 
                continue;

            BaseLoadoutClothComponent cloth = BaseLoadoutClothComponent.Cast(entity.FindComponent(BaseLoadoutClothComponent));
            if (cloth)
                cloth.SetAlpha(a);
        }
    }
};
