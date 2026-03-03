/*-------------------------------------------------------------------------------------
  File Name     : decode.c
  Description   : Source file for the decoding module of the LSB Image Steganography
                  project. This file extracts hidden secret data from a stego BMP
                  image using the Least Significant Bit (LSB) technique.
                  
  Project Title : LSB Image Steganography (Decoding Part)
  Name          : Harsh Singh
  Date          :
-------------------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "typesd.h"
#include "common.h"
#include "decode.h"

/* Read and validate decode arguments */
d_Status read_and_validate_decode_args(int argc, char *argv[], DecodeInfod *decInfo)
{
    if (argc < 3)
        return d_failure;

    if (strstr(argv[2], ".bmp") == NULL)
        return d_failure;

    decInfo->stego_image_fname = argv[2];

    if (argc >= 4)
        strncpy(decInfo->buffer, argv[3], sizeof(decInfo->buffer) - 1);
    else
    {
        strcpy(decInfo->buffer, "output");
    }
    return d_success;
}

/* Decode magic string */
d_Status decode_magic_string(char *magic_string, DecodeInfod *decInfo)
{
    char image[8];

    for (int i = 0; i < strlen(MAGIC_STRING); i++)
    {
        fread(image, 1, 8, decInfo->fptr_stego_image);
        decode_byte_from_lsb(&magic_string[i], image);
    }

    magic_string[strlen(MAGIC_STRING)] = '\0';

    if (strcmp(magic_string, MAGIC_STRING) == 0)
    {
        printf("1. Magic string verified: %s\n", magic_string);
        return d_success;
    }

    return d_failure;
}

/* Decode extension size */
d_Status decode_secret_file_extn_size(long int *size, DecodeInfod *decInfo)
{
    char image[32];
    fread(image, 1, 32, decInfo->fptr_stego_image);
    decode_size_from_lsb(size, image);

    printf("2. Secret file extension size decoded...\n");
    return d_success;
}

/* Decode extension and open output file */
d_Status decode_extn(int *size, DecodeInfod *decInfo)
{
    char image[8];

    for (int i = 0; i < *size; i++)
    {
        fread(image, 1, 8, decInfo->fptr_stego_image);
        decode_byte_from_lsb(&decInfo->extn_secret_file[i], image);
    }

    decInfo->extn_secret_file[*size] = '\0';
    strcat(decInfo->buffer, decInfo->extn_secret_file);

    decInfo->output_fptr = fopen(decInfo->buffer, "wb");
    if (decInfo->output_fptr == NULL)
    {
        perror("fopen");
        return d_failure;
    }

    printf("3. Secret file extension decoded...\n");
    return d_success;
}

/* Decode secret file size */
d_Status decode_secret_file_size(long *file_size, DecodeInfod *decInfo)
{
    char image[32];
    fread(image, 1, 32, decInfo->fptr_stego_image);
    decode_size_from_lsb(file_size, image);

    printf("4. Secret file size decoded bytes...");
    return d_success;
}

/* Decode secret file data */
d_Status decode_secret_file_data(DecodeInfod *decInfo)
{
    char image[8];
    char ch;

    for (long i = 0; i < decInfo->size_secret_file; i++)
    {
        fread(image, 1, 8, decInfo->fptr_stego_image);
        decode_byte_from_lsb(&ch, image);
        fwrite(&ch, 1, 1, decInfo->output_fptr);
    }

    fclose(decInfo->output_fptr);
    printf("5. Secret file data decoded successfully...\n");
    return d_success;
}

/* Decode byte from LSB */
d_Status decode_byte_from_lsb(char *data, char *image_buffer)
{
    *data = 0;
    for (int i = 0; i < 8; i++)
        *data = (*data << 1) | (image_buffer[i] & 1);

    return d_success;
}

/* Decode size from LSB */
d_Status decode_size_from_lsb(long int *size, char *imageBuffer)
{
    *size = 0;
    for (int i = 0; i < 32; i++)
        *size = (*size << 1) | (imageBuffer[i] & 1);

    return d_success;
}

/* Main decoding function */
d_Status do_decoding(DecodeInfod *decInfo)
{
    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "rb");
    if (decInfo->fptr_stego_image == NULL)
    {
        perror("fopen");
        return d_failure;
    }

    fseek(decInfo->fptr_stego_image, 54, SEEK_SET);

    char magic_string[strlen(MAGIC_STRING) + 1];

    if (decode_magic_string(magic_string, decInfo) == d_failure)
        return d_failure;

    long int extn_size;
    decode_secret_file_extn_size(&extn_size, decInfo);
    decode_extn((int *)&extn_size, decInfo);

    decode_secret_file_size(&decInfo->size_secret_file, decInfo);
    decode_secret_file_data(decInfo);

    fclose(decInfo->fptr_stego_image);

    printf("\nDecoding completed successfully...\n");
    return d_success;
}
