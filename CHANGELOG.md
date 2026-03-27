# Changelog

## beta 4

* Bug Fixes
  * Fix the gems showing as unlocked in the pause menu after winning their original gem cup, even if the player never actually found the gem item. These "fake gems" did not count for warp pad unlocks, though, so this was a purely visual bug.

## beta 3

* Bug Fixes
  * Fix the background of the `SAVING TO MEMORY CARD` screen not being transparent.
  * Fix issue introduced in `beta 2` that played the "post-opening a door" mask hint if mask hints were turned off, causing the crash to actually happen to more players.

## beta 2

### Bug Fixes

* Mask Hints
  * Fix a possible game crash after opening the first key door, if `Skip Mask Hints` was turned off. Fixed by always skipping the "opening first door" mask hint, even if `Skip Mask Hints` is disabled.

## beta 1

### Bug Fixes (beta 1)

* Relic Races
  * Fix 10 second bonus counter for perfect runs resetting to `-10` after counting to zero.
  * Fix 10 second bonus counter for perfect runs staying on screen instead of vanishing.
  * Fix `PERFECT` text for perfect runs not doing a fade-out to the right-side screen edge.
* Adventure Mode save screen
  * Fix not being able to close the green save screen by pressing Triangle or Square

## alpha 3

### General Changes (alpha 3)

* CTR Challenges
  * Hide the CTR token at the end of a successful CTR Challenge
  * Change the displayed text from `CTR TOKEN AWARDED` to `LETTERS COLLECTED`
* Crystal Challenges
  * Hide the CTR token at the end of a successful Crystal Challenge
  * Change the displayed text from `CTR TOKEN AWARDED` to `CRYSTALS COLLECTED`
* Relic Races
  * Hide the Relic at the end of a successful Relic Race
  * Change the displayed text from `RELIC AWARDED!` to `<type> TIME BEATEN` (type being `SAPPHIRE`, `GOLD`, `PLATINUM`)

## alpha 2

### Bug Fixes (alpha 2)

* Gem Cups
  * Exiting out of a gem cup, or losing a gem cup, should no longer place the player into an unloaded hub. This only happened if gem cup warp pads were shuffled into locations outside of Gemstone Valley.
* Oxide Cutscene
  * The "true ending" post-Oxide cutscene is no longer triggered by having won the Sewer Speedway CTR Challenge. Instead requires beating Oxide's Final Challenge.
* Credits
  * The "true ending" cutscene should no longer play if the player has beaten all sapphire relic times. Instead requires beating Oxide's Final Challenge.
  * The "true ending with confetti" aka "101% credits" should no longer play if the player has beaten all gold relic times. Instead actually requires 101% completion.
