#pragma once

#include "esphome.h"
#include "esphome/core/component.h"

//using namespace esphome;
//using namespace light;
//using namespace time;

namespace esphome {
namespace wordclock_light {

static const char *TAG = "wordclock_light.component";

int leds_time_it_is[] = {20, -1, -1, -1, -1}; // ES IST
int leds_minutes[] = {124, 123, 122, 121}; // Minutes LEDS
int leds_skiped[] = {110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120}; // Check Line 101
int leds_time_minutes[][3] = {
    {  4,  -1,  -1}, // UHR
    { 21,  17,  -1}, // FÜNF, NACH
    { 22,  17,  -1}, // ZEHN, NACH
    { 23,  17,  -1}, // VIERTEL, NACH
    { 24,  17,  -1}, // ZWANZIG, NACH
    { 21,  18,  16}, // FÜNF, VOR, HALB
    { 16,  -1,  -1}, // HALB
    { 21,  17,  16}, // FÜNF, NACH, HALB
    { 24,  18,  -1}, // ZWANZIG, VOR
    { 19,  -1,  -1}, // DREIVIERTEL
    { 22,  18,  -1}, // ZEHN, VOR
    { 21,  18,  -1}  // FÜNF, VOR
};

int leds_time_hours[][6] = {
    {  3,  -1,  -1,  -1,  -1,  -1}, // ZWÖLF
    { 12,  -1,  -1,  -1,  -1,  -1}, // EINS
    { 13,  -1,  -1,  -1,  -1,  -1}, // ZWEI
    { 14,  -1,  -1,  -1,  -1,  -1}, // DREI
    {  9,  -1,  -1,  -1,  -1,  -1}, // VIER
    {  8,  -1,  -1,  -1,  -1,  -1}, // FÜNF
    {  7,  -1,  -1,  -1,  -1,  -1}, // SECHS
    {  6,  -1,  -1,  -1,  -1,  -1}, // SIEBEN
    {  5,  -1,  -1,  -1,  -1,  -1}, // ACHT
    {  0,  -1,  -1,  -1,  -1,  -1}, // NEUN
    {  1,  -1,  -1,  -1,  -1,  -1}, // ZEHN
    {  2,  -1,  -1,  -1,  -1,  -1}, // ELF
    { 11,  -1,  -1,  -1,  -1,  -1}  // EIN
};


int red = 124;
int green = 124;
int blue = 124;
int brightness = 50;
    
int hour = -1;
int minute = -1;
int second = -1;

bool change = false;

class WordClockLight : public light::LightOutput, public Component {
 public:
  void set_internal_light(light::AddressableLight* light) {
    this->internal_light_ = light;
  }
  void set_timesource(time::RealTimeClock* timesource) {
    this->timesource_ = timesource;
  }

  light::LightTraits get_traits() override {
      // return the traits this light supports
      auto traits = light::LightTraits();
      traits.set_supported_color_modes({light::ColorMode::RGB});
      return traits;
  }

