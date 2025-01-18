#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "minimap.h"
#include "mmpriv.h"

void print_mmi_metadata(const char *mmi_file) {
    FILE *fp = fopen(mmi_file, "rb");
    if (fp == NULL) {
        fprintf(stderr, "Error: Cannot open .mmi file '%s'\n", mmi_file);
        exit(1);
    }

    // Load the .mmi file
    mm_idx_t *mi = mm_idx_load(fp);
    fclose(fp);

    if (mi == NULL) {
        fprintf(stderr, "Error: Failed to load .mmi file '%s'\n", mmi_file);
        exit(1);
    }

    // Print general index parameters
    printf("MMI Metadata for '%s':\n", mmi_file);
    printf("  k-mer size: %d\n", mi->k);
    printf("  Seed width: %d\n", mi->w);
    printf("  Number of sequences: %d\n", mi->n_seq);

    // Print information for each reference sequence
    printf("Reference Sequences:\n");
    for (int i = 0; i < mi->n_seq; ++i) {
        printf("  [%d] Name: %s, Length: %d\n", i + 1, mi->seq[i].name, mi->seq[i].len);
    }

    // Print index statistics
    mm_idx_stat(mi);
    
    // Free the index structure
    mm_idx_destroy(mi);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <index.mmi>\n", argv[0]);
        return 1;
    }

    const char *mmi_file = argv[1];
    print_mmi_metadata(mmi_file);
    return 0;
}

