/*
 * Copyright (c) 2018, The Linux Foundation. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
#ifndef _LEDS_H_
#define _LEDS_H_

#include "core.h"

#ifdef CONFIG_ATH10K_LEDS
void ath10k_leds_unregister(struct ath10k *ar);
int ath10k_leds_start(struct ath10k *ar);
int ath10k_leds_register(struct ath10k *ar);
#else
static inline void ath10k_leds_unregister(struct ath10k *ar)
{
}

static inline int ath10k_leds_start(struct ath10k *ar)
{
	return 0;
}

static inline int ath10k_leds_register(struct ath10k *ar)
{
	return 0;
}

#endif
#endif /* _LEDS_H_ */
