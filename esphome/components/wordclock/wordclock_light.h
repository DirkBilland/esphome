#include "esphome.h"

using namespace esphome;
using namespace light;

class WordClockLight : public LightOutput {
 public:
  void set_internal_light(esphome::light::AddressableLight* light) {
    this->internal_light_ = light;
  }

  esphome::light::LightTraits get_traits() override {
      // return the traits this light supports
      auto traits = esphome::light::LightTraits();
      traits.set_supported_color_modes({esphome::light::ColorMode::RGB, esphome::light::ColorMode::BRIGHTNESS});
      return traits;
  }

  void write_state(esphome::light::LightState* state) override {
    float r, g, b;
    state->current_values_as_rgb(&r, &g, &b);

    // Beispiel: LEDs 0–5 für „ES IST“
    esphome::Color color = esphome::Color::from_rgb(r * 255, g * 255, b * 255);
    this->internal_light_->all().fade_to_black(); // Alles aus
    this->internal_light_->range(0, 5) = color;
    this->internal_light_->schedule_show();
  }

 protected:
  esphome::light::AddressableLight* internal_light_;
};
