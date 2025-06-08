#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "minimap.h"
#include "mmpriv.h"

void print_mmi_metadata(const char *mmi_file)
{
    FILE *fp = fopen(mmi_file, "rb");
    if (fp == NULL)
    {
        fprintf(stderr, "Error: Cannot open .mmi file '%s'\n", mmi_file);
        exit(1);
    }

    // Read and print magic number
    char magic[5] = {0};
    if (fread(magic, 1, 4, fp) != 4)
    {
        fprintf(stderr, "Error: Failed to read magic number\n");
        fclose(fp);
        exit(1);
    }
    printf("Magic number (raw): \"");
    for (int i = 0; i < 4; ++i)
    {
        if (magic[i] >= 32 && magic[i] <= 126)
            putchar(magic[i]);
        else
            printf("\\u%04x", (unsigned char)magic[i]);
    }
    printf("\"\n");

    // Read header fields
    int32_t seed_width, kmer_size, bucket_bits, num_sequences, flags;
    if (fread(&seed_width, sizeof(int32_t), 1, fp) != 1 ||
        fread(&kmer_size, sizeof(int32_t), 1, fp) != 1 ||
        fread(&bucket_bits, sizeof(int32_t), 1, fp) != 1 ||
        fread(&num_sequences, sizeof(int32_t), 1, fp) != 1 ||
        fread(&flags, sizeof(int32_t), 1, fp) != 1)
    {
        fprintf(stderr, "Error: Failed to read header fields\n");
        fclose(fp);
        exit(1);
    }

    printf("MMI Header:\n");
    printf("  k-mer size: %d\n", kmer_size);
    printf("  Seed width: %d\n", seed_width);
    printf("  Bucket bits: %d\n", bucket_bits);
    printf("  Number of sequences: %d\n", num_sequences);
    printf("  Flags: 0x%x\n\n", flags);

    printf("Sequences:\n");
    printf("  Idx\tName\tLength\n");
    for (int i = 0; i < num_sequences; ++i)
    {
        uint8_t name_length;
        if (fread(&name_length, sizeof(uint8_t), 1, fp) != 1)
        {
            fprintf(stderr, "Error: Failed to read name length\n");
            fclose(fp);
            exit(1);
        }
        char name[256] = {0};
        if (fread(name, 1, name_length, fp) != name_length)
        {
            fprintf(stderr, "Error: Failed to read name\n");
            fclose(fp);
            exit(1);
        }
        int32_t seq_length;
        if (fread(&seq_length, sizeof(int32_t), 1, fp) != 1)
        {
            fprintf(stderr, "Error: Failed to read sequence length\n");
            fclose(fp);
            exit(1);
        }
        printf("  [%d]\t%s\t%d\n", i + 1, name, seq_length);
    }
    fclose(fp);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <index.mmi>\n", argv[0]);
        return 1;
    }

    const char *mmi_file = argv[1];
    print_mmi_metadata(mmi_file);
    return 0;
}
