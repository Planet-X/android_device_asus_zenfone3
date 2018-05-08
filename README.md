Unified TWRP device tree for ASUS Zenfone 3 (ZE520KL/ZE552KL)
========================================================

For building TWRP for Asus Zenfone 3 (ZE520KL and ZE552KL) only.

Manifest: 
https://github.com/minimal-manifest-twrp/platform_manifest_twrp_omni/tree/twrp-8.1

To build:
```sh
. build/envsetup.sh
export ALLOW_MISSING_DEPENDENCIES=true
lunch omni_zenfone3-userdebug
mka recoveryimage
```
