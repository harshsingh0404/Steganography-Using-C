/*-------------------------------------------------------------------------------------
  File Name     : encode.c
  Description   : Source file for the encoding module of the LSB Image Steganography
                  project. This file contains the implementation of functions used
                  to hide secret data inside a BMP image using the Least Significant
                  Bit (LSB) technique while preserving image quality.
                  
  Project Title : LSB Image Steganography (Encoding Part)
  Name          : Harsh Singh
  Date          :
-------------------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "types.h"
#include "common.h"

/* Function Definitions */
/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)                 
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("   width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("   height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}

uint get_file_size(FILE *fptr)
{
    long size;
    // Find the size of secret file data

    fseek(fptr, 0, SEEK_END);
    size=ftell(fptr);
    fseek(fptr,0,SEEK_SET);
    return (uint)size;
    
}

/*
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */

Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    //Check whether the file name is there or not (before.)
    if(argv[2]!=NULL)
    {
        if(strstr(argv[2],".bmp")!=NULL)
        {
            encInfo->src_image_fname=argv[2];
        }
        else
        {
            return e_failure;
        }
    }
    else
    {
        return e_failure;
    }
    //Check the src file is having (.bmp) or not.
    //encInfo -> src_image_fname argv[2]
    //return failure

    //Check whether the file Loading there or not (before.)
    //Check the src file is having (.txt or .c or.h or .sh) or not.
    if(argv[3]!=NULL)
    {
        if(strstr(argv[3],".txt")!=NULL||strstr(argv[3],".c")!=NULL||strstr(argv[3],".h")!=NULL||strstr(argv[3],".sh")!=NULL)
        {
            encInfo -> secret_fname = argv[3];
        }
        else
        {
            return e_failure;
        }
    }
    else
    {
        return e_failure;
    }
    //Check whether the argv[4] is having NULL or not
   if (argv[4] == NULL)
       {
            printf("ERROR: Please provide output BMP filename\n");
            return e_failure;
        }

    //encInfo -> stego_image_fname "default.bmp";
    //Check whether the file name is there or not (before.)
    //Check the src file is having (.bmp) or not.
    else
    {
        if(strstr(argv[4], ".bmp") != NULL)
        {
            encInfo->stego_image_fname = argv[4];
        }
        else
        {
            return e_failure;
        }
    }

    
      
    char *extn_ptr = strstr(encInfo->secret_fname, ".");
    if(extn_ptr != NULL)
    {
        strncpy(encInfo->extn_secret_file, extn_ptr, sizeof(encInfo->extn_secret_file) - 1);
        encInfo->extn_secret_file[sizeof(encInfo->extn_secret_file) - 1] = '\0';
    }
    else
    {
        return e_failure;
    }

    return e_success;
    //encInfo-> stego_image_fname = argv[4]
    //return failure
    //return e_success
}

Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

        return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

        return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

        return e_failure;
    }

    // No failure return e_success
    printf("1.All Files opened successfully\n");
    return e_success;
}

Status check_capacity(EncodeInfo *encInfo)
{
    encInfo -> image_capacity = get_image_size_for_bmp (encInfo -> fptr_src_image);
    encInfo -> size_secret_file = get_file_size (encInfo -> fptr_secret);
    printf("Secret file size = %ld bytes\n", encInfo->size_secret_file);
    char * extn = strstr(encInfo -> secret_fname, ".txt");
    int extn_size = strlen(extn);
    int total_bytes = 54 + (strlen(MAGIC_STRING) * 8) + 32 + (extn_size*8) + 32 + (encInfo -> size_secret_file * 8);
    if(encInfo -> image_capacity > total_bytes)
    {
        printf("2. Sufficient image capacity\n");
        return e_success;
    }
    else
    {
        printf("Insufficient image capacity\n");
        return e_failure;
    }
}

Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    char image_buffer[54];
    if(fread(image_buffer,1,54,fptr_src_image)!=54)
    {
         return e_failure;
    }
    if(fwrite(image_buffer,1,54,fptr_dest_image)!=54)
    {
        return e_failure;
    }
     printf("3. bmp header copied successfully...\n");
    return e_success;
