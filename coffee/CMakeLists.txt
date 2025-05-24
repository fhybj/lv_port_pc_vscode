# coffee/CMakeLists.txt
file(GLOB_RECURSE COFFEE_SOURCES
    "*.c"
    "app/*.c"
    "ui/**/*.c"
    "lang/*.c"
    "data_manager/**/*.c"
    "comm/*.c"
    "drivers/*.c"
    "utils/*.c"
)

add_library(coffee_project STATIC ${COFFEE_SOURCES})

target_include_directories(coffee_project PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/app"
    "${CMAKE_CURRENT_SOURCE_DIR}/ui"
    "${CMAKE_CURRENT_SOURCE_DIR}/ui/screens/screen_making"
    "${CMAKE_CURRENT_SOURCE_DIR}/ui/screens/screen_info"
    "${CMAKE_CURRENT_SOURCE_DIR}/ui/screens/screen_splash"
    "${CMAKE_CURRENT_SOURCE_DIR}/ui/components/status_bar"
    "${CMAKE_CURRENT_SOURCE_DIR}/ui/assets/images" # 即使是占位符，也要包含
    "${CMAKE_CURRENT_SOURCE_DIR}/ui/theme"
    "${CMAKE_CURRENT_SOURCE_DIR}/lang"
    "${CMAKE_CURRENT_SOURCE_DIR}/data_manager"
    "${CMAKE_CURRENT_SOURCE_DIR}/data_manager/storage_backend"
    "${CMAKE_CURRENT_SOURCE_DIR}/comm"
    "${CMAKE_CURRENT_SOURCE_DIR}/drivers"
    "${CMAKE_CURRENT_SOURCE_DIR}/utils"
    # 添加 LVGL 核心头文件路径
    "${PROJECT_SOURCE_DIR}" # LVGL 的 src 目录，包含 lvgl.h 等
)

# 确保LVGL的头文件路径也被包含，通常由主CMakeLists传递
# target_link_libraries(coffee_project PRIVATE lvgl) # 在主CMakeLists中链接