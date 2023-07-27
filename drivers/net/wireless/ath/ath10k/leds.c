/*
 * Copyright (c) 2005-2011 Atheros Communications Inc.
 * Copyright (c) 2011-2017 Qualcomm Atheros, Inc.
 * Copyright (c) 2018 Sebastian Gottschall <s.gottschall@dd-wrt.com>
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

#include <linux/leds.h>

#include "core.h"
#include "wmi.h"
#include "wmi-ops.h"

#include "leds.h"

static int ath10k_leds_set_brightness_blocking(struct led_classdev *led_cdev,
					       enum led_brightness brightness)
{
	struct ath10k *ar = container_of(led_cdev, struct ath10k,
					 leds.cdev);
	struct gpio_led *led = &ar->leds.wifi_led;

	mutex_lock(&ar->conf_mutex);

	if (ar->state != ATH10K_STATE_ON)
		goto out;

	ar->leds.gpio_state_pin = (brightness != LED_OFF) ^ led->active_low;
	ath10k_wmi_gpio_output(ar, led->gpio, ar->leds.gpio_state_pin);

out:
	mutex_unlock(&ar->conf_mutex);

	return 0;
}

int ath10k_leds_start(struct ath10k *ar)
{
	if (ar->hw_params.led_pin == 0)
		/* leds not supported */
		return 0;

	/* under some circumstances, the gpio pin gets reconfigured
	 * to default state by the firmware, so we need to
	 * reconfigure it this behaviour has only ben seen on
	 * QCA9984 and QCA99XX devices so far
	 */
	ath10k_wmi_gpio_config(ar, ar->hw_params.led_pin, 0,
			       WMI_GPIO_PULL_NONE, WMI_GPIO_INTTYPE_DISABLE);
	ath10k_wmi_gpio_output(ar, ar->hw_params.led_pin, 1);

	return 0;
}

int ath10k_leds_register(struct ath10k *ar)
{
	int ret;

	if (ar->hw_params.led_pin == 0)
		/* leds not supported */
		return 0;

	snprintf(ar->leds.label, sizeof(ar->leds.label), "ath10k-%s",
		 wiphy_name(ar->hw->wiphy));
	ar->leds.wifi_led.active_low = 1;
	ar->leds.wifi_led.gpio = ar->hw_params.led_pin;
	ar->leds.wifi_led.name = ar->leds.label;
	ar->leds.wifi_led.default_state = LEDS_GPIO_DEFSTATE_KEEP;

	ar->leds.cdev.name = ar->leds.label;
	ar->leds.cdev.brightness_set_blocking = ath10k_leds_set_brightness_blocking;
	ar->leds.cdev.default_trigger = ar->led_default_trigger;

	ret = led_classdev_register(wiphy_dev(ar->hw->wiphy), &ar->leds.cdev);
	if (ret)
		return ret;

	return 0;
}

void ath10k_leds_unregister(struct ath10k *ar)
{
	if (ar->hw_params.led_pin == 0)
		/* leds not supported */
		return;

	led_classdev_unregister(&ar->leds.cdev);
}

