#include <Arduino.h>

#define RED 3
#define GRN 5
#define BLU 6

void setup() {
    pinMode(RED, OUTPUT);
    pinMode(GRN, OUTPUT);
    pinMode(BLU, OUTPUT);
}

void loop() {

}

void set_colour(uint8_t r, uint8_t g, uint8_t b) {
    digitalWrite(RED, r);
    digitalWrite(GRN, g);
    digitalWrite(BLU, b);
}