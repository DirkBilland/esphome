import esphome.codegen as cg
from esphome.components import light, output
import esphome.config_validation as cv
from esphome.const import CONF_ID

wordclock_ns = cg.esphome_ns.namespace('wordclock')
WordClockLight = wordclock_ns.class_('WordClockLight', light.LightOutput)

CONF_INTERNAL_LIGHT = "internal_light"

CONFIG_SCHEMA = light.BINARY_LIGHT_SCHEMA.extend({
    cg.GenerateID(): cg.declare_id(WordClockLight),
    cg.Required(CONF_INTERNAL_LIGHT): cg.use_id(light.AddressableLight),
})

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    internal = await cg.get_variable(config[CONF_INTERNAL_LIGHT])
    cg.add(var.set_internal_light(internal))
    await light.register_light(var, config)
