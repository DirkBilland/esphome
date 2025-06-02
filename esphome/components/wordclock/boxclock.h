#include "esphome.h"
#include <NeoPixelBus.h>

const uint16_t PixelCount = 25; // this example assumes 4 pixels, making it smaller will cause a failure
const uint8_t PixelPin = D4;  // make sure to set this to the correct pin, ignored for Esp8266

//Define your NeoPixel setup. Please read https://github.com/Makuna/NeoPixelBus/wiki/NeoPixelBus-object#neo-methods
// NeoPixelBrightnessBus<NeoGrbwFeature, NeoEsp8266Uart1800KbpsMethod> strip(PixelCount, PixelPin);
//NeoPixelBus<NeoBrgFeature, NeoEsp8266DmaWs2812xMethod> strip(PixelCount, PixelPin);
//NeoPixelBrightnessBus<NeoGrbFeature, NeoEsp8266Uart1800KbpsMethod> strip(PixelCount, PixelPin);
//NeoPixelBus<NeoGrbFeature, NeoEsp8266Uart1Ws2812Method> strip(PixelCount, PixelPin);
//NeoPixelBus<NeoGrbFeature, NeoEsp8266AsyncUart1800KbpsMethod> strip(PixelCount, PixelPin);
NeoPixelBus<NeoGrbFeature, NeoEsp8266AsyncUart1800KbpsMethod> strip(PixelCount, PixelPin);

// esphome dependencies:
// needs: esphome time --> id: current_time
// needs: esphome fastled --> id: fastledlight

// Layout of Clock (X not used chars) - PIN Layout
// ESXISTXFÜNF    1.......11
// ZEHNZWANZIG    22......12
// DREIVIERTEL    23......33
// XXNACHVORXX    44......34
// HALBXZWÖLFX    45......55
// ZWEIXSIEBEN    ....
// XDREIXXFÜNF    ....
// ELFNEUNVIER    ....
// XACHTZEHNRS    ....
// XSECHSXXUHR    109.......101
// ICONS          110.......120
// MINUTES        124.......121

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

int hour = -1;
int minute = -1;
int second = -1;

int red = 124;
int green = 124;
int blue = 124;
float fred, fgreen, fblue;
int white = 124;
int brightness = 50;
int brightness2;
bool change = false;

//class Wordclock : public Component, public CustomAPIDevice {
class Boxclock : public Component, public LightOutput {
    public:
         void setup() override {
            strip.Begin();
            strip.ClearTo(RgbColor(0, 0, 0));
            strip.Show();

            // // Start all LED with on and default color and brightness to check if everything is working...
            for(int i = 0; i < PixelCount; i++) { strip.SetPixelColor(i, RgbColor(red, 0, 0)); strip.Show(); delay(10); }
            for(int i = 0; i < PixelCount; i++) { strip.SetPixelColor(i, RgbColor(0, green, 0)); strip.Show(); delay(10); }
            for(int i = 0; i < PixelCount; i++) { strip.SetPixelColor(i, RgbColor(0, 0, blue)); strip.Show(); delay(10); }
            strip.ClearTo(RgbColor(0, 0, 0));
            strip.Show();
            ESP_LOGD("setup", "Setup done!");
           //register_service(&Wordclock::on_setled, "setled", {"number","red", "blue", "green"});
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
        
        void on_setled(int number, int red, int blue, int green) {
            if (number < PixelCount || number > 0) {
                ESP_LOGD("setled", "Setting led number %d to color %i %i %i", number, red, green, blue );
                strip.SetPixelColor(number, RgbColor(red, green, blue));
                strip.Show();
                }
            else { ESP_LOGE("setled", "Not a valid LED Number - out of range"); }
         }

        void loop() override {
            //auto time = id(current_time).now();
            auto time = ESPTime::from_epoch_local( id(current_time).timestamp_now() + 150);
            int h = time.hour;
            int m = time.minute;
            int s = time.second;

            boxclocklight->current_values_as_rgb(&fred, &fgreen, &fblue);
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
                strip.SetPixelColor(0, RgbColor(255, 0, 0)); strip.Show(); delay(250);
                strip.SetPixelColor(0, RgbColor(0, 255, 0)); strip.Show(); delay(250);
                strip.SetPixelColor(0, RgbColor(0, 0, 255)); strip.Show(); delay(250);
                strip.SetPixelColor(0, RgbColor(0, 0, 0));   strip.Show();
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
                        for(int i = 0; i < PixelCount; i++) {     if(i < 110 || i > 120) strip.SetPixelColor(i, RgbColor(0, 0, 0)); }
                        for(int i = 0; i < 5; i++) {            strip.SetPixelColor(leds_time_it_is[i], RgbColor(red, green, blue)); }
                        for(int i = 0; i < 3; i++) {           if(leds_time_minutes[tmp_minute][i] >= 0) { strip.SetPixelColor(leds_time_minutes[tmp_minute][i], RgbColor(red, green, blue)); } }
                        for(int i = 0; i < 6; i++) {            if(leds_time_hours[tmp_hour][i] >= 0) { strip.SetPixelColor(leds_time_hours[tmp_hour][i], RgbColor(red, green, blue)); } }
//                        for(int i = 0; i < minutessum; i++) {   leds[leds_minutes[i]].setRGB(red, green, blue);}
                        strip.Show();
                        ESP_LOGD("loop", "Update Time: %i:%i  Brightness: %i RGB: %i-%i-%i", hour, minute, brightness, red, green, blue);
                        ESP_LOGD("loop", "Using tmp_hour: %i tmp_minute: %i minutessum: %i", tmp_hour, tmp_minute, minutessum);
                    }
                }
            }
        }
};