//      char image_buffer[54];
//     fread(image_buffer,1,54,fptr_src_image);
//     fwrite(image_buffer,1,54,fptr_dest_image);
//     if(ftell(fptr_src_image)==ftell(fptr_dest_image))
//     {
//         printf("bmp header copied successfully...\n");
//         return e_success;
//     }
//     else
//     {
//          printf("bmp header copying failed...\n");
//         return e_failure;
//     }  
 }

Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    //declare char buffer[8];
    //run the loop up to i<strlen(magic_string)
    //in loop {} fread(buffer,8,1,encInfo -> fptr_src_image;
    //call the func encode_byte_to_lsb(magic_string[i],buffer);
    //in loop {} fwrite(buffer,8,1,encInfo -> fptr_stego_image;
    
    char buffer[8];
    for(size_t i=0;i<strlen(magic_string);i++)
    {
        fread(buffer, 8,1,encInfo -> fptr_src_image);           
        encode_byte_to_lsb(magic_string[i],buffer);
        fwrite(buffer, 8, 1, encInfo -> fptr_stego_image);
    }
    printf("4. Magic string encoded successfully...\n");                  //magic string encoded
    return e_success;


}
Status encode_secret_file_extn_size(int size, EncodeInfo *encInfo)
{
    
    char buffer[32];                                                      //declare char buffer[32];
    fread(buffer,32,1,encInfo -> fptr_src_image);                         //fread(buffer,32,1,encInfo->fptr_sec_image);
    // call the function
     encode_size_to_lsb(size,buffer);                                     //encode_size_to_lsb(size,buffer);
     fwrite(buffer,32,1, encInfo -> fptr_stego_image);
     printf("5. secret file extension size encoded successfully...\n");
     return e_success;                                                       //fread(buffer,32,1,encInfo->fptr_stego_image);
} 

Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{    
    char buffer[8];                                                         //declare char buffer[8];
    for(size_t i=0; i<strlen(file_extn);i++)                                 //run the loop up to i<strlen(file_extn)
    {
        fread(buffer, 8, 1, encInfo -> fptr_src_image);                       //in loop {} fread(buffer,8,1,encInfo -> fptr_src_image;
        encode_byte_to_lsb(file_extn[i],buffer);                             //call the func encode_byte_to_lsb(file_extn[i],buffer);
        fwrite(buffer, 8, 1, encInfo -> fptr_stego_image);                  // fwrite(buffer,8,1,encInfo -> fptr_stego_image;

    }
    printf("6. secret file extension encoded successfully...\n");
    return e_success;

}

Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    //declare char buffer[32];
    char buffer[32];
    fread(buffer,32,1,encInfo->fptr_src_image);                                   //read from source file 
    // call the function
    encode_size_to_lsb(file_size, buffer);
    fwrite(buffer,32,1,encInfo->fptr_stego_image);                               //read from destination file
   printf("7. secret file size encoded successfully...\n");
    return e_success;
}

Status encode_secret_file_data(EncodeInfo *encInfo)
{
    rewind(encInfo->fptr_secret); // Go to start of secret file
    fread(encInfo->secret_data, encInfo->size_secret_file, 1, encInfo->fptr_secret); // Read secret file

    char buffer[8]; // 8-byte temp buffer for image data
    for (size_t i = 0; i < encInfo->size_secret_file; i++)
    {
        fread(buffer, 8, 1, encInfo->fptr_src_image);                   // Read 8 bytes from image
        encode_byte_to_lsb(encInfo->secret_data[i], buffer);            // Encode 1 byte of secret
        fwrite(buffer, 8, 1, encInfo->fptr_stego_image);                // Write to stego image
    }

    printf("8. secret file data encoded successfully\n");
    return e_success;
}


Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    char ch;
    //get to know the total image capacity,taken then substract it with the secret file size
   //and write the remaining info into destination file 
    while(fread(&ch, 1, 1, fptr_src))
    {
        fwrite(&ch, 1, 1, fptr_dest);                                       // Write each byte to destination
    }

    printf("9. Remaining image data copied successfully...\n");
    return e_success;

}

