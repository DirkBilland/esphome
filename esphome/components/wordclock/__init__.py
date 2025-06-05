import esphome.codegen as cg
from esphome.components import light, output, neopixelbus
import esphome.config_validation as cv
from esphome.const import CONF_ID, CONF_OUTPUT_ID

wordclock_ns = cg.esphome_ns.namespace("wordclock")
WordClock = wordclock_ns.class_("WordClock", cg.Component)

CONF_INTERNAL_LIGHT = "internal_light"

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(WordClock),
    cv.Required(CONF_INTERNAL_LIGHT): cv.use_id(light.AddressableLight),
})

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    internal = await cg.get_variable(config[CONF_INTERNAL_LIGHT])
    cg.add(var.set_internal_light(internal))
