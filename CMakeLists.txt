add_library(picoi2clcd INTERFACE)
target_include_directories(picoi2clcd INTERFACE ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(picoi2clcd INTERFACE hardware_i2c)
target_sources(picoi2clcd INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/lcd.c
    ${CMAKE_CURRENT_LIST_DIR}/lcd.h
)