Status encode_byte_to_lsb(char data, char *image_buffer)
{
    for(size_t i=0; i<8;i++)
    {
        image_buffer[i] = (image_buffer[i] & (~1)) | ((data >> (7-i)) &1);
             // printf("%hhx\n", image_buffer[i]);
    }
            //  printf("encoding byte to lsb is successful...\n");
    return e_success;

}

Status encode_size_to_lsb(int size, char *imageBuffer)                 //encoding size to lsb 
{
     for(size_t i=0; i<32; i++)
    {
        imageBuffer[i] = (imageBuffer[i] & (~1)) | ((size >> 31-i) &1);
    }
    //  printf("encoding size to lsb is successful...\n");
    return e_success;
}

Status do_encoding(EncodeInfo *encInfo)                                       // function calls for encoding  
{
    if(open_files(encInfo)== e_failure)                                         //call the open_files(encInfo)
    {
        printf("ERROR : Unable to open required files\n");
        return e_failure;
    }

    //call the check_capacity
    if(check_capacity(encInfo) == e_failure)                                 //function call for check capacity
    {
        printf("ERROR : Image does not have enough capacity\n");
        return e_failure;
    }

    rewind(encInfo->fptr_src_image);
    //call the copy_bmp_header(src_file,dest_file)

    if(copy_bmp_header(encInfo -> fptr_src_image,encInfo ->fptr_stego_image) == e_failure)  //function call for copy bmp header
    {
        printf("ERROR : copy bmp header failed\n");
        return e_failure;
    }
    //call the encode_magic_string(MAGIC_STRING,encInfo);
    if(encode_magic_string(MAGIC_STRING,encInfo) == e_failure)                      //function call for encode magic string 
    {
        printf("ERROR : Encoding magic string is failed\n");
        return e_failure;
    }

    //call the secret file extn size
    int extn_size = strlen(encInfo -> extn_secret_file);
    if(encode_secret_file_extn_size(extn_size, encInfo) == e_failure)           //function call for encode secret file extension size    
    {
        printf("ERROR : Encoding secret file extension size is failed\n");
        return e_failure;
    }
    // call the function secret_file_ext
    if(encode_secret_file_extn(encInfo->extn_secret_file,encInfo) == e_failure)    //function call for encode secret file extension file 
    
    {
        printf("ERROR : Encoding secret file extension is failed\n");
        return e_failure;
    }

    //call the secret file size
    long secret_size = encInfo->size_secret_file;                          //long size
    if(encode_secret_file_size(secret_size,encInfo) == e_failure)          //function 
    {
        printf("ERROR : Encoding the secret file size is failed\n");
        return e_failure;
    }
    //call the  secret file data
    if(encode_secret_file_data(encInfo) == e_failure)                                  //function call for encode secret file extn file  
    {
        printf("ERROR : Encoding secret file data is failed\n");
        return e_failure;
    }
    // call the function copy remaining data 
    if(copy_remaining_img_data(encInfo -> fptr_src_image,encInfo -> fptr_stego_image) == e_failure)         //function call for copy remaining image data
    {
        printf("ERROR : Coping the remaining image data  failed\n");
        return e_failure;
    }
    printf("\nEncoding Completed Successfully...\n");                               //encoding successful
    return e_success;

    // after copy_remaining_img_data and before return
    fclose(encInfo->fptr_src_image);                        //close source image file
    fclose(encInfo->fptr_secret);                           //close secret file
    fclose(encInfo->fptr_stego_image);                      // close the destination file
}