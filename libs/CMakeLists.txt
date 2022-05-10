include_directories(headers)

add_library(mylib STATIC ./FGetLine.c ./KGetLine.c ./MyString.c SortsSearch.c headers/SortsSearch.h MyMath.c headers/MyMath.h)