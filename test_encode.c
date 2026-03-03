#include <stdio.h>
#include <string.h>
#include "encode.h"     // encoding functions
#include "types.h"      // encode types
#include "decode.h"     // decoding functions
#include "typesd.h"     // decode types

OperationType check_operation_type(char *);  // check -e or -d

int main(int argc, char *argv[])
{
    OperationType op = check_operation_type(argv[1]);  // decide operation

    if(op == e_encode && argc >= 4)
    {
        EncodeInfo enc_info;   // info for encoding

        if(read_and_validate_encode_args(argv,&enc_info) == e_success) // check args
        {
            do_encoding(&enc_info);  // start encoding
        }
    }
    else if(op == e_decode && argc >= 3)
    {
        DecodeInfod dec_info;  // info for decoding

        if(read_and_validate_decode_args(argc,argv,&dec_info) == d_success) // check args
        {
             do_decoding(&dec_info);  // start decoding
        }
    }
    else
    {
        printf("Unsupported operation or insufficient arguments...\n"); // wrong input
    }
}

OperationType check_operation_type(char *symbol)
{
    if(strcmp(symbol,"-e") == 0)
    {
        printf("encoding started...\n");
        return e_encode;   // encode mode
    }

    if(strcmp(symbol,"-d") == 0)
    {
        printf("decoding started...\n");
        return e_decode;   // decode mode
    }
    else
    {
        printf("Unsupported operation type!\n");
        return e_unsupported;  // unknown input
    }
}