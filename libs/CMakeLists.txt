include_directories(headers)

# SET(GCC_COVERAGE_LINK_FLAGS    "-lm")
# SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")
# SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")

add_library(mylib STATIC ./FGetLine.c ./KGetLine.c ./MyString.c SortsSearch.c headers/SortsSearch.h MyMath.c headers/MyMath.h)
target_link_libraries(mylib m)