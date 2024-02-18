/*
 * Copyright (C) 2024  Konnected Inc.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "konnected_gdo.h"
#include "esphome/core/application.h"
#include "esphome/core/log.h"

namespace esphome {
namespace konnectedgdo {

    static const char* const TAG = "konnectedGDO";

    static void gdo_event_handler(const gdo_status_t* status, gdo_cb_event_t event, void *arg) {
        GDOComponent *gdo = static_cast<GDOComponent *>(arg);
        switch (event) {
        case GDO_CB_EVENT_SYNCED:
            ESP_LOGI(TAG, "Synced: %s", status->synced ? "true" : "false");
            break;
        case GDO_CB_EVENT_LIGHT:
            ESP_LOGI(TAG, "Light: %s", gdo_light_state_to_string(status->light));
            gdo->set_light_state(status->light);
            break;
        case GDO_CB_EVENT_LOCK:
            ESP_LOGI(TAG, "Lock: %s", gdo_lock_state_to_string(status->lock));
            gdo->set_lock_state(status->lock);
            break;
        case GDO_CB_EVENT_DOOR_POSITION:
            ESP_LOGI(TAG, "Door: %s, %.2f%%, target: %.2f%%", gdo_door_state_to_string(status->door),
                    (float)status->door_position/100.0f, (float)status->door_target/100.0f);
            gdo->set_door_state(status->door, (float)status->door_position/10000.0f);
            break;
        case GDO_CB_EVENT_LEARN:
            ESP_LOGI(TAG, "Learn: %s", gdo_learn_state_to_string(status->learn));
            break;
        case GDO_CB_EVENT_OBSTRUCTION:
            ESP_LOGI(TAG, "Obstruction: %s", gdo_obstruction_state_to_string(status->obstruction));
            gdo->set_obstruction(status->obstruction);
            break;
        case GDO_CB_EVENT_MOTION:
            ESP_LOGI(TAG, "Motion: %s", gdo_motion_state_to_string(status->motion));
            gdo->set_motion_state(status->motion);
            break;
        case GDO_CB_EVENT_BATTERY:
            ESP_LOGI(TAG, "Battery: %s", gdo_battery_state_to_string(status->battery));
            break;
        case GDO_CB_EVENT_BUTTON:
            ESP_LOGI(TAG, "Button: %s", gdo_button_state_to_string(status->button));
            gdo->set_button_state(status->button);
            break;
        case GDO_CB_EVENT_MOTOR:
            ESP_LOGI(TAG, "Motor: %s", gdo_motor_state_to_string(status->motor));
            gdo->set_motor_state(status->motor);
            break;
        case GDO_CB_EVENT_OPENINGS:
            ESP_LOGI(TAG, "Openings: %d", status->openings);
            gdo->set_openings(status->openings);
            break;
        case GDO_CB_EVENT_TTC:
            ESP_LOGI(TAG, "Time to close: %d", status->ttc_seconds);
            break;
        case GDO_CB_EVENT_PAIRED_DEVICES:
            ESP_LOGI(TAG, "Paired devices: %d remotes, %d keypads, %d wall controls, %d accessories, %d total",
                    status->paired_devices.total_remotes, status->paired_devices.total_keypads,
                    status->paired_devices.total_wall_controls, status->paired_devices.total_accessories,
                    status->paired_devices.total_all);
            break;
        default:
            ESP_LOGI(TAG, "Unknown event: %d", event);
            break;
        }
    }

    void GDOComponent::setup() {
        gdo_config_t gdo_conf = {
            .uart_num = UART_NUM_1,
            .protocol = GDO_PROTOCOL == 1 ? GDO_PROTOCOL_SEC_PLUS_V1 : GDO_PROTOCOL_SEC_PLUS_V2,
            .obst_from_status = true,
            .invert_uart = true,
            .uart_tx_pin = (gpio_num_t)GDO_UART_TX_PIN,
            .uart_rx_pin = (gpio_num_t)GDO_UART_RX_PIN,
            .obst_in_pin = (gpio_num_t)GDO_OBST_INPUT_PIN,
        };

        gdo_init(&gdo_conf);
        gdo_get_status(&this->status_);
        gdo_start(gdo_event_handler, this);
        ESP_LOGI(TAG, "KonnectedGDO started!");
    }

    void GDOComponent::loop() {}

    void GDOComponent::dump_config() {
        ESP_LOGCONFIG(TAG, "Setting up KonnectedGDO...");
    }

} // namespace konnectedgdo
} // namespace esphome