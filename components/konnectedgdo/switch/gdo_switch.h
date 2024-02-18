/************************************
 *
 * Copyright (C) 2024 Konnected.io
 *
 * GNU GENERAL PUBLIC LICENSE
 ************************************/

#pragma once

#include "esphome/components/switch/switch.h"
#include "esphome/core/component.h"
#include "../gdolib.h"

namespace esphome {
namespace konnectedgdo {

    class GDOSwitch : public switch_::Switch, public Component {
    public:
        void write_state(bool state) override {
            if (state) {
                gdo_activate_learn();
            } else {
                gdo_deactivate_learn();
            }
        }
    };

} // namespace konnectedgdo
} // namespace esphome
