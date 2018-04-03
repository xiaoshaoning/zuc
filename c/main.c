#include "zuc.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

int main(void) 
{
    const uint64_t BUFFER_SIZE = 4096 / sizeof(uint32_t);
    uint8_t key[16], iv[16];
    uint32_t * buffer = (uint32_t *)calloc(BUFFER_SIZE, sizeof(uint32_t));
    const uint64_t TEST_SIZE = 1*1024; // 10 * 1024 MiB.
    uint64_t index;
    time_t start, stop;
    double time_used;
    zuc_context context;

    memset(key, 0, 16);
    memset(iv, 0, 16);
    
    zuc_init(&context, key, iv);
    zuc_generate_keystream(&context, buffer, 1024 / 4);
    
    printf("%x, %x\n", buffer[0], buffer[1]); // 27bede74, 18082da
    if (buffer[0] == 0x27bede74 && buffer[1] == 0x18082da) {
        puts("Implemented correctly.");
    }
    else 
    {
        puts("Wrong output");
        exit(EXIT_FAILURE);
    }
    
    puts("Benchmarking performance.");
    start = time(NULL);
    for (index = 0; index < (TEST_SIZE * 1024 * 1024) / (sizeof(uint32_t) * BUFFER_SIZE); ++index) 
    {
        zuc_generate_keystream(&context, buffer, BUFFER_SIZE);
    }
    
    stop = time(NULL);
    time_used = (double) (stop - start);
    
    printf("Speed: %lfMiB/s\n",((double) TEST_SIZE) / time_used);
    
    free(buffer);
}
