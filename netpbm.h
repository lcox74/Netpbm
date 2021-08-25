#include <stdio.h>
#include <math.h>

#define MIN(a,b) (((a)>(b))?(b):(a))
#define MAX(a,b) (((a)>(b))?(a):(b))

/* Type abstractions */
typedef unsigned char   u8;
typedef unsigned short  u16;
typedef unsigned int    u32;
typedef unsigned long   u64;

/* Helper functions */
static inline u32 convert_rgb(u8 red, u8 green, u8 blue);
static inline u32 convert_hsv(float hue, float sat, float val);

/* Colour Abstractions */
#define RGB888(r,g,b) ((u32)convert_rgb((u8)(r),(u8)(g),(u8)(b)))
#define HSV888(h,s,v) ((u32)convert_hsv((h),(s),(v)))

/* Main function declarations */
int construct_pbm_image(const char* file, u8  *buffer, size_t w, size_t h);
int construct_pgm_image(const char* file, u16 *buffer, size_t w, size_t h);
int construct_ppm_image(const char* file, u32 *buffer, size_t w, size_t h);
int construct_pam_image(const char* file, u32 *buffer, size_t w, size_t h);

/* Buffer Abstractions */
#define INIT_PBM_BUF(name,w,h) u8  name[(w)*(h)] 
#define INIT_PGM_BUF(name,w,h) u16 name[(w)*(h)] 
#define INIT_PPM_BUF(name,w,h) u32 name[(w)*(h)] 
#define INIT_PAM_BUF(name,w,h) u32 name[(w)*(h)] 


/** Implementations **/

/*
 * Constructs a pbm image using a buffer, if the value in the buffer is not 0
 * then it is white, else if it is 0 then the pixel is black
 */
int construct_pbm_image(const char* file, u8 *buffer, size_t w, size_t h) {
    FILE *fp;
    u32 i;
    u8 mini_buffer, buf_count;

    // Create File
    if (!(fp = fopen(file, "wb"))) return 1;
    
    // Write header to file
    fprintf(fp, "P4\n%ld %ld\n", w, h);

    // Write Colour bytes to file
    buf_count = mini_buffer = 0x00;
    for (i = 0; i < w * h; ++i) {
        if (buf_count >= 8) {
            fwrite((const void *) (&mini_buffer), sizeof(u8), 1, fp);
            mini_buffer = buf_count = 0x00;
        }
        mini_buffer |= ((buffer[i] > 0) & 0x1) << buf_count++; 
    }

    // Writing any data that is left
    if (buf_count > 0x00) 
        fwrite((const void *) (&mini_buffer), sizeof(u8), 1, fp);

    // Close file
    fclose(fp); 
    return 0;
}

/* Constructs a pgm image using a buffer */
int construct_pgm_image(const char* file, u16 *buffer, size_t w, size_t h) {
    FILE *fp;
    u32 i;

    // Create File
    if (!(fp = fopen(file, "wb"))) return 1;
    
    // Write header to file
    fprintf(fp, "P5\n%ld %ld 65535\n", w, h);

    // Write Colour bytes to file
    for (i = 0; i < w * h; ++i)
        fwrite((const void *) (&buffer[i]), sizeof(u16), 1, fp);
    
    // Close file
    fclose(fp); 
    return 0;
}

/* Constructs a ppm image using a buffer */
int construct_ppm_image(const char* file, u32 *buffer, size_t w, size_t h) {
    FILE *fp;
    u32 i;

    // Create File
    if (!(fp = fopen(file, "wb"))) return 1;
    
    // Write header to file
    fprintf(fp, "P6\n%ld %ld 255\n", w, h);

    // Write Colour bytes to file
    for (i = 0; i < w * h; ++i)
        fwrite((const void *) (u8*)(&buffer[i]) + 1, sizeof(u8), 3, fp);
    
    // Close file
    fclose(fp); 
    return 0;
}

/* Constructs a pam image using a buffer [Currently Unsupported] */
int construct_pam_image(const char* file, u32 *buffer, size_t w, size_t h) {
    (void)file;
    (void)buffer;
    (void)w;
    (void)h;
    return 1;
}

/*
 * The buffer requires to have colour in the order of BGRA so this
 * converts the conventional RGB to that format.
 */
static inline u32 convert_rgb(u8 r, u8 g, u8 b) {
    return (b << 8*3) | (g << 8*2) | (r << 8*1) | (0xFF << 8*0);
}

/*
 * Support for HSV colour allowing more variety of colours and 
 * is generally the easier format to pick specific colours.
 */
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
    if (hue < 60)                       { r = c; g = x; b = 0; }
    else if (hue >= 60 && hue < 120)    { r = x; g = c; b = 0; }
    else if (hue >= 120 && hue < 180)   { r = 0; g = c; b = x; }
    else if (hue >= 180 && hue < 240)   { r = 0; g = x; b = c; }
    else if (hue >= 240 && hue < 300)   { r = x; g = 0; b = c; }
    else                                { r = c; g = 0; b = x; }

    return RGB888((r+m)*255.0f,(g+m)*255.0f,(b+m)*255.0f);
}
