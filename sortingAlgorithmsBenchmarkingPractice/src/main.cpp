#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <iomanip>

#include "generator.h"
#include "sorting_algorithms.h"
#include "memory_tracker.h"


// информация о размерах фреймов стека для расчета потребления памяти стека.
// значения получены из вывода компилятора с флагом -fstack-usage.
struct FrameInfo {
    size_t base_frame;       // фрейм обертки
    size_t recursive_frame;  // фрейм функции выполняющей рекурсию
    size_t child_frame;      // фрейм вспомогательной функции внутри рекурсии
    bool is_recursive;
};

// результат бенчмарка для одной сортировки
struct BenchmarkResult {
    std::string algorithm_name;
    size_t array_size;
    std::string data_type;
    double median_ms;    // медианное время в миллисекундах
    double min_ms;       // минимальное время
    double max_ms;       // максимальное время
    size_t recursion_depth;       // максимальная глубина рекурсии
    size_t peak_heap_bytes;       // пиковое потребление кучи
    size_t estimated_stack_bytes; // расчетное потребление стека
    size_t total_memory_bytes;    // общая память (куча + стек)
};

// описание сортировки для бенчмарка
struct SortDescriptor {
    std::string name;
    void (*sort_function)(std::vector<uint32_t>&);
    FrameInfo frames;
};

// функция бенчмаркинга
BenchmarkResult run_benchmark(const std::string& name, void (*sort_func)(std::vector<uint32_t>&),
                            const std::vector<uint32_t>& source, size_t repeats,
                            const std::string& data_type, const FrameInfo& frames) {
    std::vector<double> times;
    size_t max_depth = 0;
    size_t max_heap = 0;
    for(size_t i{0}; i < repeats; ++i) {
        std::vector<uint32_t> arr_copy = source;
        // cбрасываем трекер памяти, чтобы замерить только то, что использует сама сортировка.
        reset_memory_tracker();
        // замер времени
        auto start = std::chrono::high_resolution_clock::now();
        sort_func(arr_copy);
        auto end = std::chrono::high_resolution_clock::now();
        double ms = std::chrono::duration<double, std::milli>(end - start).count();
        times.push_back(ms);
        // запоминаем максимальные значения памяти по всем прогонам
        size_t heap = get_peak_memory_bytes();
        if (heap > max_heap) {
            max_heap = heap;
        }
        if(max_recursion_depth > max_depth) {
            max_depth = max_recursion_depth;
        }
    }
    // вычисляем медиану, минимум и максимум времени
    std::sort(times.begin(), times.end());
    double median = times[times.size() / 2];
    double min_val = times.front();
    double max_val = times.back();
    // рассчитываем потребление памяти стека
    // для рекурсивных: фрейм обертки + глубина рекурсии * фрейм рекурсии + фрейм помощника
    // для нерекурсивных: только фрейм обертки
    size_t stack_bytes = 0;
    if (frames.is_recursive && max_depth > 0) {
        stack_bytes = frames.base_frame + max_depth * frames.recursive_frame + frames.child_frame;
    } else {
        stack_bytes = frames.base_frame;
    }
    size_t total_memory = max_heap + stack_bytes;
    BenchmarkResult result;
    result.algorithm_name = name;
    result.array_size = source.size();
    result.data_type = data_type;
    result.median_ms = median;
    result.min_ms = min_val;
    result.max_ms = max_val;
    result.recursion_depth = max_depth;
    result.peak_heap_bytes = max_heap;
    result.estimated_stack_bytes = stack_bytes;
    result.total_memory_bytes = total_memory;
    return result;
}

