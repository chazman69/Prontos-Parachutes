# Prontos Parachutes

Arma Reforger / Enfusion addon adding a deployable T10 parachute system.

This addon is **standalone** — it embeds its own copy of the parachute flight,
deployment and networking code (originally from the Parachute Framework, see
[Credits](#credits)) so it no longer depends on the framework Workshop item. All
gameplay classes are namespaced `Pronto_*`.

> **Do not run this alongside the Parachute Framework.** Both override the same
> base-game player controller (`{6E2BB64764E3BE9B}DefaultPlayerController.et`), so
> loading both on one server makes them fight over it. This addon replaces the
> framework — it is not a companion to it.

- **Project ID:** `ProntosParachutes`
- **GUID:** `68996556700D5CA1`

## Contents

| Path | What |
|------|------|
| `Assets/` | Meshes, materials, textures, animations (canopy, rig, bag) |
| `Configs/` | Inventory item entity catalogs (FIA/US/USSR), inventory UI, parachute config |
| `Prefabs/` | Character base, deployed parachute canopy, parachute bag backpacks, player controller |
| `Scripts/` | Parachute component / deployed entity / item / helpers (`Pronto_*`), slot camera + loadout area |
| `UI/` | Inventory slot icon |
| `license.txt` | Arma Public License – Share Alike (APL-SA) |
| `addon.gproj` | Enfusion game project file |

## Dependencies

Base game only:

- `58D0FB3206B6F859`

(The `65930CB4CD0237B2` Parachute Framework dependency has been removed — its code
is now vendored into this addon.)

## Use

Open `addon.gproj` in the Enfusion Workbench.

**No wiring required.** `Prefabs/Characters/Core/DefaultPlayerController.et` overrides
the base-game player controller (same path + GUID `6E2BB64764E3BE9B`) and adds
`Pronto_ParachuteComponent`, so the parachute logic is injected into every player
controller automatically — the same mechanism the original framework uses. Just
enable the addon and give players the `ParachuteBag_T10_01` backpack (registered in
the bundled arsenal / entity catalogs).

## Credits

The parachute flight model, deployment flow and custom networking are adapted from
the **Arma Reforger Parachute Framework** by **Alphaegen**:

- Source: https://github.com/Alphaegen/ArmaReforgerParachutes
- Workshop: https://reforger.armaplatform.com/workshop/65930CB4CD0237B2-ParachuteFramework
- Forked from upstream at commit `28f2aa6` (2026-01-26)

This addon modifies that work (namespaced the classes to `Pronto_*`, vendored the
prefabs so the framework dependency could be dropped, and fixed a multiplayer
landing bug where a still-seated player could be deleted together with the
parachute on touchdown — see `Scripts/Game/Parachutes/Pronto_ParachuteComponent.c`).

## Licence

This work is licensed under the **Arma Public License – Share Alike (APL-SA)** — see
[`license.txt`](license.txt). As required by the licence: attribution above,
non-commercial and Arma-only use, and any redistribution must remain under APL-SA.

<a rel="license" href="https://www.bohemia.net/community/licenses/arma-public-license-share-alike" target="_blank"><img src="https://data.bistudio.com/images/license/APL-SA.png"></a>
