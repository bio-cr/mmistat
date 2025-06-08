VERSION = {{ `shards version #{__DIR__}`.chomp.stringify }}
USAGE   = <<-USAGE
  Program: mmistat (minimap2 MMI file statistics) 
  Version: #{VERSION}
  Source:  https://github.com/bio-cr/mmistat

    Usage:
      mmistat <file.mmi>

  USAGE

def main
  File.open(ARGV[0], "rb") do |file|
    # Read and verify the magic number
    magic = file.read_string(4)
    puts "Magic number (raw): #{magic.inspect}"
    if magic != "MMI\2"
      raise "Invalid .mmi file: Magic number mismatch"
    end

    # Read the header values
    seed_width = file.read_bytes(Int32)
    kmer_size = file.read_bytes(Int32)
    bucket_bits = file.read_bytes(Int32)
    num_sequences = file.read_bytes(Int32)
    flags = file.read_bytes(Int32)

    puts "MMI Header:"
    puts "  k-mer size: #{kmer_size}"
    puts "  Seed width: #{seed_width}"
    puts "  Bucket bits: #{bucket_bits}"
    puts "  Number of sequences: #{num_sequences}"
    puts "  Flags: 0x#{flags.to_s(16)}"
    puts

    # Read sequence information
    puts "Sequences:"
    num_digits = num_sequences.to_s.size
    puts "  Idx\tName\tLength"
    num_sequences.times do |i|
      name_length = file.read_bytes(UInt8)
      name = file.read_string(name_length)
      seq_length = file.read_bytes(Int32)
      puts "  [#{(i + 1).to_s.rjust(num_digits)}]\t#{name}\t#{seq_length}"
    end
  end
rescue e
  STDERR.puts USAGE
  STDERR.puts "\e[35m#{e.message}: \e[0m"
  exit 1
end

case ARGV.size
when 0 then STDERR.puts(USAGE); exit(1)
when 1 then main
else
  STDERR.puts USAGE
  STDERR.puts "\e[35mToo many arguments.\e[0m"
  exit 1
end
