#include <stdio.h>
#include "netpbm.h"

int main() {
    size_t width = 800, height = 600;
    float hue, saturation, value;
    
    /* Initialise a image buffer called `pixels` with width and height */
    INIT_PPM_BUF(pixels, width, height);

    /* Loop through every pixel in image */
    for (u32 y = 0; y < height; ++y) {
        for (u32 x = 0; x < width; ++x) {

            /* Precalc HSV values */
            hue = (x / (float)width) * 360.0f;
            saturation = 1.0f;
            value = 1.0f - (y / (float)height);

            /* Set pixel colour */
            pixels[y * width + x] = HSV888(hue, saturation, value);
        
        }
    }

    /* Write pixel buffer to image file */
    return construct_ppm_image("out.ppm", pixels, width, height);
}

