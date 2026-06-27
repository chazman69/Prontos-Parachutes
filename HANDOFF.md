# Handoff — Prontos Parachutes (standalone)

For the dev taking this to Workbench and Workshop. Read before publishing.

---

## 1. What this is

A **standalone** version of the parachute mod. The Parachute Framework code is
vendored in (classes namespaced `Pronto_*`) and the framework Workshop dependency
is removed. It also carries the multiplayer landing fix.

- **Replaces the framework — do not run both.** This addon overrides the base-game
  player controller (`{6E2BB64764E3BE9B}Prefabs/Characters/Core/DefaultPlayerController.et`)
  to inject `Pronto_ParachuteComponent` into every player — the same mechanism the
  framework uses. If the framework is also loaded, both patch that one controller and
  fight over it. Ship this *instead of* the framework, not next to it.
- **No wiring needed.** Because the controller is a base override, the parachute
  works on any scenario with the addon enabled — nothing to set per-mission.
- **Landing fix:** the chute is deleted only once its compartment is confirmed empty,
  so a still-seated player is never deleted on touchdown (the spawn-screen bug). See
  `Scripts/Game/Parachutes/Pronto_ParachuteComponent.c` (`SafeDeleteChuteWhenEmpty`)
  and `Pronto_ParachuteDeployedEntity.c` (`DestroyParachute`).

> ⚠️ Prefabs, the controller override, and the new GUIDs were authored by hand on a
> machine without Workbench. **Not validated in-engine.** Step 2 is mandatory.

---

## 2. Workbench validation checklist (do this first)

1. Open `addon.gproj` in the Enfusion Workbench.
2. Let Workbench regenerate the script `.meta` files and `resourceDatabase.rdb` on
   first open. Commit the regenerated files.
3. **Script Editor:** all four `Pronto_*` scripts compile clean.
4. **`Prefabs/Characters/Core/DefaultPlayerController.et`** — this is the key one.
   Confirm it shows as an **override** of the base controller (padlock icon, GUID
   `6E2BB64764E3BE9B`) and that it carries `Pronto_ParachuteComponent`. If Workbench
   made it a *new* prefab with a fresh GUID instead of overriding the base, the
   parachute will not auto-apply — re-create it via "inherit/override" on the base
   `DefaultPlayerController.et`.
5. **`DeployedParachute_T10_Canopy_Base.et`** (biggest hand-merge) — root resolves to
   `Pronto_ParachuteDeployedEntity`; RigidBody, `SCR_BaseCompartmentManagerComponent`
   (door + `passenger_l02` slot), `SCR_DamageManagerComponent`, `RplComponent`,
   `VehicleAnimationComponent`, MeshObject = `Parachute_Canopy.xob` all present/bound.
6. **`ParachuteBag_T10_Base.et`** — inherits `Backpack_Kolobok`, carries
   `Pronto_ParachuteItemComponent` with `m_ParachutePrefab` → the T-10 canopy.
7. **MP smoke test** (the bug repro): host with a **remote** client, jump from height,
   deploy, land hard and soft. Confirm: player is **not** sent to the spawn screen,
   hard landing applies leg-break / fatal as configured, chute removed after exit.

---

## 3. Do NOT coexist with the Parachute Framework

Both this addon and the framework override the same base-game player controller
(`6E2BB64764E3BE9B`). On a server running both, only the last-loaded override wins, so
one mod's parachutes break. This is by design — standalone replaces the framework.
(If you ever need them side by side, that's the *keep-the-dependency* build instead,
which patches the bug with `modded class` overrides and adds no second controller.)

---

## 4. Workshop publish

1. Finish Step 2 and commit the regenerated `.meta` / `.rdb`.
2. Build / publish from Workbench. Use `WORKSHOP_DESCRIPTION.txt` as the store copy.
3. **APL-SA** (required): keep `license.txt`, the README credits, and the per-file
   script headers. Non-commercial, Arma only, redistribution stays APL-SA with
   attribution to **Alphaegen**.

---

## 5. Tracking upstream (optional)

Forked from framework `28f2aa6` (2026-01-26). To pull later changes:

```bash
git remote add framework-upstream https://github.com/Alphaegen/ArmaReforgerParachutes.git
git fetch framework-upstream
git diff 28f2aa6 framework-upstream/main -- scripts/Game/Parachutes
```

Port relevant changes into the `Pronto_*` files by hand, then bump the "Forked" line
in the README and the script headers.
