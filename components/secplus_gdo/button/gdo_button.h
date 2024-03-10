/************************************
 *
 * Copyright (C) 2024 Konnected.io
 *
 * GNU GENERAL PUBLIC LICENSE
 ************************************/

#pragma once

#include "esphome/components/button/button.h"
#include "esphome/core/component.h"
#include "../gdolib.h"

namespace esphome {
namespace secplus_gdo {

    class GDOCloseButton : public button::Button, public Component {
    public:
        void press_action() override {
            gdo_door_close();
        }
    };

} // namespace secplus_gdo
} // namespace esphome
