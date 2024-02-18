"""
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
 """

import esphome.codegen as cg
import esphome.config_validation as cv
import voluptuous as vol
from esphome import pins
from esphome.const import CONF_ID

DEPENDENCIES = ["preferences"]
MULTI_CONF = True

konnectedgdo_ns = cg.esphome_ns.namespace("konnectedgdo")
KONNECTEDGDO = konnectedgdo_ns.class_("GDOComponent", cg.Component)

CONF_OUTPUT_GDO = "output_gdo_pin"
DEFAULT_OUTPUT_GDO = ("1")
CONF_INPUT_GDO = "input_gdo_pin"
DEFAULT_INPUT_GDO = ("2")
CONF_INPUT_OBST = "input_obst_pin"
DEFAULT_INPUT_OBST = ("5")

CONF_KONNECTEDGDO_ID = "konnectedgdo_id"
CONF_PROTOCOL = "protocol"

PROTOCOL_SECPLUSV1 = "secplusv1"
PROTOCOL_SECPLUSV2 = "secplusv2"
SUPPORTED_PROTOCOLS = [PROTOCOL_SECPLUSV1, PROTOCOL_SECPLUSV2]

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(KONNECTEDGDO),
        cv.Required(CONF_OUTPUT_GDO): pins.gpio_output_pin_schema,
        cv.Required(CONF_INPUT_GDO): pins.gpio_input_pin_schema,
        cv.Optional(CONF_INPUT_OBST, default=DEFAULT_INPUT_OBST): cv.Any(
            cv.none, pins.gpio_input_pin_schema
        ),
        cv.Optional(CONF_PROTOCOL, default=PROTOCOL_SECPLUSV2): vol.In(
            SUPPORTED_PROTOCOLS
        ),
    }
).extend(cv.COMPONENT_SCHEMA)

KONNECTED_GDO_CONFIG_SCHEMA = cv.Schema(
    {
        cv.Required(CONF_KONNECTEDGDO_ID): cv.use_id(KONNECTEDGDO),
    }
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    cg.add_define("GDO_UART_TX_PIN", config[CONF_OUTPUT_GDO]['number'])
    cg.add_define("GDO_UART_RX_PIN", config[CONF_INPUT_GDO]['number'])
    if CONF_INPUT_OBST in config and config[CONF_INPUT_OBST]:
        cg.add_define("GDO_OBST_INPUT_PIN", config[CONF_INPUT_OBST]['number'])

    if config[CONF_PROTOCOL] == PROTOCOL_SECPLUSV1:
        cg.add_define("GDO_PROTOCOL", 1)
    else:
        cg.add_define("GDO_PROTOCOL", 2)
