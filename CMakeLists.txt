cmake_minimum_required(VERSION 3.20.0)


project(maplang)


set(SOURCES src/main.c
  src/parser/parser.c
  src/parser/lexer.c
  src/parser/ast.c
  src/interpreter/interpreter.c
  src/interpreter/memory.c

  src/stdlib/io.c
)

set(HEADERS include/parser/parser.h
  include/parser/lexer.h
  include/parser/ast.h
  include/interpreter/interpreter.h
  include/interpreter/bytecode.h
  include/interpreter/memory.h
  include/stdlib/io.h
)


add_executable(
  ${PROJECT_NAME}

  ${SOURCES}
  ${HEADERS}
)
