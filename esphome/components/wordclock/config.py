from . import WordClockLight
import esphome.config_validation as cv
import esphome.codegen as cg
from esphome.const import CONF_ID
from esphome.components import light
from . import CONF_INTERNAL_LIGHT

CONFIG_SCHEMA = light.BINARY_LIGHT_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(WordClockLight),
    cv.Required(CONF_INTERNAL_LIGHT): cv.use_id(light.AddressableLight),
})

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    internal = await cg.get_variable(config[CONF_INTERNAL_LIGHT])
    cg.add(var.set_internal_light(internal))
    await light.register_light(var, config)
