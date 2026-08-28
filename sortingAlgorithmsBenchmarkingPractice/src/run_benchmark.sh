#!/bin/bash

set -e

SOURCES="main.cpp generator.cpp sorting_algorithms.cpp memory_tracker.cpp"
CXXFLAGS="-std=c++17"

echo "=========================================="
echo "Компиляция бенчмарков"
echo "=========================================="

echo "[1/10] Компиляция: g++ -O0"
g++ -O0 $CXXFLAGS -o benchmark_gcc_O0 $SOURCES

echo "[2/10] Компиляция: g++ -O2"
g++ -O2 -DSTACK_PROFILE_GCC_O2 $CXXFLAGS -o benchmark_gcc_O2 $SOURCES

echo "[3/10] Компиляция: g++ -O3"
g++ -O3 -DSTACK_PROFILE_GCC_O3 $CXXFLAGS -o benchmark_gcc_O3 $SOURCES

echo "[4/10] Компиляция: g++ -O3 -march=native"
g++ -O3 -march=native -DSTACK_PROFILE_GCC_NATIVE $CXXFLAGS -o benchmark_gcc_native $SOURCES

echo "[5/10] Компиляция: g++ -O0 -march=native"
g++ -O0 -march=native $CXXFLAGS -o benchmark_gcc_march $SOURCES

echo "[6/10] Компиляция: clang++ -O0"
clang++ -O0 -DSTACK_PROFILE_CLANG_O0 $CXXFLAGS -o benchmark_clang_O0 $SOURCES

echo "[7/10] Компиляция: clang++ -O2"
clang++ -O2 -DSTACK_PROFILE_CLANG_O2 $CXXFLAGS -o benchmark_clang_O2 $SOURCES

echo "[8/10] Компиляция: clang++ -O3"
clang++ -O3 -DSTACK_PROFILE_CLANG_O3 $CXXFLAGS -o benchmark_clang_O3 $SOURCES

echo "[9/10] Компиляция: clang++ -O3 -march=native"
clang++ -O3 -march=native -DSTACK_PROFILE_CLANG_NATIVE $CXXFLAGS -o benchmark_clang_native $SOURCES

echo "[10/10] Компиляция: clang++ -O0 -march=native"
clang++ -O0 -march=native -DSTACK_PROFILE_CLANG_O0 $CXXFLAGS -o benchmark_clang_march $SOURCES

echo ""
echo "=========================================="
echo "Все бенчмарки скомпилированы успешно"
echo "=========================================="
echo ""

echo "=========================================="
echo "Запуск бенчмарков"
echo "=========================================="

for BENCH in \
    benchmark_gcc_O0 \
    benchmark_gcc_O2 \
    benchmark_gcc_O3 \
    benchmark_gcc_native \
    benchmark_gcc_march \
    benchmark_clang_O0 \
    benchmark_clang_O2 \
    benchmark_clang_O3 \
    benchmark_clang_native \
    benchmark_clang_march
do
    echo ""
    echo "------------------------------------------"
    echo "Запуск: ./$BENCH"
    echo "------------------------------------------"
    ./"$BENCH"
done

echo ""
echo "=========================================="
echo "Все бенчмарки завершены"
echo "=========================================="