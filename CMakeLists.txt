cmake_minimum_required(VERSION 3.16)

project(DreamLang 
    VERSION 2.0.0
    DESCRIPTION "DreamLang"
    LANGUAGES CXX
)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(PkgConfig REQUIRED)
pkg_check_modules(GETTEXT REQUIRED)

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)

# Source files
set(LEXER_SOURCES
    src/lexer/lexical.cpp
    src/lexer/token.cpp
    src/lexer/token_type.cpp
    src/lexer/lexical_exception.cpp
)

set(I18N_SOURCES
    src/i18n/message_catalog.cpp
    src/i18n/locale_manager.cpp
)

set(CONFIG_SOURCES
    src/config/config_manager.cpp
)

set(CORE_SOURCES
    src/main.cpp
    ${LEXER_SOURCES}
    ${I18N_SOURCES}
    ${CONFIG_SOURCES}
)

# Create executable
add_executable(dreamlang ${CORE_SOURCES})

# Compiler flags
target_compile_options(dreamlang PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -O2
)

# Link libraries (if using libintl)
if(APPLE)
    # On macOS, we might need to link with libintl from homebrew
    find_library(LIBINTL_LIBRARIES NAMES intl libintl)
    if(LIBINTL_LIBRARIES)
        target_link_libraries(dreamlang ${LIBINTL_LIBRARIES})
    endif()
endif()

# Install target
install(TARGETS dreamlang DESTINATION bin)

# Install config file
install(FILES config.json DESTINATION .)

# Create locale directory structure and install .po files
set(LOCALE_DIR ${CMAKE_INSTALL_PREFIX}/share/locale)
set(LANGUAGES zh_CN en_US)

foreach(LANG ${LANGUAGES})
    install(DIRECTORY DESTINATION ${LOCALE_DIR}/${LANG}/LC_MESSAGES)
endforeach()

# Custom target for updating translations
add_custom_target(update-po
    COMMAND xgettext --language=C++ --keyword=_ --keyword=N_ --output=po/dreamlang.pot src/**/*.cpp include/**/*.h
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Updating translation template"
)

# Custom target for compiling .po files to .mo files
add_custom_target(compile-po
    COMMAND for lang in ${LANGUAGES}\; do msgfmt po/$$lang.po -o po/$$lang.mo\; done
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Compiling translation files"
)
