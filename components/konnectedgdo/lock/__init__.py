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
from esphome.components import lock
from esphome.const import CONF_ID

from .. import KONNECTED_GDO_CONFIG_SCHEMA, konnectedgdo_ns, CONF_KONNECTEDGDO_ID

DEPENDENCIES = ["konnectedgdo"]

GDOLock = konnectedgdo_ns.class_("GDOLock", lock.Lock, cg.Component)

CONFIG_SCHEMA = lock.LOCK_SCHEMA.extend(
    {
        cv.GenerateID(): cv.declare_id(GDOLock),
    }
).extend(KONNECTED_GDO_CONFIG_SCHEMA)



async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await lock.register_lock(var, config)
    await cg.register_component(var, config)
    parent = await cg.get_variable(config[CONF_KONNECTEDGDO_ID])
    text = "std::bind(&" + str(GDOLock) + "::set_state," + str(config[CONF_ID]) + ",std::placeholders::_1)"
    cg.add(parent.register_lock(cg.RawExpression(text)))