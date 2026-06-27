// -----------------------------------------------------------------------------
// Adapted from the Arma Reforger Parachute Framework by Alphaegen.
//   Source : https://github.com/Alphaegen/ArmaReforgerParachutes
//   Forked : 28f2aa6 (2026-01-26)
//   Licence: Arma Public License - Share Alike (APL-SA) - see license.txt
//
// Modifications: classes namespaced Pronto_* and vendored so the framework
// dependency could be dropped; multiplayer landing fix (chute is deleted only
// once its compartment is empty) to stop seated players being deleted on
// touchdown and dumped to the spawn screen.
// -----------------------------------------------------------------------------

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
