#pragma once

#include "esphome/core/component.h"

namespace esphome {
namespace wordclock {

bool change = false;

class WordClockLight : public Component {
 public:
  void set_internal_light(AddressableLight* light) {
    this->internal_light_ = light;
  }

  LightTraits get_traits() override {
      // return the traits this light supports
      auto traits = LightTraits();
      traits.set_supported_color_modes({ColorMode::RGB, ColorMode::BRIGHTNESS});
      return traits;
  }

  void write_state(LightState *state) override {
      float fred, fgreen, fblue;
      int red = 124;
      int green = 124;
      int blue = 124;
    
      // This will be called by the light to get a new state to be written.
      //float fred, fgreen, fblue;
      // use any of the provided current_values methods
      state->current_values_as_rgb(&fred, &fgreen, &fblue);
      // Write red, green and blue to HW
      // ...
      red = fred * 255;
      green = fgreen * 255;
      blue = fblue * 255;
      change = 1;
      ESP_LOGD("write_state", "New color %i %i %i", red, green, blue );
  }

 protected:
  esphome::light::AddressableLight* internal_light_;
};
} // namespace wordclock
} // namespace esphome
