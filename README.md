# Prontos Parachutes

Arma Reforger / Enfusion addon adding a deployable T-10 parachute system, built on
the **Parachute Framework** by Alphaegen (see [Credits](#credits)).

- **Project ID:** `ProntosParachutes`
- **GUID:** `68996556700D5CA1`

## Dependencies

- `65930CB4CD0237B2` — Parachute Framework (Alphaegen)
- `58D0FB3206B6F859` — base game

The framework is kept as a Workshop dependency. This addon adds the T-10 content
(meshes, rig, bag, arsenal entries) on top of it, plus a multiplayer landing fix.

## Multiplayer landing fix

`Scripts/Game/Parachutes/Pronto_ParachuteLandingFix.c` patches the framework with
two `modded class` overrides so it never deletes a still-seated player on landing:

- **`ParachuteComponent.DeleteParachuteEntity`** — now polls the parachute's
  compartment and deletes only once it is confirmed empty. The framework asks the
  owning client to disembark asynchronously and then deleted the chute on a fixed
  delay; under MP latency / load the get-out had not finished, and
  `DeleteEntityAndChildren` took the still-seated character with it — dumping the
  player back to the spawn screen. Covers every delete path (landing, death,
  controlled-entity change).
- **`ParachuteDeployedEntity.DestroyParachute`** — while occupied, defers to the
  exit flow's safe delete instead of self-deleting with children.

Override-only: no framework files are modified or copied, so the fix rides on top
of whatever framework version is installed.

## Contents

| Path | What |
|------|------|
| `Assets/` | Meshes, materials, textures, animations (canopy, rig, bag) |
| `Configs/` | Inventory item entity catalogs (FIA/US/USSR), inventory UI, parachute config |
| `Prefabs/` | T-10 canopy, parachute bag backpacks, character base (inherit framework prefabs) |
| `Scripts/` | Landing-fix overrides, slot camera component, loadout area UI |
| `UI/` | Inventory slot icon |
| `addon.gproj` | Enfusion game project file |

## Use

Open `addon.gproj` in the Enfusion Workbench. Deployment, controls and the player
controller are provided by the Parachute Framework dependency — no extra wiring
beyond however you already integrate the framework.

## Credits

Built on the **Arma Reforger Parachute Framework** by **Alphaegen**:

- Source: https://github.com/Alphaegen/ArmaReforgerParachutes
- Workshop: https://reforger.armaplatform.com/workshop/65930CB4CD0237B2-ParachuteFramework

The landing-fix overrides in this addon adapt the framework's `ParachuteComponent`
and `ParachuteDeployedEntity` behaviour and are therefore licensed under the same
licence.

## Licence

Arma Public License – Share Alike (APL-SA) — see [`license.txt`](license.txt).
Non-commercial, Arma only; redistribution stays under APL-SA with attribution.

<a rel="license" href="https://www.bohemia.net/community/licenses/arma-public-license-share-alike" target="_blank"><img src="https://data.bistudio.com/images/license/APL-SA.png"></a>
