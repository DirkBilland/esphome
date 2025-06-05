#pragma once

#include "esphome/core/component.h"

using namespace esphome;
using namespace light;

namespace esphome {
namespace wordclock {

static const char *TAG = "example_component.component";

bool change = false;

class WordClock : public Component, public LightOutput {
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

void setup() {
    // Code here should perform all component initialization,
    //  whether hardware, memory, or otherwise
  }
  
  void loop() {
    // Tasks here will be performed at every call of the main application loop.
    // Note: code here MUST NOT BLOCK (see below)
  }
  
  void dump_config(){
    ESP_LOGCONFIG(TAG, "Example component");
    //ESP_LOGCONFIG(TAG, "  foo = %s", TRUEFALSE(this->foo_));
    //ESP_LOGCONFIG(TAG, "  bar = %s", this->bar_.c_str());
    //ESP_LOGCONFIG(TAG, "  baz = %i", this->baz_);
  }

 protected:
  esphome::light::AddressableLight* internal_light_;
};
} // namespace wordclock
} // namespace esphome



