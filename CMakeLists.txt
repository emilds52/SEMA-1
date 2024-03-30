cmake_minimum_required(VERSION 3.15)

project(semaphoros
  LANGUAGES
    C
)

include_directories(src)

set(HEADER_FILES src/fsm.h src/model.h)

add_executable(test src/main.c src/fsm.c src/model.c ${HEADER_FILES})
