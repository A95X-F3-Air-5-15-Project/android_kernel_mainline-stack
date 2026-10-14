/* SPDX-License-Identifier: GPL-2.0 OR BSD-3-Clause */
/*
 * Backports the kernel APIs that rtw88/ (written for Linux 6.18) uses on
 * older kernels. Force-included into every rtw88/ object by the top
 * Makefile, so the driver sources stay identical to upstream.
 *
 * Written against the Android common kernel android13-5.15, which carries
 * some later mac80211 changes (e.g. struct ieee80211_link_sta), so the
 * version checks below describe that kernel rather than plain v5.15.
 */
#ifndef _RTW88_COMPAT_H
#define _RTW88_COMPAT_H

#include <linux/version.h>

#endif /* _RTW88_COMPAT_H */
