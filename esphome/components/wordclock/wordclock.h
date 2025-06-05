#pragma once

#include "esphome/core/component.h"

using namespace esphome;
using namespace light;

namespace esphome {
namespace wordclock {

static const char *TAG = "example_component.component";

bool change = false;

class WordClock : public Component {
 public:
  void set_internal_light(AddressableLight* light) {
    this->internal_light_ = light;
  }

void ExampleComponent::setup() {
  // Code here should perform all component initialization,
  //  whether hardware, memory, or otherwise
}

void ExampleComponent::loop() {
  // Tasks here will be performed at every call of the main application loop.
  // Note: code here MUST NOT BLOCK (see below)
}

void ExampleComponent::dump_config(){
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



