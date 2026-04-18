# RAM Locations

This document will highlight some interesting RAM locations, what they're used for, and how to interpret their data. This file is mostly aimed at implementing auto-trackers and multiworld clients.

## Adventure Mode Data

The Adventure Mode progress is tracked in 5 different save slots:  
One currently loaded save (from now on called CurrAdv), and 4 "memory card" saves (from now on called SaveSlots).

The SaveSlots technically are not the actual data on the memory card, but rather copies of that data but saved in RAM.  
When the game boots, all 4 SaveSlots in RAM are filled with data found on the actual memory card.  
Whenever the player saves their Adventure Mode progress, CurrAdv gets written into the SaveSlot, and then all 4 SaveSlots are copied, all at once, onto the actual memory card.  
This is also the reason why saving takes a moment, but loading a save is instant: When loading an existing save slot, that slot's data just gets copied into CurrAdv, and the game reloads the current scene with the new data.

Each of the 5 Adventure Mode progress slots is 0x50 bytes big.  
The CurrAdv data is found at `8008FBA4` in RAM.  
The SaveSlots start at `800992E8`. With each slot using 0x50 bytes, the 4 save slots are located at `800992E8`, `80099338`, `80099388`, and `800993D8`.

The randomizer only makes CurrAdv and SaveSlot 1 available to the player; SaveSlot 2, 3, and 4 are reserved for other data. Furthermore, CurrAdv works slightly differently compared to the base game, since the game progress flags (like winning Adventure Mode races) do not pull double-duty as item counters.

* CurrAdv: Currently loaded Adventure Mode game; progress flags only
* SaveSlot 1: slot for CurrAdv to save to.
* SaveSlot 2: Local item unlocks. Here lives CurrAdv's item inventory. Gets deleted when a new Adventure Mode run is started.
* SaveSlot 3: Copy of SaveSlot 2. Does not get deleted when a new Adventure Mode run is started. Gets copied over by SaveSlot 2 every time the player manually saves the game. Gets copied to SaveSlot 2 whenever the player manually loads a save. (don't worry about this slot, it's for internal use only)
* SaveSlot 4: Multiworld item inventory. Has the exact same data layout as SaveSlot 2, but is never written to by the game. Items written into this save file by outside tools are added to SaveSlot 2's data while calculating item counts for display in the Adventure Mode UI or for unlocking warp pads and boss garages.

For tracker and multiworld tools, the only relevant data is found in:

* CurrAdv - progress flags (trackers and multiworld tools read from here)
* SaveSlot 2 - local item inventory (trackers read from here)
* SaveSlot 4 - multiworld item inventory (multiworld tools write to here)

Note: Only read or write data when the four bytes (int) at `8008D984` read exactly `00000001`. This is basically our "it's save to read or write at the moment" marker.

### Save Slot data

The following shows how data is arranged in the 0x50 bytes that each of the 5 slots (CurrAdv, SaveSlots 1-4) consists of:

```text
struct AdvProgress
{
    // offset 0x0
    unsigned int rewards[6]; (6 * 0x4), but last 2 unused

    // offset 0x18
    char name[18]; // 0x12

    // offset 0x2A
    short characterID; // 0x2

    // offset 0x2C
    short unk; // 0x2

    // offset 0x2E
    short HubLevYouSavedOn; // 0x2

    // offset 0x30
    // Count up to 10 times player lost
    char timesLostRacePerLev[18]; // 0x12

    // offset 0x42
    char timeLostCupRace[5]; // 0x5

    // offset 0x47
    char timesLostBossRace[5]; // 0x5

    // offset 0x4C
    int unk_8FBF0;  // 0x4
}
```

The `rewards` array saves the Adventure Mode progress. The 6 ints stored here hold the following flag data:

