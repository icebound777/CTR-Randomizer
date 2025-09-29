# Mod-specific Setup

To make this mod compile, make the following change to the CTR-ModSDK files:

* `CTR-ModSDK/include/ovr_232.h` > `struct WarpPad`: remove `#if 0, #endif` that comments out the three specLight arrays.
