
/* --------------------------------------------------------------------------------------- */
/* Functions to handle MIL buffers residing in a chunk of sharable inter-process memory.   */
/* --------------------------------------------------------------------------------------- */


/* Shared memory handling functions specification. */
#define ALLOC_SHARED_MEMORY_NB_PARAM                 12
#define ALLOC_SHARED_MEMORY_OPCODE                    1 
#define ALLOC_SHARED_MEMORY_PARAMETER_ERROR_CODE      1
#define ALLOC_SHARED_MEMORY_MAPPING_FILE_ERROR_CODE   2

#define FREE_SHARED_MEMORY_NB_PARAM                 2
#define FREE_SHARED_MEMORY_OPCODE                   2 
#define FREE_SHARED_MEMORY_PARAMETER_ERROR_CODE     1

#define SHARED_MEMORY_ALLOCATE 1
#define SHARED_MEMORY_CREATE   2
#define SHARED_MEMORY_FREE     3

