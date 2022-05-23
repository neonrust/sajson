#include <sajson.h>
#include <sajson_dump.h>

#include <memory>
#include <vector>
#include <chrono>
#include <cstdio>

using namespace std::chrono;

const std::vector<std::string> default_files {
    "testdata/apache_builds.json", "testdata/github_events.json",
    "testdata/instruments.json",   "testdata/mesh.json",
    "testdata/mesh.pretty.json",   "testdata/nested.json",
    "testdata/svg_menu.json",      "testdata/truenull.json",
    "testdata/twitter.json",       "testdata/update-center.json",
    "testdata/whitespace.json",
};

template <typename AllocationStrategy>
void run_benchmark(size_t N, size_t max_string_length, const std::string &filename) {
    std::FILE* file = std::fopen(filename.c_str(), "rb");
    if (!file) {
        perror("fopen failed");
        return;
    }

    std::unique_ptr<FILE, int (*)(FILE*)> deleter(file, fclose);

    if (std::fseek(file, 0, SEEK_END)) {
        perror("fseek failed");
        return;
    }
    size_t length = static_cast<size_t>(ftell(file));
    if (std::fseek(file, 0, SEEK_SET)) {
        perror("fseek failed");
        return;
    }

    std::vector<char> buffer(length);
    if (std::fread(buffer.data(), length, 1, file) != 1) {
        perror("fread failed");
        return;
    }

    deleter.reset();

    microseconds minimum_each { std::numeric_limits<clock_t>::max() };
    const auto start = high_resolution_clock::now();

    printf("%*s   ",static_cast<int>(max_string_length), filename.c_str());
    std::fflush(stdout);

    for (size_t i = 0; i < N; ++i) {
        const auto before_each = high_resolution_clock::now();;

        sajson::parse(AllocationStrategy(), std::string_view(buffer.data(), buffer.size()));

        const auto elapsed_each = duration_cast<microseconds>(high_resolution_clock::now() - before_each);
        minimum_each = std::min(minimum_each, elapsed_each);
    }

    const auto elapsed = duration_cast<microseconds>(high_resolution_clock::now() - start);

    const double average_elapsed_ms = static_cast<double>(elapsed.count()) / 1e3 / static_cast<double>(N);
    const double minimum_elapsed_ms = static_cast<double>(minimum_each.count()) / 1e3;

    printf("%0.3f ms   %0.3f ms\n", average_elapsed_ms, minimum_elapsed_ms);
}


static void run_dump_benchmark(size_t N, size_t max_string_length, const std::string &filename) {

    printf("%*s   reading...", static_cast<int>(max_string_length), filename.c_str());
    std::fflush(stdout);

    std::FILE* file = std::fopen(filename.c_str(), "rb");
    if (!file) {
        perror("fopen failed");
        return;
    }

    std::unique_ptr<FILE, int (*)(FILE*)> deleter(file, fclose);

    if (std::fseek(file, 0, SEEK_END)) {
        perror("fseek failed");
        return;
    }
    size_t length = static_cast<size_t>(ftell(file));
    if (std::fseek(file, 0, SEEK_SET)) {
        perror("fseek failed");
        return;
    }

    std::vector<char> buffer(length);
    if (std::fread(buffer.data(), length, 1, file) != 1) {
        perror("fread failed");
        return;
    }

    deleter.reset();

    const auto doc = sajson::parse(sajson::single_allocation(), std::string_view(buffer.data(), buffer.size()));
    // erase the line
    printf("\r%*s\r", static_cast<int>(max_string_length + 13), "");

    microseconds minimum_each { std::numeric_limits<clock_t>::max() };
    const auto start = high_resolution_clock::now();

    std::FILE *dev_null = std::fopen("/dev/null", "wb");

    printf("%*s   ",static_cast<int>(max_string_length), filename.c_str());
    std::fflush(stdout);

    for (size_t i = 0; i < N; ++i) {
        const auto before_each = high_resolution_clock::now();;
        sajson::write(dev_null, doc.get_root());
        const auto elapsed_each = duration_cast<microseconds>(high_resolution_clock::now() - before_each);
        minimum_each = std::min(minimum_each, elapsed_each);
    }

    std::fclose(dev_null);

    const auto elapsed = duration_cast<microseconds>(high_resolution_clock::now() - start);

    const double average_elapsed_ms = static_cast<double>(elapsed.count()) / 1e3 / static_cast<double>(N);
    const double minimum_elapsed_ms = static_cast<double>(minimum_each.count()) / 1e3;

    printf("%0.3f ms   %0.3f ms\n", average_elapsed_ms, minimum_elapsed_ms);
}


static size_t print_header(const std::vector<std::string> &files)
{
    const auto max_string_length = std::max_element(files.begin(), files.end(), [](const auto &A, const auto &B) {
        return A.size() < B.size();
    })->size();

    printf(
        "%*s   %8s   %8s\n",
        static_cast<int>(max_string_length),
        "file",
        "avg",
        "min");
    printf(
        "%*s   %8s   %8s\n",
        static_cast<int>(max_string_length),
        "----",
        "---",
        "---");

    return max_string_length;
}

template <typename AllocationStrategy>
void run_all(size_t N, const std::vector<std::string> &files) {
    const auto max_string_length = print_header(files);

    for (const auto &fname: files) {
        run_benchmark<AllocationStrategy>(N, max_string_length, fname);
    }
}

static void run_dump_all(size_t N, const std::vector<std::string> &files) {
    const auto max_string_length = print_header(files);

    for (const auto &fname: files) {
        run_dump_benchmark(N, max_string_length, fname);
    }
}

int main(int argc, const char** argv) {

    const auto parse_N = 1000;
    const auto write_N = 100;


    printf("benchmark: sajson::parse() [%d]...\n", parse_N);

    if (argc > 1) {
        // printf("\n=== SINGLE ALLOCATION ===\n\n");
//        run_all<sajson::single_allocation>(parse_N, { argv[1] });
        // printf("\n=== DYNAMIC ALLOCATION ===\n\n");
        // run_all<sajson::dynamic_allocation>(argc - 1, argv + 1);
        printf("benchmark: sajson::write() [%d]...\n", write_N);
        run_dump_all(write_N, { argv[1] });
    } else {
        // printf("\n=== SINGLE ALLOCATION ===\n\n");
        run_all<sajson::single_allocation>(parse_N, default_files);
        // printf("\n=== DYNAMIC ALLOCATION ===\n\n");
        // run_all<sajson::dynamic_allocation>(default_files_count,
        // default_files);
    }
}
