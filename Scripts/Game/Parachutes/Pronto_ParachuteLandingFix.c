// -----------------------------------------------------------------------------
// Multiplayer landing fix for the Arma Reforger Parachute Framework by Alphaegen.
//   Framework: https://github.com/Alphaegen/ArmaReforgerParachutes  (APL-SA)
//
// Bug: on landing the framework asks the OWNING client to disembark
// (AskOwnerToGetOutFromVehicle, async over the network) and then deletes the
// parachute on a fixed delay. SCR_EntityHelper.DeleteEntityAndChildren removes
// the parachute AND its children -- and a seated character is a child. Under MP
// latency / load the get-out had not completed when the delete fired, so the
// still-seated player was deleted with the chute and dumped to the spawn screen.
//
// Fix (overrides only -- the framework stays a dependency):
//  * ParachuteComponent.DeleteParachuteEntity now polls the compartment and
//    deletes only once it is confirmed empty. Covers every delete path
//    (landing exit, player death, controlled-entity change).
//  * ParachuteDeployedEntity.DestroyParachute no longer self-deletes with
//    children while occupied; it defers to the exit flow's safe delete.
// -----------------------------------------------------------------------------

modded class ParachuteComponent
{
	// The framework calls this (directly and via CallLater) to remove the chute.
	// Make it occupancy-safe: never delete while a character is still seated.
	override void DeleteParachuteEntity(IEntity parachute)
	{
		Pronto_SafeDeleteChute(parachute, 0);
	}

	protected void Pronto_SafeDeleteChute(IEntity parachute, int tries)
	{
		if (!parachute)
			return;

		if (Pronto_ChuteOccupied(parachute))
		{
			// ~3s cap (60 * 50ms). If the get-out never lands, leak the chute
			// rather than ever risk deleting the player along with it.
			if (tries < 60)
				GetGame().GetCallqueue().CallLater(Pronto_SafeDeleteChute, 50, false, parachute, tries + 1);
			else
				Print("[ProntoParachute] occupant did not disembark; leaving chute to avoid deleting player", LogLevel.WARNING);

			return;
		}

		// Confirmed empty -> framework's real delete.
		super.DeleteParachuteEntity(parachute);
	}

	protected bool Pronto_ChuteOccupied(IEntity parachute)
	{
		BaseCompartmentManagerComponent bcm = BaseCompartmentManagerComponent.Cast(parachute.FindComponent(BaseCompartmentManagerComponent));
		if (!bcm)
			return false;

		array<BaseCompartmentSlot> slots = {};
		bcm.GetCompartments(slots);
		foreach (BaseCompartmentSlot s : slots)
		{
			if (s && s.IsOccupied())
				return true;
		}

		return false;
	}
}

modded class ParachuteDeployedEntity
{
	override void DestroyParachute()
	{
		if (m_bIsDestroyed)
			return;

		// If still occupied, let the exit flow (ParachuteComponent.Rpc_ServerExitParachute
		// -> the occupancy-safe DeleteParachuteEntity above) remove the chute. Do NOT
		// DeleteEntityAndChildren here while a player is seated, or we take them with it.
		if (m_Compartment && m_Compartment.IsOccupied())
		{
			m_bIsDestroyed = true;
			AskServerExit();
			return;
		}

		super.DestroyParachute();
	}
}