// таблица сортировок с размерами фреймов
std::vector<SortDescriptor> get_sort_descriptors() {
#if defined(STACK_PROFILE_GCC_O2)
    // g++ -O2
    return {
        {"bubble_sort", bubble_sort, {8, 0, 0, false}},
        {"selection_sort", selection_sort, {8, 0, 0, false}},
        {"insertion_sort", insertion_sort, {8, 0, 0, false}},
        {"quick_sort_naive", quick_sort_naive, {8, 48, 8, true}},
        {"quick_sort_improved", quick_sort_improved, {8, 48, 8, true}},
        {"merge_sort_naive", merge_sort_naive, {8, 128, 0, true}},
        {"merge_sort_improved", merge_sort_improved, {80, 64, 40, true}},
        {"heap_sort_iterative", heap_sort_iterative, {32, 0, 0, false}},
        {"heap_sort_recursive", heap_sort_recursive, {40, 24, 0, true}},
        {"counting_sort", counting_sort, {32, 0, 0, false}},
        {"radix_sort", radix_sort, {2128, 0, 0, false}},
        {"bucket_sort", bucket_sort, {112, 0, 0, false}},
        {"builtin_sort", builtin_sort, {32, 0, 0, false}}
    };

#elif defined(STACK_PROFILE_GCC_O3)
    // g++ -O3
    return {
        {"bubble_sort", bubble_sort, {8, 0, 0, false}},
        {"selection_sort", selection_sort, {40, 0, 0, false}},
        {"insertion_sort", insertion_sort, {8, 0, 0, false}},
        {"quick_sort_naive", quick_sort_naive, {8, 48, 8, true}},
        {"quick_sort_improved", quick_sort_improved, {8, 48, 8, true}},
        {"merge_sort_naive", merge_sort_naive, {80, 112, 0, true}},
        {"merge_sort_improved", merge_sort_improved, {128, 112, 56, true}},
        {"heap_sort_iterative", heap_sort_iterative, {48, 0, 0, false}},
        {"heap_sort_recursive", heap_sort_recursive, {32, 24, 0, true}},
        {"counting_sort", counting_sort, {32, 0, 0, false}},
        {"radix_sort", radix_sort, {2128, 0, 0, false}},
        {"bucket_sort", bucket_sort, {128, 0, 0, false}},
        {"builtin_sort", builtin_sort, {64, 0, 0, false}}
    };

#elif defined(STACK_PROFILE_GCC_NATIVE)
    // g++ -O3 -march=native
    return {
        {"bubble_sort", bubble_sort, {8, 0, 0, false}},
        {"selection_sort", selection_sort, {40, 0, 0, false}},
        {"insertion_sort", insertion_sort, {8, 0, 0, false}},
        {"quick_sort_naive", quick_sort_naive, {8, 48, 8, true}},
        {"quick_sort_improved", quick_sort_improved, {8, 48, 8, true}},
        {"merge_sort_naive", merge_sort_naive, {128, 128, 0, true}},
        {"merge_sort_improved", merge_sort_improved, {112, 112, 64, true}},
        {"heap_sort_iterative", heap_sort_iterative, {48, 0, 0, false}},
        {"heap_sort_recursive", heap_sort_recursive, {32, 24, 0, true}},
        {"counting_sort", counting_sort, {64, 0, 0, false}},
        {"radix_sort", radix_sort, {2128, 0, 0, false}},
        {"bucket_sort", bucket_sort, {192, 0, 0, false}},
        {"builtin_sort", builtin_sort, {64, 0, 0, false}}
    };

#elif defined(STACK_PROFILE_CLANG_O0)
    // clang++ -O0
    return {
        {"bubble_sort", bubble_sort, {72, 0, 0, false}},
        {"selection_sort", selection_sort, {72, 0, 0, false}},
        {"insertion_sort", insertion_sort, {72, 0, 0, false}},
        {"quick_sort_naive", quick_sort_naive, {24, 56, 72, true}},
        {"quick_sort_improved", quick_sort_improved, {24, 56, 88, true}},
        {"merge_sort_naive", merge_sort_naive, {24, 200, 0, true}},
        {"merge_sort_improved", merge_sort_improved, {104, 56, 120, true}},
        {"heap_sort_iterative", heap_sort_iterative, {56, 0, 0, false}},
        {"heap_sort_recursive", heap_sort_recursive, {56, 72, 0, true}},
        {"counting_sort", counting_sort, {200, 0, 0, false}},
        {"radix_sort", radix_sort, {2248, 0, 0, false}},
        {"bucket_sort", bucket_sort, {296, 0, 0, false}},
        {"builtin_sort", builtin_sort, {40, 0, 0, false}}
    };

#elif defined(STACK_PROFILE_CLANG_O2)
    // clang++ -O2
    return {
        {"bubble_sort", bubble_sort, {0, 0, 0, false}},
        {"selection_sort", selection_sort, {0, 0, 0, false}},
        {"insertion_sort", insertion_sort, {0, 0, 0, false}},
        {"quick_sort_naive", quick_sort_naive, {0, 40, 0, true}},
        {"quick_sort_improved", quick_sort_improved, {0, 40, 0, true}},
        {"merge_sort_naive", merge_sort_naive, {0, 88, 0, true}},
        {"merge_sort_improved", merge_sort_improved, {56, 72, 48, true}},
        {"heap_sort_iterative", heap_sort_iterative, {16, 0, 0, false}},
        {"heap_sort_recursive", heap_sort_recursive, {32, 0, 0, true}},
        {"counting_sort", counting_sort, {56, 0, 0, false}},
        {"radix_sort", radix_sort, {2136, 0, 0, false}},
        {"bucket_sort", bucket_sort, {168, 0, 0, false}},
        {"builtin_sort", builtin_sort, {24, 0, 0, false}}
    };

#elif defined(STACK_PROFILE_CLANG_O3)
    // clang++ -O3
    return {
        {"bubble_sort", bubble_sort, {0, 0, 0, false}},
        {"selection_sort", selection_sort, {0, 0, 0, false}},
        {"insertion_sort", insertion_sort, {0, 0, 0, false}},
        {"quick_sort_naive", quick_sort_naive, {0, 40, 0, true}},
        {"quick_sort_improved", quick_sort_improved, {0, 40, 0, true}},
        {"merge_sort_naive", merge_sort_naive, {0, 88, 0, true}},
        {"merge_sort_improved", merge_sort_improved, {56, 72, 48, true}},
        {"heap_sort_iterative", heap_sort_iterative, {16, 0, 0, false}},
        {"heap_sort_recursive", heap_sort_recursive, {32, 0, 0, true}},
        {"counting_sort", counting_sort, {56, 0, 0, false}},
        {"radix_sort", radix_sort, {2136, 0, 0, false}},
        {"bucket_sort", bucket_sort, {168, 0, 0, false}},
        {"builtin_sort", builtin_sort, {24, 0, 0, false}}
    };

#elif defined(STACK_PROFILE_CLANG_NATIVE)
    // clang++ -O3 -march=native
    return {
        {"bubble_sort", bubble_sort, {0, 0, 0, false}},
        {"selection_sort", selection_sort, {32, 0, 0, false}},
        {"insertion_sort", insertion_sort, {0, 0, 0, false}},
        {"quick_sort_naive", quick_sort_naive, {0, 40, 0, true}},
        {"quick_sort_improved", quick_sort_improved, {0, 40, 0, true}},
        {"merge_sort_naive", merge_sort_naive, {0, 72, 0, true}},
        {"merge_sort_improved", merge_sort_improved, {56, 72, 48, true}},
        {"heap_sort_iterative", heap_sort_iterative, {32, 0, 0, false}},
        {"heap_sort_recursive", heap_sort_recursive, {48, 0, 0, true}},
        {"counting_sort", counting_sort, {56, 0, 0, false}},
        {"radix_sort", radix_sort, {2120, 0, 0, false}},
        {"bucket_sort", bucket_sort, {168, 0, 0, false}},
        {"builtin_sort", builtin_sort, {24, 0, 0, false}}
    };

#else
    // g++ -O0 (по умолчанию, также используется для -O0 -march=native)
    return {
        {"bubble_sort", bubble_sort, {80, 0, 0, false}},
        {"selection_sort", selection_sort, {80, 0, 0, false}},
        {"insertion_sort", insertion_sort, {80, 0, 0, false}},
        {"quick_sort_naive", quick_sort_naive, {32, 64, 96, true}},
        {"quick_sort_improved", quick_sort_improved, {32, 64, 96, true}},
        {"merge_sort_naive", merge_sort_naive, {32, 160, 0, true}},
        {"merge_sort_improved", merge_sort_improved, {96, 80, 112, true}},
        {"heap_sort_iterative", heap_sort_iterative, {80, 0, 0, false}},
        {"heap_sort_recursive", heap_sort_recursive, {80, 80, 0, true}},
        {"counting_sort", counting_sort, {160, 0, 0, false}},
        {"radix_sort", radix_sort, {2208, 0, 0, false}},
        {"bucket_sort", bucket_sort, {224, 0, 0, false}},
        {"builtin_sort", builtin_sort, {48, 0, 0, false}}
    };
#endif
}

