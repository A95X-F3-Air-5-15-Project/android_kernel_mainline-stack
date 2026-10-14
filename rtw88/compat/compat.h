/* SPDX-License-Identifier: GPL-2.0 OR BSD-3-Clause */
/*
 * Backports the kernel APIs that rtw88/ (written for Linux 6.18) uses on
 * older kernels. Force-included into every rtw88/ object by the top
 * Makefile.
 *
 * Written against the Android common kernel android13-5.15, which carries
 * some later mac80211 changes (e.g. struct ieee80211_link_sta), so the
 * version checks below describe that kernel rather than plain v5.15.
 *
 * What cannot be done here (callbacks whose signature changed, struct
 * members that do not exist yet) is guarded by kernel version in the
 * sources; see the commit that builds rtw88 on Linux 5.15.
 */
#ifndef _RTW88_COMPAT_H
#define _RTW88_COMPAT_H

#include <linux/version.h>
#include <linux/bitfield.h>
#include <linux/device.h>
#include <linux/mmc/sdio_func.h>
#include <linux/mmc/sdio_ids.h>
#include <linux/overflow.h>
#include <linux/skbuff.h>
#include <linux/slab.h>
#include <linux/timer.h>
#include <net/cfg80211.h>
#include <net/mac80211.h>

/* ------------------------------------------------------------------ */
/* Core kernel                                                         */
/* ------------------------------------------------------------------ */

#if LINUX_VERSION_CODE < KERNEL_VERSION(6, 2, 0)
#define timer_delete_sync(t)	del_timer_sync(t)
#define timer_delete(t)		del_timer(t)
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(6, 16, 0)
#define timer_container_of(var, callback_timer, timer_fieldname) \
	from_timer(var, callback_timer, timer_fieldname)
#endif

