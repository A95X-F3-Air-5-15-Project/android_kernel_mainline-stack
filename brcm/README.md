# brcm

Firmware for the Broadcom/Cypress (Ampak) SDIO Wi-Fi and UART Bluetooth
modules of the TV boxes. The drivers are the mainline ones from the kernel
tree itself (brcmfmac, hci_uart with hci_bcm, btbcm), which the board
build enables; only their firmware lives here, under the names they ask
for (`/vendor/firmware/brcm/...`).

| File | Chip (module) | Source |
|------|---------------|--------|
| `brcmfmac4354-sdio.{bin,clm_blob}` | BCM4354 | linux-firmware 20260916 (Cypress) |
| `brcmfmac4356-sdio.{bin,clm_blob}` | BCM4356 (AP6356S) | linux-firmware 20260916 (Cypress) |
| `brcmfmac4356-sdio.txt` | AP6356S NVRAM | linux-firmware `brcmfmac4356-sdio.AP6356S.txt` |
| `brcmfmac43455-sdio.{bin,clm_blob}` | BCM43455 (AP6255) | linux-firmware 20260916 (Cypress) |
| `brcmfmac43455-sdio.txt` | AP6255 NVRAM | stock H96 Max X3 firmware, `nvram_ap6255.txt` |
| `BCM4354.hcd`, `BCM4345C0.hcd`, `BCM4345C5.hcd` | Bluetooth patch RAM | stock H96 Max X3 firmware, `/vendor/etc/bluetooth` |

brcmfmac looks for `brcmfmac<chip>-sdio.<root compatible>.txt` before
`brcmfmac<chip>-sdio.txt`, so a box with its own NVRAM can add one named
after the `compatible` of its DTB.
