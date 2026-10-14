# mainline-stack

Wireless drivers from mainline-based Linux (unifreq/linux-6.18.y), built
out of tree for the Android common kernel android13-5.15 (kleaf).

| Directory  | Driver                                   | Modules |
|------------|------------------------------------------|---------|
| `mt7668/`  | MediaTek MT7668 SDIO Wi-Fi and Bluetooth | `wlan_mt76x8_sdio`, `btmtk`, `btmtksdio` |
| `rtw88/`   | Realtek rtw88 SDIO Wi-Fi (8822BS/CS, 8821CS, 8723DS/CS) | `rtw88_*` |
| `brcm/`    | Broadcom/Cypress SDIO Wi-Fi and UART Bluetooth: firmware only, the drivers (brcmfmac, hci_bcm) are the kernel's own | — |

Each driver keeps the upstream sources unchanged where it can and
force-includes its own `compat/compat.h`, which backports the kernel APIs
the sources need; what it cannot cover is guarded by kernel version in the
sources. Firmware lives next to each driver in `firmware/`.

The kleaf rules are in `defs.bzl`.
