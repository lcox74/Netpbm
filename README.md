# Netpbm Study

I found a simple image format that I wanted to try and implement.

The examples and format is described on its 
[wiki page](https://en.wikipedia.org/wiki/Netpbm). There are a couple
of different modes refered to as `Magic Number` these being:
- `P1` Portable BitMap (Ascii) -> Black or White
- `P2` Portable GrayMap (Ascii) -> Greyscale
- `P3` Portable PixMap (Ascii) -> RGB
- `P4` Portable BitMap (Binary) -> Black or White
- `P5` Portable GrayMap (Binary) -> Greyscale
- `P6` Portable PixMap (Binary) -> RGB

Each of these modes have their own file format:
- Portable BitMap `.pbm`
- Portable GrayMap `.pgm`
- Portable PixMap `.ppm`

There is also an extension mode called `Portalbe Arbitrary Map (.pam)` 
which uses `P7` as the magic number. This mode allows a more dynamic
image types and can support all three previous modes as well as the 
addition of transparency by setting the `TUPLTYPE` value in the file.

More information on PAM [here](http://netpbm.sourceforge.net/doc/pam.html).

