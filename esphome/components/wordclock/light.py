import esphome.codegen as cg
from esphome.components import light, output
import esphome.config_validation as cv
from esphome.const import CONF_ID, CONF_OUTPUT_ID

wordclocklight_ns = cg.esphome_ns.namespace('wordclock')
WordClockLight = wordclocklight_ns.class_('WordClockLight', light.LightOutput)

CONF_INTERNAL_LIGHT = "internal_light"

CONFIG_SCHEMA = light.ADDRESSABLE_LIGHT_SCHEMA.extend({
    cv.GenerateID(CONF_OUTPUT_ID): cv.declare_id(WordClockLight),
    cv.Required(CONF_INTERNAL_LIGHT): cv.use_id(light.AddressableLight),
})

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_OUTPUT_ID])
    await light.register_light(var, config)
    internal = await cg.get_variable(config[CONF_INTERNAL_LIGHT])
    cg.add(var.set_internal_light(internal))