// вспомогательные функции

std::string array_type_to_string(arr_type type) {
    switch (type) {
        case arr_type::RANDOM: 
            return "RANDOM";
        case arr_type::SORTED: 
            return "SORTED";
        case arr_type::REVERSE: 
            return "REVERSE";
        case arr_type::ALMOST_SORTED: 
            return "ALMOST_SORTED";
        default: 
            return "UNKNOWN";
    }
}

// адаптивное количество повторов в зависимости от размера массива.
size_t get_repeats(size_t size) {
    if(size == 10) {
        return 1000;
    }
    if(size == 500) {
        return 100;
    }
    if(size == 1000) {
        return 50;
    }
    if(size == 50000) {
        return 10;
    }
    return 3;
}

// функция для получения имени исполняемого файла, который создается при проведении бенчмарка
// используется в дальнейшем для создания файла basename.csv, куда будут выводится результаты
std::string get_base_name(const std::string& path) {
    size_t slash_pos = path.find_last_of("/\\");
    if (slash_pos == std::string::npos) {
        return path;
    }
    return path.substr(slash_pos + 1);
}

// бенчмарк

void run_benchmark_all(std::ostream& output) {
    const size_t sizes[] = {10, 500, 1000, 50000, 250000, 500000, 1000000};
    const arr_type types[] = {
        arr_type::RANDOM,
        arr_type::SORTED,
        arr_type::REVERSE,
        arr_type::ALMOST_SORTED
    };
    std::vector<SortDescriptor> sorts = get_sort_descriptors();
    // заголовок .csv
    output << "algorithm,size,data_type,median_ms,min_ms,max_ms,"
           << "recursion_depth,peak_heap_bytes,estimated_stack_bytes,total_memory_bytes\n";
    for(size_t i{0}; i < 7; ++i) {
        const size_t size = sizes[i]; 
        size_t repeats = get_repeats(size);
        for(size_t j{0}; j < 4; ++j) {
            const arr_type type = types[j];
            // генерируем массив 1 раз для данной комбинации и работаем с его копией
            // тогда все алгоритмы и все прогоны работают с одним и тем же массивом
            std::vector<uint32_t> source = gen_arr(size, type);
            std::string type_str = array_type_to_string(type);
            // подсказки во время бенчмарка, выводятся в консоль
            std::cerr << "Benchmarking size = " << size << " type = " << type_str << " repeats = " << repeats << "\n";
            for(const auto& sort : sorts) {
                // пропущены квадратичные сортировки на очень больших размерах
                // чтобы не ждать часами.
                bool is_quadratic = (sort.name == "bubble_sort" ||
                                     sort.name == "selection_sort" ||
                                     sort.name == "insertion_sort" ||
                                     sort.name == "quick_sort_naive");
                if (size >= 500000 && is_quadratic) {
                    std::cerr << "  Skipping " << sort.name << " (quadratic, size is too large)\n";
                    continue;
                }
                std::cerr << "  Running " << sort.name << "...\n";
                BenchmarkResult result = run_benchmark(
                    sort.name,
                    sort.sort_function,
                    source,
                    repeats,
                    type_str,
                    sort.frames
                );
                output << result.algorithm_name << ","
                       << result.array_size << ","
                       << result.data_type << ","
                       << std::fixed << std::setprecision(6) << result.median_ms << ","
                       << std::fixed << std::setprecision(6) << result.min_ms << ","
                       << std::fixed << std::setprecision(6) << result.max_ms << ","
                       << result.recursion_depth << ","
                       << result.peak_heap_bytes << ","
                       << result.estimated_stack_bytes << ","
                       << result.total_memory_bytes << "\n";
                output.flush();
            }
        }
    }
}

int main(int argc, char* argv[]) {
    std::string base_name = get_base_name(argv[0]);
    std::string output_file = base_name + ".csv";
    std::ofstream outfile(output_file);
    if(!outfile.is_open()) {
        std::cerr << "Cannot open output file: " << output_file << "\n";
        return 1;
    }
    std::cerr << "Starting benchmark. Results will be written to: " << output_file << "\n";
    run_benchmark_all(outfile);
    outfile.close();
    std::cerr << "Benchmark completed. Results saved to: " << output_file << "\n";
    return 0;
}