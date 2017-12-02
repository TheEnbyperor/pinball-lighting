#include <Arduino.h>

#define RED 3
#define GRN 5
#define BLU 6

uint8_t currentAnim;
uint8_t colour[3] = {0, 0, 0};
unsigned int step = 0;
unsigned long change, last;

void set_colour(uint8_t r, uint8_t g, uint8_t b);
void process_anim(uint8_t anim);

void setup() {
    Serial.begin(115200);
    pinMode(RED, OUTPUT);
    pinMode(GRN, OUTPUT);
    pinMode(BLU, OUTPUT);
    change = millis();
    last = millis();
    currentAnim = (uint8_t)random(0, 3);
    randomSeed((unsigned long)analogRead(0));
}

void loop() {
    if (millis() - last > 10) {
        last = millis();
        process_anim(currentAnim);
        set_colour(colour[0], colour[1], colour[2]);
    }
    if (millis() - change > 14000) {
        change = millis();
        step = 0;
        colour[0] = 0;
        colour[1] = 0;
        colour[2] = 0;
        currentAnim = (uint8_t)random(0, 3);
    }
}

void do_pulse();
void do_strobe();
void do_flash();

void process_anim(uint8_t anim) {
    if (anim == 0) {
        do_pulse();
    } else if (anim == 1) {
        do_strobe();
    } else if (anim == 2) {
        do_flash();
    }

}

void set_colour(uint8_t r, uint8_t g, uint8_t b) {
    digitalWrite(RED, r);
    digitalWrite(GRN, g);
    digitalWrite(BLU, b);
}