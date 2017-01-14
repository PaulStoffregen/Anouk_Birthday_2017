#include <Adafruit_NeoPixel.h>

#define LEDS 12
#define PIN  17

Adafruit_NeoPixel strip = Adafruit_NeoPixel(LEDS, PIN, NEO_GRB + NEO_KHZ800);

void setup()
{
	pinMode(11, OUTPUT);
	pinMode(12, OUTPUT);
	digitalWrite(11, HIGH);
	digitalWrite(12, LOW);
	strip.begin();
	strip.show();
	strip.show();
}

#define RGB(r, g, b) (((r) << 16) | ((g) << 8) | (b))
#define RGBA(r, g, b, a) (((a) << 24) | ((r) << 16) | ((g) << 8) | (b))


#define NUM 6

float speed[NUM] = {
	-0.1,
	 0.07,
	 0.023,
	 0.3,
	 -0.0673};
float bright[NUM] = {
	0.5,
	0.7,
	0.6,
	0.9,
	1.0};
float size[NUM] = {
	0.361,
	0.5,
	0.4,
	3.1,
	0.7};
uint32_t color[NUM] = {
	RGB(255, 0, 110),
	RGB(205, 0, 140),
	RGB(255, 0, 90),
	RGB(255, 255, 0),
	RGB(250, 0, 105)};
float phase[NUM];

void loop() {
	int i, n;
	int knob1 = analogRead(A9);
	int knob2 = analogRead(A8);
	uint32_t pixel[LEDS];
	uint32_t framebuf[LEDS];

	//Serial.print(knob1);
	//Serial.print(", ");
	//Serial.print(knob2);
	//Serial.println();

	speed[3] = (float)(knob1 - 512) * .00032;
	if (knob1 < 512) {
		color[3] = RGB(255, 200, 0); // yellow
	} else {
		color[3] = RGB(0, 0, 255); // blue
	}
	bright[3] = fabsf(speed[3]) * 7.2;

	bright[0] = (float)(knob2 + 700) * .001;
	bright[1] = (float)(knob2 + 900) * .0013;
	bright[4] = (float)(knob2) * .003;
	speed[4] = (float)(knob2) * .0003;

	memset(framebuf, 0, sizeof(framebuf));

	for (n=0; n < NUM; n++) {
		for (i=0; i < LEDS; i++) {
			int a = sinf(i * size[n] + phase[n]) * (bright[n] * 255.0);
			if (a < 0) a = 0;
			if (a > 255) a = 255;
			pixel[i] = color[n] | ((a & 255) << 24);
		}
		phase[n] = phase[n] + speed[n];
		if (phase[n] > 6.28318) phase[n] -= 6.28318;
		else if (phase[n] < 0) phase[n] += 6.28318;
		blend(framebuf, pixel);
	}

	for (i=0; i < LEDS; i++) {
		strip.setPixelColor(i, framebuf[i]);
	}
	strip.show();

	delay(20);
}


void blend(uint32_t *framebuf, const uint32_t *data)
{
	unsigned int i, r, g, b, a, r0, g0, b0;

	for (i=0; i < LEDS; i++) {
		r0 = (framebuf[i] >> 16) & 255;
		g0 = (framebuf[i] >> 8) & 255;
		b0 = (framebuf[i] >> 0) & 255;
		a = (data[i] >> 24) & 255;
		r = (data[i] >> 16) & 255;
		g = (data[i] >> 8) & 255;
		b = (data[i] >> 0) & 255;
		r = (r * a + r0 * (255 - a)) / 255;
		g = (g * a + g0 * (255 - a)) / 255;
		b = (b * a + b0 * (255 - a)) / 255;
		if (r > 255) r = 255;
		if (g > 255) g = 255;
		if (b > 255) b = 255;
		framebuf[i] = RGBA(r, g, b, 0);
	}
}
