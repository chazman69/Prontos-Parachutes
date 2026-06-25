class Pronto_ParachuteHelperFunctions
{
	static Pronto_ParachuteComponent GetParachuteComponentFromSlotOwner(InventoryStorageSlot slot)
	{
		if (!slot)
			return null;

		IEntity storageOwner = slot.GetStorage().GetOwner();
		if (!SCR_ChimeraCharacter.Cast(storageOwner))
			return null;

		int playerId = GetGame().GetPlayerManager().GetPlayerIdFromControlledEntity(storageOwner);
		if (playerId < 0)
			return null;

		SCR_PlayerController pc = SCR_PlayerController.Cast(GetGame().GetPlayerManager().GetPlayerController(playerId));
		if (!pc)
			return null;

		return Pronto_ParachuteComponent.Cast(pc.FindComponent(Pronto_ParachuteComponent));
	}
}
