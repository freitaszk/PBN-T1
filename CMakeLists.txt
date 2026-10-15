cmake_minimum_required (VERSION 3.5)

project(bagunceitor C)

include_directories(include)

add_executable(bagunceitor main.c imagem.c chave.c confusao.c difusao.c)

target_link_libraries(bagunceitor m)
