idf_component_register(
    # Báo cho ESP-IDF biết các thư mục chứa file .c cần đem đi biên dịch
    SRC_DIRS 
        "." 
        "system" 
        "drivers" 
        "comm" 
        "model" 
        "algo" 
        "ui" 
        "utils"

    # Báo cho ESP-IDF biết các thư mục chứa file .h để các file #include tìm thấy nhau
    INCLUDE_DIRS 
        "." 
        "system" 
        "drivers" 
        "comm" 
        "model" 
        "algo" 
        "ui" 
        "utils"
)