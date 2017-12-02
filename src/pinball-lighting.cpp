#include <Arduino.h>

#define RED 3
#define GRN 5
#define BLU 6

#define NUM_ANIMS 4

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
    currentAnim = (uint8_t)random(0, NUM_ANIMS);
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
        currentAnim = (uint8_t)random(0, NUM_ANIMS);
    }
}

void do_pulse();
void do_strobe();
void do_flash();
void do_shuffle();

void process_anim(uint8_t anim) {
    if (anim == 0) {
        do_pulse();
    } else if (anim == 1) {
        do_strobe();
    } else if (anim == 2) {
        do_flash();
    } else if (anim == 3) {
        do_shuffle();
    }
}

void do_pulse() {
    if (colour[0] == 255 && step == 0) {
        step = 1;
    } else if (colour[0] == 0 && step == 1) {
        step = 2;
    } else if (colour[1] == 255 && step == 2) {
        step = 3;
    } else if (colour[1] == 0 && step == 3) {
        step = 4;
    } else if (colour[2] == 255 && step == 4) {
        step = 5;
    } else if (colour[2] == 0 && step == 5) {
        step = 0;
    }

    if (step == 0) {
        colour[0] += 1;
    } else if (step == 1) {
        colour[0] -= 1;
    } else if (step == 2) {
        colour[1] += 1;
    } else if (step == 3) {
        colour[1] -= 1;
    } else if (step == 4) {
        colour[2] += 1;
    } else if (step == 5) {
        colour[2] -= 1;
    }
}

void do_strobe() {
    step += 1;
    if (step > 150) {
        step = 0;
    }
    if (0 <= step && step <= 50) {
        colour[0] = 255;
        colour[1] = 0;
        colour[2] = 0;
    } else if (50 < step && step <= 100) {
        colour[0] = 0;
        colour[1] = 255;
        colour[2] = 0;
    } else if (100 < step && step <= 150) {
        colour[0] = 0;
        colour[1] = 0;
        colour[2] = 255;
    }
}

void do_flash() {
    step += 1;
    if (step > 120) {
        step = 0;
    }
    if (0 <= step && step <= 10) {
        colour[0] = 255;
        colour[1] = 0;
        colour[2] = 0;
    } else if (10 < step && step <= 20) {
        colour[0] = 0;
        colour[1] = 0;
        colour[2] = 0;
    } else if (20 < step && step <= 30) {
        colour[0] = 255;
        colour[1] = 0;
        colour[2] = 0;
    } else if (30 < step && step <= 40) {
        colour[0] = 0;
        colour[1] = 0;
        colour[2] = 0;
    } else if (40 < step && step <= 50) {
        colour[0] = 0;
        colour[1] = 255;
        colour[2] = 0;
    } else if (50 < step && step <= 60) {
        colour[0] = 0;
        colour[1] = 0;
        colour[2] = 0;
    } else if (60 < step && step <= 70) {
        colour[0] = 0;
        colour[1] = 255;
        colour[2] = 0;
    } else if (70 < step && step <= 80) {
        colour[0] = 0;
        colour[1] = 0;
        colour[2] = 0;
    } else if (80 < step && step <= 90) {
        colour[0] = 0;
        colour[1] = 0;
        colour[2] = 255;
    } else if (90 < step && step <= 100) {
        colour[0] = 0;
        colour[1] = 0;
        colour[2] = 0;
    } else if (100 < step && step <= 110) {
        colour[0] = 0;
        colour[1] = 0;
        colour[2] = 255;
    } else if (110 < step && step <= 120) {
        colour[0] = 0;
        colour[1] = 0;
        colour[2] = 0;
    }
}

void do_shuffle() {
    step += 1;
    if (step > 160) {
        step = 0;
    }
    if (0 <= step && step <= 40) {
        colour[0] = 255;
        colour[1] = 0;
        colour[2] = 0;
    } else if (40 < step && step <= 80) {
        colour[0] = 0;
        colour[1] = 255;
        colour[2] = 0;
    } else if (80 < step && step <= 120) {
        colour[0] = 0;
        colour[1] = 0;
        colour[2] = 255;
    } else if (120 < step && step <= 160) {
        colour[0] = 0;
        colour[1] = 255;
        colour[2] = 0;
    }
}

void set_colour(uint8_t r, uint8_t g, uint8_t b) {
    analogWrite(RED, r);
    analogWrite(GRN, g);
    analogWrite(BLU, b);
}