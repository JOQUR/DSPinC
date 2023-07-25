#ifndef _WAV_FILE_H
#define _WAV_FILE_H


#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

typedef struct Header
{
    uint8_t riff[4];
    uint32_t overall_size;
    uint8_t wave[4];
} Header;

typedef struct fmt
{
    uint8_t subchunk_id[4];
    uint32_t subchunk1_size;
    uint16_t format;
    uint16_t num_channels;
    uint32_t sample_rate;
    uint32_t byte_rate;
    uint16_t block_allign;
    uint16_t bitsPerSample;

} Fmt;

typedef struct data
{
    uint8_t subchunk2_id[4];
    uint32_t subchunk2_size;
    uint8_t* data;
} Data;

typedef struct WavFile
{
    Header header;
    Fmt fmt;
    Data data;
} WavFile;

bool wavFile_Init(char* path);

WavFile wavFile_GetFile(void);


#endif