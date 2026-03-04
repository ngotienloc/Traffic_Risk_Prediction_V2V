idf_component_register(
    SRC_DIRS 
        "." 
        "system" 
        "drivers" 
        "comm" 
        "model" 
        "algo" 
        "ui" 
        "utils"

    INCLUDE_DIRS 
        "." 
        "system" 
        "drivers" 
        "comm" 
        "model" 
        "algo" 
        "ui" 
        "utils"

    REQUIRES
        esp_system
        esp_wifi
        esp_event
        driver
        nvs_flash
)