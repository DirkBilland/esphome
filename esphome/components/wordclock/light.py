
import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import light, output
from esphome.components import neopixelbus, time as time_
from esphome.const import CONF_ID, CONF_OUTPUT_ID

wordclock_light_ns = cg.esphome_ns.namespace("wordclock_light")
WordClockLight = wordclock_light_ns.class_("WordClockLight", light.LightOutput)

CONF_INTERNAL_LIGHT = "internal_light"
CONF_TIMESOURCE = "timesource"

CONFIG_SCHEMA = light.ADDRESSABLE_LIGHT_SCHEMA.extend(
    {
        cv.GenerateID(CONF_OUTPUT_ID): cv.declare_id(WordClockLight),
        cv.Required(CONF_INTERNAL_LIGHT): cv.use_id(light.AddressableLight),
        cv.Required(CONF_TIMESOURCE): cv.use_id(time_.RealTimeClock  )
    }
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_OUTPUT_ID])
    await light.register_light(var, config)
    
    internal = await cg.get_variable(config[CONF_INTERNAL_LIGHT])
    cg.add(var.set_internal_light(internal))
    internal = await cg.get_variable(config[CONF_TIMESOURCE])
    cg.add(var.set_timesource(internal))
