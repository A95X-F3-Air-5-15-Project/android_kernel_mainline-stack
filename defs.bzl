load("//build/kernel/kleaf:kernel.bzl", "kernel_module")

# Realtek rtw88 (SDIO) Wi-Fi from linux-6.18.y: the core, the SDIO bus
# glue and the RTL8822BS/8822CS/8821CS/8723DS/8723CS chips.
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
        srcs = ["//vendor/realtek/rtw88:rtw88_srcs"],
        makefile = ["//vendor/realtek/rtw88:Makefile"],
        deps = deps,
        outs = RTW88_SDIO_OUTS,
        kernel_build = kernel_build,
    )
