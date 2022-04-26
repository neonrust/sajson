#include "sajson_dump.h"

#include <stdio.h>

#include <assert.h>

#include <chrono>

static constexpr size_t MAX_BUFFER_SIZE = 1 << 24;
static constexpr size_t CHUNK_SIZE = 1 << 16;

int main(int argc, char *argv[]) {
    FILE* file = stdin;

    if(argc > 1) {
        file = fopen(argv[1], "rb");
        if (!file) {
            fprintf(stderr, "Failed to open file\n");
            return 1;
        }
    }

    std::string buffer;
    buffer.resize(CHUNK_SIZE);

    size_t offset = 0;
    while(!feof(file)) {
        if(offset + CHUNK_SIZE >= MAX_BUFFER_SIZE)
        {
            fprintf(stderr, "file is too large (> %lu MiB)\n", MAX_BUFFER_SIZE/(1 << 20));
            return 1;
        }

        size_t n = fread(&buffer[offset], 1, CHUNK_SIZE, file);
        if(n < CHUNK_SIZE) // did not read a complete chunk, i.e. we read until EOF
        {
            buffer.resize(offset + n);
            break;
        }
        offset += CHUNK_SIZE;   // advance offset to after the chunk that was just read
        buffer.resize(offset + CHUNK_SIZE); // and make space for the next chunk
        fprintf(stderr, "read %lu KiB ...\n", offset/(1 << 10));
    }
    if(file != stdin)
        fclose(file);

    auto t0 = std::chrono::high_resolution_clock::now();
    const auto &document = sajson::parse(sajson::dynamic_allocation(), sajson::mutable_string_view(buffer));
    auto t1 = std::chrono::high_resolution_clock::now();
    assert(document.is_valid());

    auto root = document.get_root();

    //auto s = sajson::to_string(root, true);

    auto t2 = std::chrono::high_resolution_clock::now();
    sajson::write(stdout, root, true);
    auto t3 = std::chrono::high_resolution_clock::now();

    fprintf(stderr, "deserialization time: %.3f ms\n", static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count())/1e3);
    fprintf(stderr, "serialization time: %.3f ms\n", static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(t3 - t2).count())/1e3);

    return 0;
}
