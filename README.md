# LZ77 compression algorithm

This is standard lz77 compression algorithm implementation. Program takes arguments from command line.

## Usage

Usage is described by following grammer:

    lz77 <mode> [<options>] <input-file> <output-file>
    <mode>    ::= -encode | -decode
    <options> ::= --l <pi> [--s <pi>] | --s <pi> [--l <pi>]

    <pi>          is arbitrary positive integer
    --l              is parametrized optional argument setting lookahead size
    --s             is parametrized optional argument setting buffer size
    (buffer size must be lesser or equal to lookahead)
