# Questions

## What's `stdint.h`?

allows us to define integer types with specific widths and store them in typedef

## What's the point of using `uint8_t`, `uint32_t`, `int32_t`, and `uint16_t` in a program?

these are the defined integer widths (8 bit unsigned, 32bit unsigned, etc..). Unsigned means there is no designation of positive or negative, so the bits can all go to the int, allowing for larger possible ints (0 to 65536). Signed int designates a positive or negative, which requires use of bit, so it goes from -32767 - 32767

## How many bytes is a `BYTE`, a `DWORD`, a `LONG`, and a `WORD`, respectively?

1 byte, 4 bytes, 4 bytes, 2 bytes

## What (in ASCII, decimal, or hexadecimal) must the first two bytes of any BMP file be? Leading bytes used to identify file formats (with high probability) are generally called "magic numbers."

BM

## What's the difference between `bfSize` and `biSize`?

bfsize = size of the bitmap file
bisize = required size for the struct file 

## What does it mean if `biHeight` is negative?

the bitmap is a top-down DIB and its origin is the upper-left corner.

## What field in `BITMAPINFOHEADER` specifies the BMP's color depth (i.e., bits per pixel)?

biBitCount

## Why might `fopen` return `NULL` in `copy.c`?

it couldn't be found

## Why is the third argument to `fread` always `1` in our code?

We only want to read the first element 

## What value does `copy.c` assign to `padding` if `bi.biWidth` is `3`?

3

## What does `fseek` do?

TODO

## What is `SEEK_CUR`?

TODO
