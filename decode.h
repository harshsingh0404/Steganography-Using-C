/*-------------------------------------------------------------------------------------
  File Name     : decode.h
  Description   : Header file for the decoding module of the LSB Image Steganography
                  project. This file contains function declarations, macros, and
                  structure definitions required for extracting hidden secret data
                  from a stego BMP image using the Least Significant Bit (LSB)
                  technique.
                  
  Project Title : LSB Image Steganography (Decoding Part)
  Name          : Harsh Singh
  Date          :
-------------------------------------------------------------------------------------*/

#ifndef DECODE_H
#define DECODE_H
#include <stdio.h>

#include "typesd.h" // Contains user defined types

/*
 * Structure to store information required for
 * encoding secret file to source Image
 * Info about output and intermediate data is
 * also stored
 */

typedef struct _DecodeInfod
{
    /* Source Image info */
    char *src_image_fname; // To store the src image name
    FILE *output_fptr;  // To store the address of the o/p image
    uint image_capacity;   // To store the size of image

    /* Secret File Info */
    char *secret_fname;       // To store the secret file name
    FILE *fptr_secret;        // To store the secret file address
    char extn_secret_file[5]; // To store the Secret file extension
    char secret_data[100];    // To store the secret data
    long size_secret_file;    // To store the size of the secret data

    /* Stego Image Info */
    char *stego_image_fname; // To store the dest file name
    FILE *fptr_stego_image;  // To store the address of stego image
    char buffer[20];
} DecodeInfod;

/* Encoding function prototype */

/* Read and validate Encode args from argv */
d_Status read_and_validate_decode_args(int argc, char *argv[], DecodeInfod *decInfod);

/* Perform the decoding */
d_Status do_decoding(DecodeInfod *decInfo);

/* Store Magic String */
d_Status decode_magic_string(char *magic_string, DecodeInfod *decInfo);

/*Encode extension size*/
d_Status decode_secret_file_extn_size(long int *size, DecodeInfod *decInfo);

/* Encode secret file extenstion */
d_Status decode_extn(int *size, DecodeInfod *decInfod);

/* Encode secret file size */
d_Status decode_secret_file_size(long *file_size, DecodeInfod *decodeInfo);

/* Encode secret file data*/
d_Status decode_secret_file_data(DecodeInfod *decodInfo);

/* Encode a byte into LSB of image data array */
d_Status decode_byte_from_lsb(char *data, char *image_buffer);

// Encode a size to lsb

d_Status decode_size_from_lsb(long int *size, char *imageBuffer);


#endif