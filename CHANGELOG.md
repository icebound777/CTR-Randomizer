# Changelog

## alpha 3

### General Changes

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

### Bug Fixes

* Gem Cups
  * Exiting out of a gem cup, or losing a gem cup, should no longer place the player into an unloaded hub. This only happened if gem cup warp pads were shuffled into locations outside of Gemstone Valley.
* Oxide Cutscene
  * The "true ending" post-Oxide cutscene is no longer triggered by having won the Sewer Speedway CTR Challenge. Instead requires beating Oxide's Final Challenge.
* Credits
  * The "true ending" cutscene should no longer play if the player has beaten all sapphire relic times. Instead requires beating Oxide's Final Challenge.
  * The "true ending with confetti" aka "101% credits" should no longer play if the player has beaten all gold relic times. Instead actually requires 101% completion.
