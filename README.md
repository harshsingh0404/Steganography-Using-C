# Steganography Using C

## Description
A C-based image steganography application that hides secret text inside an image
using bit-level manipulation. The program encodes and decodes data while
preserving the visual quality of the image.

## Features
- Hide secret text inside image files
- Extract hidden text from stego-images
- Uses Least Significant Bit (LSB) technique
- Secure data hiding using bitwise operations
- File handling for binary image processing

## Technologies Used
- C Programming
- Bit Manipulation
- File Handling
- Structures

## How It Works
- The encoder embeds secret data into the least significant bits of image bytes.
- The decoder reads the modified bits to reconstruct the original hidden message.
- Since only LSBs are altered, image quality remains nearly unchanged.

## How to Compile and Run
```bash
gcc *.c
./a.out
