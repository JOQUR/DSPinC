#include "../inc/wavFile.h"
#include <stdlib.h>

static FILE* ptr;

static uint32_t little_to_big(uint8_t* arr, size_t len);
static Header wavFile_ReadHeader(void);
static Fmt wavFile_ReadFMT(void);
static Data wavFile_ReadData(void);

WavFile wavFile;

bool wavFile_Init(char* path)
{
    ptr = fopen(path, "rb");
    if(NULL == ptr)
    {
        return false;
    }

    return true;
}

static Header wavFile_ReadHeader(void)
{
    uint8_t ov_size[4];
    Header x;
    size_t size = fread(x.riff, sizeof(x.riff), 1, ptr);
    size = fread(ov_size, sizeof(ov_size), 1, ptr);
    size = fread(x.wave, sizeof(x.wave), 1, ptr);


    x.overall_size = little_to_big(ov_size, sizeof(ov_size));
    return x;
}

static Fmt wavFile_ReadFMT(void)
{
    Fmt fmt;
    uint8_t sc_size[4];
    uint8_t format[2];
    uint8_t num_channels[2];
    uint8_t sample_rate[4];
    uint8_t byte_rate[4];
    uint8_t block_allign[2];
    uint8_t bitsPerSample[2];
    size_t size = fread(fmt.subchunk_id, sizeof(fmt.subchunk_id), 1, ptr);
    size = fread(sc_size, sizeof(sc_size), 1, ptr);
    size = fread(format, sizeof(format), 1, ptr);
    size = fread(num_channels, sizeof(num_channels), 1, ptr);
    size = fread(sample_rate, sizeof(sample_rate), 1, ptr);
    size = fread(byte_rate, sizeof(byte_rate), 1, ptr);
    size = fread(block_allign, sizeof(block_allign), 1, ptr);
    size = fread(bitsPerSample, sizeof(bitsPerSample), 1, ptr);


    fmt.subchunk1_size = little_to_big(sc_size, sizeof(sc_size));
    fmt.format = little_to_big(format, sizeof(format));
    fmt.num_channels = little_to_big(num_channels, sizeof(num_channels));
    fmt.sample_rate = little_to_big(sample_rate, sizeof(sample_rate));
    fmt.byte_rate = little_to_big(byte_rate, sizeof(byte_rate));
    fmt.block_allign = little_to_big(block_allign, sizeof(block_allign));
    fmt.bitsPerSample = little_to_big(bitsPerSample, sizeof(bitsPerSample));
    return fmt;
}

static Data wavFile_ReadData(void)
{
    Data data;
    uint8_t data_size[4];
    size_t size = fread(data.subchunk2_id, sizeof(data.subchunk2_id), 1, ptr);
    size = fread(data_size, sizeof(data_size), 1, ptr);
    data.subchunk2_size = little_to_big(data_size, sizeof(data_size));
    data.data = (uint8_t*)malloc(sizeof(uint8_t) * data.subchunk2_size);
    size = fread(data.data, data.subchunk2_size, 1, ptr);

    return data;
}


WavFile wavFile_GetFile(void)
{
    wavFile.header = wavFile_ReadHeader();
    wavFile.fmt = wavFile_ReadFMT();
    wavFile.data = wavFile_ReadData();
    fclose(ptr);
    assert((wavFile.fmt.sample_rate * wavFile.fmt.bitsPerSample * wavFile.fmt.num_channels / 8) == wavFile.fmt.byte_rate);
    assert((4 + 8 + wavFile.fmt.subchunk1_size + 8 + wavFile.data.subchunk2_size) == wavFile.header.overall_size);
}


static uint32_t little_to_big(uint8_t* arr, size_t len)
{
    uint32_t temp = 0;
    for(int i = 0; i < len; i++)
    {
        temp |= arr[i] << (i * 8);
    }
    return temp;
}