import esphome.codegen as cg
from esphome.components import light
from esphome.const import CONF_ID

wordclock_ns = cg.esphome_ns.namespace('wordclock')
WordClockLight = wordclock_ns.class_('WordClockLight', light.LightOutput)

CONF_INTERNAL_LIGHT = "internal_light"

CONFIG_SCHEMA = light.BINARY_LIGHT_SCHEMA.extend({
    cg.GenerateID(): cg.declare_id(WordClockLight),
    cg.Required(CONF_INTERNAL_LIGHT): cg.use_id(light.AddressableLight),
})
