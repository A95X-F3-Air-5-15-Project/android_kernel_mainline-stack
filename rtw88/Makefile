# SPDX-License-Identifier: GPL-2.0 OR BSD-3-Clause
#
# Out-of-tree build of the Realtek rtw88 Wi-Fi driver for SDIO chips.
#
#   rtw88/      kept identical to unifreq/linux-6.18.y
#               drivers/net/wireless/realtek/rtw88
#   compat/     everything needed to build rtw88/ on older kernels
#
# Older kernels have no rtw88 SDIO support at all, so the whole driver
# (core included) is built here instead of the in-tree one.
#

ifneq ($(KERNELRELEASE),)

RTW88_ROOT := $(abspath $(srctree)/$(src))

# rtw88/Makefile is the upstream Kbuild file: pick the SDIO chips.
export CONFIG_RTW88_CORE := m
export CONFIG_RTW88_SDIO := m
export CONFIG_RTW88_8822B := m
export CONFIG_RTW88_8822BS := m
export CONFIG_RTW88_8822C := m
export CONFIG_RTW88_8822CS := m
export CONFIG_RTW88_8821C := m
export CONFIG_RTW88_8821CS := m
export CONFIG_RTW88_8723X := m
export CONFIG_RTW88_8723D := m
export CONFIG_RTW88_8723DS := m
export CONFIG_RTW88_8703B := m
export CONFIG_RTW88_8723CS := m
obj-m += rtw88/

# compat.h is force-included into every rtw88/ object, so the driver
# sources themselves never need to know which kernel they build for.
subdir-ccflags-y += -include $(RTW88_ROOT)/compat/compat.h \
		    -I$(RTW88_ROOT)/compat/include

else

KERNEL_SRC ?= /lib/modules/$(shell uname -r)/build
M ?= $(shell pwd)

.PHONY: default modules modules_install clean

default: modules

modules modules_install clean:
	$(MAKE) -C $(KERNEL_SRC) M=$(M) $@

endif
