import struct

def read_mmi_file(filename):
    with open(filename, "rb") as f:
        # Verify the magic number
        magic = f.read(4)
        if magic != b"MMI\0":
            raise ValueError("Invalid .mmi file: Magic number mismatch")

        # Read the header fields
        kmer_size, seed_width, bucket_bits, num_sequences, flags = struct.unpack("<5I", f.read(20))
        print(f"MMI Header:")
        print(f"  k-mer size: {kmer_size}")
        print(f"  Seed width: {seed_width}")
        print(f"  Bucket bits: {bucket_bits}")
        print(f"  Number of sequences: {num_sequences}")
        print(f"  Flags: 0x{flags:x}")
        print()

        # Read sequences
        print("Sequences:")
        sequences = []
        for i in range(num_sequences):
            name_length = struct.unpack("<B", f.read(1))[0]  # Read 1 byte for the name length
            name = f.read(name_length).decode("utf-8")       # Read the name
            seq_length = struct.unpack("<I", f.read(4))[0]  # Read 4 bytes for the sequence length
            sequences.append((name, seq_length))

        for i, (name, length) in enumerate(sequences, start=1):
            print(f"  [{i}] Name: {name}, Length: {length}")

if __name__ == "__main__":
    import sys
    if len(sys.argv) != 2:
        print(f"Usage: {sys.argv[0]} <index.mmi>")
        sys.exit(1)

    filename = sys.argv[1]
    try:
        read_mmi_file(filename)
    except Exception as e:
        print(f"Error: {e}")
