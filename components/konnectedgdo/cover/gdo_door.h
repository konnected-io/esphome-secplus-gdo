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

#pragma once

#include "esphome/components/cover/cover.h"
#include "esphome/core/component.h"
#include "../gdolib.h"

namespace esphome {
namespace konnectedgdo {

using namespace esphome::cover;

    class GDODoor : public cover::Cover, public Component {
    public:
        cover::CoverTraits get_traits() override {
            auto traits = CoverTraits();
            traits.set_supports_stop(true);
            traits.set_supports_toggle(true);
            traits.set_supports_position(true);
            return traits;
        }

        void set_state(gdo_door_state_t state, float position) {
            bool save_to_flash = false; //true;
            switch (state) {
            case GDO_DOOR_STATE_OPEN:
                this->position = COVER_OPEN;
                this->current_operation = COVER_OPERATION_IDLE;
                break;
            case GDO_DOOR_STATE_CLOSED:
                this->position = COVER_CLOSED;
                this->current_operation = COVER_OPERATION_IDLE;
                break;
            case GDO_DOOR_STATE_OPENING:
                this->current_operation = COVER_OPERATION_OPENING;
                this->position = position;
                save_to_flash = false;
                break;
            case GDO_DOOR_STATE_CLOSING:
                this->current_operation = COVER_OPERATION_CLOSING;
                this->position = position;
                save_to_flash = false;
                break;
            case GDO_DOOR_STATE_STOPPED:
                this->current_operation = COVER_OPERATION_IDLE;
                this->position = position;
                break;
            case GDO_DOOR_STATE_MAX:
            default:
                this->current_operation = COVER_OPERATION_IDLE;
                this->position = position;
                break;
            }

            this->publish_state(save_to_flash);
        }

    protected:
        void control(const cover::CoverCall& call) override {
            if (call.get_stop()) {
                gdo_door_stop();
            }
            if (call.get_toggle()) {
                gdo_door_toggle();
            }
            if (call.get_position().has_value()) {
                auto pos = *call.get_position();
                if (pos == COVER_OPEN) {
                    gdo_door_open();
                } else if (pos == COVER_CLOSED) {
                    gdo_door_close();
                } else {
                    gdo_door_move_to_target(pos * 10000);
                }
            }
        }
    };

} // namespace konnectedgdo
} // namespace esphome