/* kmalloc_obj(*ptr) and kmalloc_obj(*ptr, gfp), GFP_KERNEL by default */
#ifndef kmalloc_obj
#define __rtw88_compat_gfp(dummy, gfp, ...)	gfp
#define kmalloc_obj(var, ...) \
	kmalloc(sizeof(var), __rtw88_compat_gfp(0, ##__VA_ARGS__, GFP_KERNEL))
#define kzalloc_obj(var, ...) \
	kzalloc(sizeof(var), __rtw88_compat_gfp(0, ##__VA_ARGS__, GFP_KERNEL))
#endif

#ifndef FIELD_PREP_CONST
/* FIELD_PREP() usable in constant expressions, without its checks */
#define FIELD_PREP_CONST(_mask, _val) \
	((((typeof(_mask))(_val)) << __bf_shf(_mask)) & (_mask))
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(6, 14, 0)
static inline void *devm_kmemdup_array(struct device *dev, const void *src,
				       size_t n, size_t size, gfp_t flags)
{
	return devm_kmemdup(dev, src, size_mul(n, size), flags);
}
#endif

/* ------------------------------------------------------------------ */
/* SDIO                                                                */
/* ------------------------------------------------------------------ */

#ifndef SDIO_VENDOR_ID_REALTEK
#define SDIO_VENDOR_ID_REALTEK			0x024c
#endif
#ifndef SDIO_DEVICE_ID_REALTEK_RTW8723DS_2ANT
#define SDIO_DEVICE_ID_REALTEK_RTW8723DS_2ANT	0xd723
#endif
#ifndef SDIO_DEVICE_ID_REALTEK_RTW8723DS_1ANT
#define SDIO_DEVICE_ID_REALTEK_RTW8723DS_1ANT	0xd724
#endif
#ifndef SDIO_DEVICE_ID_REALTEK_RTW8821DS
#define SDIO_DEVICE_ID_REALTEK_RTW8821DS	0xd821
#endif
#ifndef SDIO_DEVICE_ID_REALTEK_RTW8723CS
#define SDIO_DEVICE_ID_REALTEK_RTW8723CS	0xb703
#endif
#ifndef SDIO_DEVICE_ID_REALTEK_RTW8821CS
#define SDIO_DEVICE_ID_REALTEK_RTW8821CS	0xc821
#endif
#ifndef SDIO_DEVICE_ID_REALTEK_RTW8822BS
#define SDIO_DEVICE_ID_REALTEK_RTW8822BS	0xb822
#endif
#ifndef SDIO_DEVICE_ID_REALTEK_RTW8822CS
#define SDIO_DEVICE_ID_REALTEK_RTW8822CS	0xc822
#endif

/*
 * struct sdio_driver got a shutdown callback of its own after 6.18
 * (linux-6.18.y here carries it); before, the chip drivers set the driver
 * core one, which takes the device.
 */
#if LINUX_VERSION_CODE < KERNEL_VERSION(6, 18, 0)
#define RTW88_COMPAT_SDIO_DRIVER_NO_SHUTDOWN
#endif

void rtw_sdio_shutdown(struct sdio_func *sdio_func);

static inline void rtw_sdio_compat_shutdown(struct device *dev)
{
	rtw_sdio_shutdown(dev_to_sdio_func(dev));
}

/* ------------------------------------------------------------------ */
/* cfg80211 and mac80211                                               */
/* ------------------------------------------------------------------ */

#if LINUX_VERSION_CODE < KERNEL_VERSION(6, 0, 0)
/*
 * The interface configuration that is not per link (assoc, aid, ps) moved
 * from bss_conf to cfg in 6.0; older kernels have the same members in
 * bss_conf. rtw88 has no other "cfg" that would clash with bss_conf.
 */
#define cfg bss_conf
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(6, 1, 0)
static inline bool cfg80211_ssid_eq(struct cfg80211_ssid *a,
				    struct cfg80211_ssid *b)
{
	if (WARN_ON(!a || !b))
		return false;
	if (WARN_ON(a->ssid_len > IEEE80211_MAX_SSID_LEN ||
		    b->ssid_len > IEEE80211_MAX_SSID_LEN))
		return false;

	return a->ssid_len == b->ssid_len &&
	       memcmp(a->ssid, b->ssid, a->ssid_len) == 0;
}
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(6, 1, 0)
/* The primary channel from the DS Parameter Set or the HT Operation. */
static inline int cfg80211_get_ies_channel_number(const u8 *ie, size_t ielen,
						  enum nl80211_band band)
{
	const struct element *tmp;

	if (band == NL80211_BAND_S1GHZ)
		return -1;

	tmp = cfg80211_find_elem(WLAN_EID_DS_PARAMS, ie, ielen);
	if (tmp && tmp->datalen == 1)
		return tmp->data[0];

	tmp = cfg80211_find_elem(WLAN_EID_HT_OPERATION, ie, ielen);
	if (tmp && tmp->datalen >= sizeof(struct ieee80211_ht_operation)) {
		const struct ieee80211_ht_operation *htop = (void *)tmp->data;

		return htop->primary_chan;
	}

	return -1;
}
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(6, 4, 0)
static inline void ieee80211_purge_tx_queue(struct ieee80211_hw *hw,
					    struct sk_buff_head *skbs)
{
	struct sk_buff *skb;

	while ((skb = skb_dequeue(skbs)))
		ieee80211_free_txskb(hw, skb);
}
#endif

/* Since 6.0 these take the link ID (-1 or 0 for non-MLO interfaces). */
#if LINUX_VERSION_CODE < KERNEL_VERSION(6, 0, 0)
#define ieee80211_beacon_get_tim(hw, vif, tim_offset, tim_length, link_id) \
	ieee80211_beacon_get_tim(hw, vif, tim_offset, tim_length)
#define ieee80211_nullfunc_get(hw, vif, link_id, qos_ok) \
	ieee80211_nullfunc_get(hw, vif, qos_ok)
#define ieee80211_request_smps(vif, link_id, smps_mode) \
	ieee80211_request_smps(vif, smps_mode)
#endif

/*
 * Drivers on older kernels without chanctx ops get the channel context
 * emulation from mac80211 itself: leave the ops empty.
 */
#if LINUX_VERSION_CODE < KERNEL_VERSION(6, 9, 0)
#define ieee80211_emulate_add_chanctx		NULL
#define ieee80211_emulate_remove_chanctx	NULL
#define ieee80211_emulate_change_chanctx	NULL
#define ieee80211_emulate_switch_vif_chanctx	NULL
#endif

#endif /* _RTW88_COMPAT_H */