  void write_state(light::LightState *state) override {
      internal_light_->update_state(state);
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

  void setup() override {
    ESP_LOGD("setup", "Setup started");
    internal_light_->setup();
    // internal_light_->all().set(Color::BLACK);
    // internal_light_->all().set(Color(red, 0, 0)); internal_light_->schedule_show(); 
    // internal_light_->all().set(Color(0, green, 0)); internal_light_->schedule_show(); 
    // internal_light_->all().set(Color(0, 0, blue)); internal_light_->schedule_show(); 
    // internal_light_->all().set(Color::BLACK); internal_light_->schedule_show();
    ESP_LOGD("setup", "Setup done!");    
  }
  
  void loop() override {
      ESP_LOGD("loop", "Loop started");
      //auto time = id(current_time).now();
      auto time = ESPTime::from_epoch_local( timesource_->timestamp_now() + 150);
      int h = time.hour;
      int m = time.minute;
      int s = time.second;

      //internal_light_->current_values_as_rgb(&fred, &fgreen, &fblue);
      //convert float 0.0 till 1.0 into int 0 till 255
      red = fred * 255;
      green = fgreen * 255;
      blue = fblue * 255;

      //auto ledlight = id(boxclocklight).remote_values;
      // https://www.esphome.io/api/classesphome_1_1light_1_1_light_color_values.html LightColorValues Class

      // red = (int)(ledlight.get_red()*255);
      // green = (int)(ledlight.get_green()*255);
      // blue = (int)(ledlight.get_blue()*255);

       //check if valid time. Blink red,green,blue until valid time is present
      if (time.is_valid() == false) {
          ESP_LOGE("loop", "Got invalid time from current_time Time: %i:%i", h, m );
          // (*internal_light_)[0] = Color(255, 0, 0); internal_light_->schedule_show(); 
          // (*internal_light_)[0] = Color(0, 255, 0); internal_light_->schedule_show(); 
          // (*internal_light_)[0] = Color(0, 0, 255); internal_light_->schedule_show(); 

          // strip.SetPixelColor(0, RgbColor(255, 0, 0)); strip.Show(); delay(250);
          // strip.SetPixelColor(0, RgbColor(0, 255, 0)); strip.Show(); delay(250);
          // strip.SetPixelColor(0, RgbColor(0, 0, 255)); strip.Show(); delay(250);
          // strip.SetPixelColor(0, RgbColor(0, 0, 0));   strip.Show();
       }
       else {
          // only update once in a Minute
//                if(h != hour || m != minute) {
          if ((s != second) || change) {
              ESP_LOGD("loop", "Using b: %i rgb %i %i %i", brightness, red, green, blue);
              hour = h;
              minute = m;  
              second = s;
              change = 0;
              if (hour >= 0 && time.is_valid() == true){
                  int tmp_hour = hour;
                  int tmp_minute = (minute - (minute % 5));
                  if(tmp_minute >= 25) { tmp_hour += 1; }
                  tmp_minute = tmp_minute / 5;
                  tmp_hour = tmp_hour % 12;
                  if ((tmp_hour == 1) && (tmp_minute == 0))  //"EINS" anstelle von "EIN" verwenden
                      tmp_hour += 11;  
                  int minutessum = minute % 5;
                  // Reset all LED, but skip LED 110 till 120
                  //for(int i = 0; i < internal_light_->size(); i++) {     if(i < 110 || i > 120) (*internal_light_)[i] = Color(0, 0, 0); }
                  internal_light_->all() = Color::BLACK;
                  for(int i = 0; i < 5; i++) {            (*internal_light_)[leds_time_it_is[i]] = Color(red, green, blue); }
                  for(int i = 0; i < 3; i++) {           if(leds_time_minutes[tmp_minute][i] >= 0) { (*internal_light_)[leds_time_minutes[tmp_minute][i]] = Color(red, green, blue); } }
                  for(int i = 0; i < 6; i++) {            if(leds_time_hours[tmp_hour][i] >= 0) { (*internal_light_)[leds_time_hours[tmp_hour][i]] = Color(red, green, blue); } }
                  //for(int i = 0; i < minutessum; i++) {   leds[leds_minutes[i]].setRGB(red, green, blue);}
                  internal_light_->schedule_show();
                  //this->update_state();
                
                 ESP_LOGD("loop", "Update Time: %i:%i  Brightness: %i RGB: %i-%i-%i", hour, minute, brightness, red, green, blue);
                 ESP_LOGD("loop", "Using tmp_hour: %i tmp_minute: %i minutessum: %i", tmp_hour, tmp_minute, minutessum);
                 
//                   for(int i = 0; i < PixelCount; i++) {     if(i < 110 || i > 120) strip.SetPixelColor(i, RgbColor(0, 0, 0)); }
//                   for(int i = 0; i < 5; i++) {            strip.SetPixelColor(leds_time_it_is[i], RgbColor(red, green, blue)); }
//                   for(int i = 0; i < 3; i++) {           if(leds_time_minutes[tmp_minute][i] >= 0) { strip.SetPixelColor(leds_time_minutes[tmp_minute][i], RgbColor(red, green, blue)); } }
//                   for(int i = 0; i < 6; i++) {            if(leds_time_hours[tmp_hour][i] >= 0) { strip.SetPixelColor(leds_time_hours[tmp_hour][i], RgbColor(red, green, blue)); } }
// //                        for(int i = 0; i < minutessum; i++) {   leds[leds_minutes[i]].setRGB(red, green, blue);}
//                   strip.Show();
//                  ESP_LOGD("loop", "Update Time: %i:%i  Brightness: %i RGB: %i-%i-%i", hour, minute, brightness, red, green, blue);
//                  ESP_LOGD("loop", "Using tmp_hour: %i tmp_minute: %i minutessum: %i", tmp_hour, tmp_minute, minutessum);
              }
          }
       }
     ESP_LOGD("loop", "Loop end");
    } // Loop()
    
  void dump_config(){
    ESP_LOGCONFIG(TAG, "WordClockLight component");
  }

 protected:
  esphome::light::AddressableLight* internal_light_;
  time::RealTimeClock* timesource_;

  float fred = 0.5, fgreen = 0.5, fblue = 0.5;
};// Class WordClock_Light
} // namespace wordclock_light
} // namespace esphome



