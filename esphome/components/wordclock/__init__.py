
import esphome.codegen as cg
from esphome.components import light, output, neopixelbus
import esphome.config_validation as cv
from esphome.const import CONF_ID, CONF_OUTPUT_ID

wordclock_light_ns = cg.esphome_ns.namespace("wordclock_light")
WordClockLight = wordclock_light_ns.class_("WordClockLight", cg.Component)

CONF_INTERNAL_LIGHT = "internal_light"

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(WordClockLight),
    cv.Required(CONF_INTERNAL_LIGHT): cv.use_id(light.AddressableLight),
    cv.Required(CONF_TIME): cv.use_id(homeassistant::HomeassistantTime)
})

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    internal = await cg.get_variable(config[CONF_INTERNAL_LIGHT])
    cg.add(var.set_internal_light(internal))
