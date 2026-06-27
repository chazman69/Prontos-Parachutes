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

class Pronto_ParachuteItemComponentClass : ScriptComponentClass {}

class Pronto_ParachuteItemComponent : ScriptComponent
{
	[Attribute("", UIWidgets.ResourceNamePicker, "Parachute Prefab", "et")]
	protected ResourceName m_ParachutePrefab;

	// Durable, replicated state.
	[RplProp(onRplName: "OnRep_ParachuteUsed")]
	protected bool m_bParachuteUsed = false;

	protected InventoryItemComponent m_ItemComp;

	ResourceName GetParachutePrefab()
	{
		return m_ParachutePrefab;
	}

	bool GetParachuteUsed()
	{
		return m_bParachuteUsed;
	}

	// Server authoritative setter (clients should not change this)
	void SetParachuteUsed_Server()
	{
		if (!Replication.IsServer())
			return;

		if (m_bParachuteUsed)
			return;

		m_bParachuteUsed = true;
		Replication.BumpMe();
	}

	void OnRep_ParachuteUsed()
	{
		// Optional: UI refresh, disable action prompts, etc.
	}

	override void OnPostInit(IEntity owner)
	{
		if (SCR_Global.IsEditMode())
			return;

		SetEventMask(owner, EntityEvent.INIT);
	}

	override void EOnInit(IEntity owner)
	{
		if (m_ParachutePrefab == "")
		{
			Print("ERROR: Pronto_ParachuteItemComponent is missing its prefab! Skipping parachute registration.");
			return;
		}

		m_ItemComp = InventoryItemComponent.Cast(owner.FindComponent(InventoryItemComponent));
		if (!m_ItemComp)
			return;

		m_ItemComp.m_OnParentSlotChangedInvoker.Insert(OnParentSlotChanged);

		GetGame().GetCallqueue().CallLater(PrimeCurrentSlot, 250, false);
	}

	override void OnDelete(IEntity owner)
	{
		if (m_ItemComp)
			m_ItemComp.m_OnParentSlotChangedInvoker.Remove(OnParentSlotChanged);
	}

	protected void PrimeCurrentSlot()
	{
		if (!m_ItemComp)
			return;

		InventoryStorageSlot slot = m_ItemComp.GetParentSlot();
		if (!slot)
			return;

		OnParentSlotChanged(null, slot);
	}

	void OnParentSlotChanged(InventoryStorageSlot oldSlot, InventoryStorageSlot newSlot)
	{
		if (oldSlot)
		{
			Pronto_ParachuteComponent parachuteComponent = Pronto_ParachuteHelperFunctions.GetParachuteComponentFromSlotOwner(oldSlot);
			if (parachuteComponent && parachuteComponent.GetParachuteItem() == this)
				parachuteComponent.ClearParachuteItem();
		}

		if (newSlot)
		{
			Pronto_ParachuteComponent parachuteComponent = Pronto_ParachuteHelperFunctions.GetParachuteComponentFromSlotOwner(newSlot);
			if (parachuteComponent)
				parachuteComponent.SetParachuteItem(this);
		}
	}
}
