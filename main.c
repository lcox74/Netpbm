#include <stdio.h>
#include <math.h>

#define MIN(a,b) (((a)>(b))?(b):(a))
#define MAX(a,b) (((a)>(b))?(a):(b))

typedef unsigned char 	u8;
typedef unsigned short 	u16;
typedef unsigned int 	u32; 

static inline u32 convert_rgb(u8, u8, u8);
static inline u32 convert_hsv(float, float, float);
//int construct_pbm_image(FILE*, u8*, u32, u32);
int construct_pgm_image(FILE*, u16*, u32, u32);
int construct_ppm_image(FILE*, u32*, u32, u32);

#define RGB888(r,g,b) ((u32)convert_rgb((u8)(r),(u8)(g),(u8)(b)))
#define HSV888(h,s,v) ((u32)convert_hsl((h),(s),(v)))

int main() {
	u32 width = 800, height = 600;
	
	u32 pixels1[width * height];
	u16 pixels2[width * height];
	for (u32 y = 0; y < height; ++y) {
		for (u32 x = 0; x < width; ++x) {
			pixels1[y * width + x] = HSL888(((float)x / (float)width) * 360.0f, 
											1.0f, 1.0f - ((float)y / (float)height)); 
			pixels2[y * width + x] = (float)x / (float)width * 255.0f;
		}
	}
	
	FILE *fp1 = fopen("out.ppm", "wb");
	construct_ppm_image(fp1, pixels1, width, height);
	fclose(fp1);
	
	FILE *fp2 = fopen("out.pgm", "wb");
	construct_pgm_image(fp2, pixels2, width, height);
	fclose(fp2);
	
	return 0;
}

int construct_ppm_image(FILE *fp, u32 *pixels, u32 width, u32 height) {
	fprintf(fp, "P6\n%d %d 255\n", width, height);
	for (u32 i = 0; i < width * height; ++i) {
		fwrite((const void *) (u8*)(&pixels[i]) + 1, sizeof(u8), 3, fp);
	}
}

int construct_pgm_image(FILE *fp, u16 *pixels, u32 width, u32 height) {
	fprintf(fp, "P5\n%d %d 65535\n", width, height);
	for (u32 i = 0; i < width * height; ++i) {
		fwrite((const void *) (&pixels[i]), sizeof(u16), 1, fp);
	}
}

static inline u32 convert_rgb(u8 r, u8 g, u8 b) {
	return (b << 8*3) | (g << 8*2) | (r << 8*1) | (0xFF << 8*0);
}

static inline u32 convert_hsv(float hue, float saturation, float value) {
	float c, x, m, r, g, b;

	/* Bind Ranges */
	hue = MIN(360, MAX(0, hue));
	saturation = MIN(1, MAX(0, saturation));
	value = MIN(1, MAX(0, value));

	/* Calculate Offsets */
	c = value * saturation;
	x = c * (1.0f - fabs(fmod(hue / 60.0f, 2.0f) - 1.0f));
	m = value - c;

	/* Calculate Colours */
	if (hue < 60)						{ r = c; g = x; b = 0; }
	else if (hue >= 60 && hue < 120) 	{ r = x; g = c; b = 0; }
	else if (hue >= 120 && hue < 180) 	{ r = 0; g = c; b = x; }
	else if (hue >= 180 && hue < 240) 	{ r = 0; g = x; b = c; }
	else if (hue >= 240 && hue < 300) 	{ r = x; g = 0; b = c; }
	else if (hue >= 300 && hue < 360) 	{ r = c; g = 0; b = x; }

	return RGB888((r+m)*255.0f,(g+m)*255.0f,(b+m)*255.0f);
}