```text
[0]
    0x1: ???
    0x2: ???
    0x4: ???
    0x8: ???
    0x10: ???
    0x20: ???
    0x40: Dingo Canyon Trophy
    0x80: Dragon Mines Trophy
    0x100: Blizzard Bluff Trophy
    0x200: Crash Cove Trophy
    0x400: Tiger Temple Trophy
    0x800: Papu's Pyramid Trophy
    0x1000: Roo's Tubes Trophy
    0x2000: Hot Air Skyway Trophy
    0x4000: Sewer Speedway Trophy
    0x8000: Mystery Caves Trophy
    0x10000: Cortex Castle Trophy
    0x20000: N Gin Labs Trophy
    0x40000: Polar Pass Trophy
    0x80000: Oxide Station Trophy
    0x100000: Coco Park Trophy
    0x200000: Tiny Arena Trophy
    0x400000: Dingo Canyon Sapphire Relic
    0x800000: Dragon Mines Sapphire Relic
    0x1000000: Blizzard Bluff Sapphre Relic
    0x2000000: Crash Cove Sapphre Relic
    0x4000000: Tiger Temple Sapphre Relic
    0x8000000: Papu's Pyramid Sapphre Relic
    0x10000000: Roo's Tubes Sapphre Relic
    0x20000000: Hot Air Skyway Sapphre Relic
    0x40000000: Sewer Speedway Sapphre Relic
    0x80000000: Mystery Caves Sapphre Relic

[1]
    0x1: Cortex Castle Sapphire Relic
    0x2: N Gin Labs Sapphire Relic
    0x4: Polar Pass Sapphire Relic
    0x8: Oxide Station Sapphire Relic
    0x10: Coco Park Sapphire Relic
    0x20: Tiny Arena Sapphire Relic
    0x40: Slide Coliseum Sapphire Relic
    0x80: Turbo Track Sapphire Relic
    0x100: Dingo Canyon Gold Relic
    0x200: Dragon Mines Gold Relic
    0x400: Blizzard Bluff Gold Relic
    0x800: Crash Cove Gold Relic
    0x1000: Tiger Temple Gold Relic
    0x2000: Papu's Pyramid Gold Relic
    0x4000: Roo's Tubes Gold Relic
    0x8000: Hot Air Skyway Gold Relic
    0x10000: Sewer Speedway Gold Relic
    0x20000: Mystery Caves Gold Relic
    0x40000: Cortex Castle Gold Relic
    0x80000: N Gin Labs Gold Relic
    0x100000: Polar Pass Gold Relic
    0x200000: Oxide Station Gold Relic
    0x400000: Coco Park Gold Relic
    0x800000: Tiny Arena Gold Relic
    0x1000000: Slide Coliseum Gold Relic
    0x2000000: Turbo Track Gold Relic
    0x4000000: Dingo Canyon Platinum Relic
    0x8000000: Dragon Mines Platinum Relic
    0x10000000: Blizzard Bluff Platinum Relic
    0x20000000: Crash Cove Platinum Relic
    0x40000000: Tiger Temple Platinum Relic
    0x80000000: Papu's Pyramid Platinum Relic

[2]
    0x1: Roo's Tubes Platinum Relic
    0x2: Hot Air Skyway Platinum Relic
    0x4: Sewer Speedway Platinum Relic
    0x8: Mystery Caves Platinum Relic
    0x10: Cortex Castle Platinum Relic
    0x20: N Gin Labs Platinum Relic
    0x40: Polar Pass Platinum Relic
    0x80: Oxide Station Platinum Relic
    0x100: Coco Park Platinum Relic
    0x200: Tiny Arena Platinum Relic
    0x400: Slide Coliseum Platinum Relic
    0x800: Turbo Track Platinum Relic
    0x1000: Dingo Canyon CTR Token
    0x2000: Dragon Mines CTR Token
    0x4000: Blizzard Bluff CTR Token
    0x8000: Crash Cove CTR Token
    0x10000: Tiger Temple CTR Token
    0x20000: Papu's Pyramid CTR Token
    0x40000: Roo's Tubes CTR Token
    0x80000: Hot Air Skyway CTR Token
    0x100000: Sewer Speedway CTR Token
    0x200000: Mystery Caves CTR Token
    0x400000: Cortex Castle CTR Token
    0x800000: N Gin Labs CTR Token
    0x1000000: Polar Pass CTR Token
    0x2000000: Oxide Station CTR Token
    0x4000000: Coco Park CTR Token
    0x8000000: Tiny Arena CTR Token
    0x10000000: ???
    0x20000000: ???
    0x40000000: Ripper Roo defeated
    0x80000000: Papu Papu defeated

[3]
    0x1: Komodo Joe defeated
    0x2: Pinstripe defeated
    0x4: Oxide 1
    0x8: Oxide 2
    0x10: N. Sanity Beach to Gemstone Valley door
    0x20: Gemstone Valley to cups door
    0x40: N. Sanity Beach to Glacier Park door
    0x80: Lost Ruins to Glacier Park door
    0x100: Glacier Park to Citadel City door
    0x200: ???
    0x400: Red Gem Cup
    0x800: Green Gem Cup
    0x1000: Blue Gem Cup
    0x2000: Yellow Gem Cup
    0x4000: Purple Gem Cup
    0x8000: Skull Rock CTR Token
    0x10000: Rampage Ruins CTR Token
    0x20000: Rocky Road CTR Token
    0x40000: Nitro Court CTR Token
    0x80000: Oxide 1
    0x100000: Oxide 2
    0x200000: ???
    0x400000: Mask Hint - Welcome to Arena
    0x800000: Mask Hint - Using a Warp Pad
    0x1000000: Mask Hint - Accessing a track
    0x2000000: Mask Hint - Accessing a boss garage
    0x4000000: Mask Hint - Entering Oxide's Ship
    0x8000000: Mask Hint - Opening world doors
    0x10000000: Mask Hint - Save / Load Screen
    0x20000000: Mask Hint - Congratulationg on opening this new area
    0x40000000: ???
    0x80000000: ???

[4]
    0x1: ???
    0x2: ???
    0x4: ???
    0x8: ???
    0x10: Mask Hint - Hang Time Turbo
    0x20: Mask Hint - Power Slide
    0x40: Mask Hint - Turbo Boost Hint
    0x80: Mask Hint - Brake Slide
    0x100: Mask Hint - Must have 2 boss keys
    0x200: ???
    0x400: ???
    0x800: ???
    0x1000: Mask Hint - Wumpa Fruit Hint
    0x2000: Mask Hint - TNT
    0x4000: Mask Hint - Map Information
    0x8000: Mask Hint - Crystal Challenge
    0x10000: Mask Hint - CTR Token Challenge
    0x20000: Mask Hint - Gem Cups
    0x40000: Mask Hint - Must get 10 relics
    0x80000: Mask Hint - Relic Challenge
    0x100000: ???
    0x200000: ???
    0x400000: ???
    0x800000: ???
    0x1000000: ???
    0x2000000: ???
    0x4000000: ???
    0x8000000: ???
    0x10000000: ???
    0x20000000: ???
    0x40000000: ???
    0x80000000: ???
```

