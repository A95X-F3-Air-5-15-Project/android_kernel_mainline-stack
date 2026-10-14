load("//build/kernel/kleaf:kernel.bzl", "kernel_module")

_STACK = "//vendor/mainline-stack"

# MediaTek MT7668 (SDIO) Wi-Fi: wlan_mt76x8_sdio.ko (SDIO function 1).
def mt7668_module(name, kernel_build, deps = None):
    kernel_module(
        name = name,
        srcs = [
            _STACK + "/mt7668:mt7668_srcs",
            _STACK + ":compat_srcs",
        ],
        makefile = [_STACK + "/mt7668:Makefile"],
        deps = deps,
        outs = [
            "wlan_mt76x8_sdio.ko",
        ],
        kernel_build = kernel_build,
    )

# MediaTek MT7668 (SDIO) Bluetooth: btmtksdio.ko (SDIO function 2) and btmtk.ko,
# from linux-6.18.y. They replace the in-tree btmtksdio, so drop that one
# from module_outs; bluetooth.ko comes from the kernel build.
def mt7668_bt_module(name, kernel_build, deps = None):
    kernel_module(
        name = name,
        srcs = [
            _STACK + "/mt7668/bluetooth:mt7668_bt_srcs",
            _STACK + "/mt7668:mt7668_bt_compat_srcs",
            _STACK + ":compat_srcs",
        ],
        makefile = [_STACK + "/mt7668/bluetooth:Makefile"],
        deps = deps,
        outs = [
            "btmtk.ko",
            "btmtksdio.ko",
        ],
        kernel_build = kernel_build,
    )

# Realtek rtw88 (SDIO) Wi-Fi from linux-6.18.y: the core, the SDIO bus
# glue and the RTL8822BS/8822CS/8821CS/8723DS/8723CS chips. Older kernels
# have no rtw88 SDIO support, so the whole driver is built here.
RTW88_SDIO_OUTS = [
    "rtw88/rtw88_8703b.ko",
    "rtw88/rtw88_8723cs.ko",
    "rtw88/rtw88_8723d.ko",
    "rtw88/rtw88_8723ds.ko",
    "rtw88/rtw88_8723x.ko",
    "rtw88/rtw88_8821c.ko",
    "rtw88/rtw88_8821cs.ko",
    "rtw88/rtw88_8822b.ko",
    "rtw88/rtw88_8822bs.ko",
    "rtw88/rtw88_8822c.ko",
    "rtw88/rtw88_8822cs.ko",
    "rtw88/rtw88_core.ko",
    "rtw88/rtw88_sdio.ko",
]

def rtw88_sdio_module(name, kernel_build, deps = None):
    kernel_module(
        name = name,
        srcs = [
            _STACK + "/rtw88:rtw88_srcs",
            _STACK + ":compat_srcs",
        ],
        makefile = [_STACK + "/rtw88:Makefile"],
        deps = deps,
        outs = RTW88_SDIO_OUTS,
        kernel_build = kernel_build,
    )
