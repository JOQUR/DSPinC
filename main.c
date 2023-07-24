#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "inc/test.h"
#include "inc/wavFile.h"

extern WavFile wavFile;
void printInfo(void);
void saveData(void);

int main(int argc, char** argv)
{
    bool ret = wavFile_Init("C:\\Users\\PKier\\Downloads\\sample-9s.wav");
    assert(true == ret);
    (void)wavFile_GetFile();
    printInfo();
    return 0;
}

void printInfo(void)
{
    for(int i = 0; i < sizeof(wavFile.header.riff); i++)
    {
        printf("%c", wavFile.header.riff[i]);
    }

    printf("\r\n");
    printf("size = %d\r\n", wavFile.header.overall_size);

    printf("wave_id = ");
    for(int i = 0; i < sizeof(wavFile.header.wave); i++)
    {
        printf("%c", wavFile.header.wave[i]);
    }
    printf("\r\n");
    printf("Audio format = %d\r\n", wavFile.fmt.format);
    printf("Sample Rate = %d\r\n", wavFile.fmt.sample_rate);
    printf("Subchunk1 size = %d\r\n", wavFile.fmt.subchunk1_size);
    printf("Num channels = %d\r\n", wavFile.fmt.num_channels);
    printf("bits per sample = %d\r\n", wavFile.fmt.bitsPerSample);
    printf("byte rate = %d\r\n", wavFile.fmt.byte_rate);
    
    printf("data_id = %s\r\n", wavFile.data.subchunk2_id);
    
    printf("data size = %d\r\n", wavFile.data.subchunk2_size);
    saveData();


    free(wavFile.data.data);
}

void saveData(void)
{
    FILE* ptr;

    ptr = fopen("C:\\Users\\PKier\\Downloads\\x.txt", "w");
    const char* x = wavFile.data.data;
    for(size_t i = 0; i < wavFile.data.subchunk2_size; i++)
    {
        fprintf(ptr, "i[%d] = %d\r\n", i, x[i]);
    }
    fclose(ptr);
}