`[5]` is entirely unused in vanilla.

### Rando item inventory

Rando uses the rewards array of SaveSlots 2, 3, 4 as follows:

```text
rewards[0] = num trophies
rewards[1] = num keys

rewards[2] & 0xFF               = num red ctr token
(rewards[2] & 0xFF00) >> 8      = num green ctr token
(rewards[2] & 0xFF0000) >> 16   = num blue ctr token
(rewards[2] & 0xFF000000) >> 24 = num yellow ctr token
rewards[3] & 0xFF               = num purple ctr token

rewards[4] & 0xFF             = num sapphire relics
(rewards[4] & 0xFF00) >> 8    = num gold relics
(rewards[4] & 0xFF0000) >> 16 = num platinum relics

rewards[5] & 0xFF   = num gems
rewards[5] & 0x100  = flag red gem
rewards[5] & 0x200  = flag green gem
rewards[5] & 0x400  = flag blue gem
rewards[5] & 0x800  = flag yellow gem
rewards[5] & 0x1000 = flag purple gem
```

## Tool-specific Data

It is possible for outside tools, like a multiworld integration, to write their own data into the save files.  
This will most likely be used for writing a multiworld's `item received sequence` into the save.

It does not actually matter where exactly in the save files the tool writes its data, as long as this does not interfere with the data handled by the randomizer mod itself.  
For this purpose, it is recommended to stick to SaveSlot 4.  
Particularly the `name` and `timesLostRacePerLev` arrays lend themselves to this, as they are never written to or read from by the randomizer mod.
