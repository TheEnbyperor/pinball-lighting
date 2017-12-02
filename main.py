import serial
import time
import random
import numpy
import threading


class Control:
    def __init__(self, port1):
        self._serial1 = serial.Serial(port1, 115200)

    def bottom_light_off(self):
        try:
            self._serial1.write('9,1,0,0,0#\r\n'.encode())
        except serial.SerialTimeoutException:
            pass

    def bottom_light_on(self, red, green, blue):
        try:
            self._serial1.write(('9,1,%s,%s,%s#\r\n' % (str(int(red)), str(int(green)), str(int(blue)))).encode())
        except serial.SerialTimeoutException:
            pass

    def pulse_feedback(self, port):
        try:
            self._serial1.write(('%s,3,15,200,0,0#\r\n' % (str(port),)).encode())
        except serial.SerialTimeoutException:
            pass


class Animations:
    _bottom_animation = 0
    _bottom_col = [0, 0, 0]
    _bottom_n_col = None
    _bottom_step = 0

    def __init__(self, control):
        self._control = control
        self._bottom_animations = {
            'pulse': self.bottom_do_pulse,
            'strobe': self.bottom_do_strobe,
            'flash': self.bottom_do_flash,
            'shuffle': self.bottom_do_shuffle,
            'random': self.bottom_do_random,
        }

    def process_bottom(self):
        self._bottom_animations[self._bottom_animation]()
        self._control.bottom_light_on(self._bottom_col[0], self._bottom_col[1], self._bottom_col[2])


    def set_bottom_anim(self, animation):
        self._bottom_animation = animation
        self._bottom_step = 0
        self._bottom_col = [0, 0, 0]

    def bottom_do_pulse(self):
        if self._bottom_col[0] == 255 and self._bottom_step == 0:
            self._bottom_step = 1
        elif self._bottom_col[0] == 0 and self._bottom_step == 1:
            self._bottom_step = 2
        elif self._bottom_col[1] == 255 and self._bottom_step == 2:
            self._bottom_step = 3
        elif self._bottom_col[1] == 0 and self._bottom_step == 3:
            self._bottom_step = 4
        elif self._bottom_col[2] == 255 and self._bottom_step == 4:
            self._bottom_step = 5
        elif self._bottom_col[2] == 0 and self._bottom_step == 5:
            self._bottom_step = 0

        if self._bottom_step == 0:
            self._bottom_col[0] += 1
        elif self._bottom_step == 1:
            self._bottom_col[0] -= 1
        elif self._bottom_step == 2:
            self._bottom_col[1] += 1
        elif self._bottom_step == 3:
            self._bottom_col[1] -= 1
        elif self._bottom_step == 4:
            self._bottom_col[2] += 1
        elif self._bottom_step == 5:
            self._bottom_col[2] -= 1

    def bottom_do_strobe(self):
        self._bottom_step += 1
        if self._bottom_step > 150:
            self._bottom_step = 0
        if 0 <= self._bottom_step <= 50:
            self._bottom_col = [255, 0, 0]
        elif 50 < self._bottom_step <= 100:
            self._bottom_col = [0, 255, 0]
        elif 100 < self._bottom_step <= 150:
            self._bottom_col = [0, 0, 255]

    def bottom_do_flash(self):
        self._bottom_step += 1
        if self._bottom_step > 120:
            self._bottom_step = 0
        if 0 <= self._bottom_step <= 10:
            self._bottom_col = [255, 0, 0]
        elif 10 < self._bottom_step <= 20:
            self._bottom_col = [0, 0, 0]
        elif 20 < self._bottom_step <= 30:
            self._bottom_col = [255, 0, 0]
        elif 30 < self._bottom_step <= 40:
            self._bottom_col = [0, 0, 0]
        elif 40 < self._bottom_step <= 50:
            self._bottom_col = [0, 255, 0]
        elif 50 < self._bottom_step <= 60:
            self._bottom_col = [0, 0, 0]
        elif 60 < self._bottom_step <= 70:
            self._bottom_col = [0, 255, 0]
        elif 70 < self._bottom_step <= 80:
            self._bottom_col = [0, 0, 0]
        elif 80 < self._bottom_step <= 90:
            self._bottom_col = [0, 0, 255]
        elif 90 < self._bottom_step <= 100:
            self._bottom_col = [0, 0, 0]
        elif 100 < self._bottom_step <= 110:
            self._bottom_col = [0, 0, 255]
        elif 110 < self._bottom_step <= 120:
            self._bottom_col = [0, 0, 0]

    def bottom_do_shuffle(self):
        self._bottom_step += 1
        if self._bottom_step > 160:
            self._bottom_step = 0
        if 0 <= self._bottom_step <= 40:
            self._bottom_col = [255, 0, 0]
        elif 40 < self._bottom_step <= 80:
            self._bottom_col = [0, 255, 0]
        elif 80 < self._bottom_step <= 120:
            self._bottom_col = [0, 0, 255]
        elif 120 < self._bottom_step <= 160:
            self._bottom_col = [0, 255, 0]

    def bottom_do_random(self):
        colour = numpy.array(self._bottom_col)
        n_colour = numpy.array(self._bottom_n_col)
        black = numpy.array([0, 0, 0])

        if self._bottom_step == 1:
            vector = n_colour - colour
            old = self._bottom_col
            self._bottom_col = list(colour + vector * 0.1)
            self._bottom_col[0] = int(round(self._bottom_col[0]))
            self._bottom_col[1] = int(round(self._bottom_col[1]))
            self._bottom_col[2] = int(round(self._bottom_col[2]))
            if old == self._bottom_col:
                self._bottom_step = 0
            time.sleep(0.02)
        elif self._bottom_step == 0:
            vector = black - colour
            old = self._bottom_col
            self._bottom_col = list(colour + vector * 0.1)
            self._bottom_col[0] = int(round(self._bottom_col[0]))
            self._bottom_col[1] = int(round(self._bottom_col[1]))
            self._bottom_col[2] = int(round(self._bottom_col[2]))
            if old == self._bottom_col:
                self._bottom_step = 1
                self._bottom_n_col = [random.randint(0, 256), random.randint(0, 256), random.randint(0, 256)]
            time.sleep(0.02)

control = Control("COM12")


def do_animations():
    modes = ['random', 'flash', 'strobe', 'shuffle', 'pulse']
    animations = Animations(control)
    change = time.clock()
    last_bottom = time.clock()
    last_top = time.clock()
    animations.set_bottom_anim(random.choice(modes))
    try:
        while True:
            if time.clock() - last_bottom > 0.02:
                last_bottom = time.clock()
                animations.process_bottom()
            if time.clock() - change > 7:
                change = time.clock()
                animations.set_bottom_anim(random.choice(modes))
    except (KeyboardInterrupt, SystemExit):
        control.bottom_light_off()

anim_t = threading.Thread(target=do_animations)
anim_t.setDaemon(False)
anim_t.start()


