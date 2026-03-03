#ifndef TYPES_H
#define TYPES_H

/* User defined type for unsigned integers */
typedef unsigned int uint;

/* Status used as function return type */
typedef enum
{
    e_failure,                           // Operation failed
    e_success                            // Operation succeeded
} Status;

/* Operation type for encoding/decoding */
typedef enum
{
    e_encode,                            // Encode operation
    e_decode,                               // Decode operation
    e_unsupported                        // Unsupported operation
} OperationType;

#endif
