# CMakeList.txt: проект CMake для Compiler; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("Compiler")

# Добавьте источник в исполняемый файл этого проекта.
add_executable (Compiler "main.cpp"  "src/lexer/token/token.h" "src/lexer/lexer.h" "src/error_handle/error_handle.h"  "src/parser/parser.h" )

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Compiler PROPERTY CXX_STANDARD 20)
endif()

# TODO: Добавьте тесты и целевые объекты, если это необходимо.
