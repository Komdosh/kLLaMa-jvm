cmake_minimum_required(VERSION 3.31.0)
project(llama)

set(CMAKE_CXX_STANDARD 17)

# JNI headers
find_package(JNI REQUIRED)
include_directories(${JNI_INCLUDE_DIRS})

set(LLAMA_DIR "../llama.cpp")

set(LLAMA_GGML_DIR ${LLAMA_DIR}/ggml)
set(LLAMA_COMMON_DIR ${LLAMA_DIR}/common)
set(LLAMA_SOURCES
        src/main/cpp/build-info.cpp
        ${LLAMA_GGML_DIR}/src/gguf.cpp

        ${LLAMA_GGML_DIR}/src/ggml.c
        ${LLAMA_GGML_DIR}/src/ggml-alloc.c
        ${LLAMA_GGML_DIR}/src/ggml-backend.cpp
        ${LLAMA_GGML_DIR}/src/ggml-backend-reg.cpp

        ${LLAMA_GGML_DIR}/src/ggml-cpu/ggml-cpu.c
        ${LLAMA_GGML_DIR}/src/ggml-cpu/ggml-cpu.cpp
        ${LLAMA_GGML_DIR}/src/ggml-cpu/ggml-cpu-aarch64.cpp
        ${LLAMA_GGML_DIR}/src/ggml-cpu/ggml-cpu-quants.c
        ${LLAMA_GGML_DIR}/src/ggml-cpu/ggml-cpu-traits.cpp
        
        ${LLAMA_GGML_DIR}/src/ggml-opt.cpp
        ${LLAMA_GGML_DIR}/src/ggml-threading.cpp
        ${LLAMA_GGML_DIR}/src/ggml-quants.c        

        ${LLAMA_DIR}/src/llama.cpp
        ${LLAMA_DIR}/src/llama-arch.cpp
        ${LLAMA_DIR}/src/llama-adapter.cpp
        ${LLAMA_DIR}/src/llama-context.cpp
        ${LLAMA_DIR}/src/llama-batch.cpp
        ${LLAMA_DIR}/src/llama-grammar.cpp
        
        ${LLAMA_DIR}/src/llama-model.cpp
        ${LLAMA_DIR}/src/llama-model-loader.cpp
        ${LLAMA_DIR}/src/llama-mmap.cpp
        ${LLAMA_DIR}/src/llama-impl.cpp
        ${LLAMA_DIR}/src/llama-hparams.cpp
        ${LLAMA_DIR}/src/llama-kv-cache.cpp
        ${LLAMA_DIR}/src/llama-sampling.cpp
        ${LLAMA_DIR}/src/llama-vocab.cpp
        
        ${LLAMA_DIR}/src/llama-chat.cpp
        ${LLAMA_DIR}/src/unicode.h
        ${LLAMA_DIR}/src/unicode.cpp
        ${LLAMA_DIR}/src/unicode-data.cpp

        ${LLAMA_COMMON_DIR}/chat.cpp
        ${LLAMA_COMMON_DIR}/arg.cpp
        ${LLAMA_COMMON_DIR}/base64.hpp
        ${LLAMA_COMMON_DIR}/common.cpp
        ${LLAMA_COMMON_DIR}/console.cpp
        ${LLAMA_COMMON_DIR}/json.hpp
        ${LLAMA_COMMON_DIR}/json-schema-to-grammar.cpp
        ${LLAMA_COMMON_DIR}/log.cpp
        ${LLAMA_COMMON_DIR}/ngram-cache.cpp
        ${LLAMA_COMMON_DIR}/sampling.cpp
)

add_library(llama SHARED ${LLAMA_SOURCES})
target_include_directories(
        llama
        PUBLIC
        ${LLAMA_GGML_DIR}/include
        ${LLAMA_GGML_DIR}/src
        ${LLAMA_GGML_DIR}/src/ggml-cpu
        ${LLAMA_DIR}/include
        ${LLAMA_COMMON_DIR}
)

target_compile_options(
        llama
        PUBLIC
        -DGGML_USE_CPU -DGGML_USE_CPU_AARCH64 -O3
)

# Export symbols for JNI
set_target_properties(llama PROPERTIES
    POSITION_INDEPENDENT_CODE ON
    CXX_VISIBILITY_PRESET default
)