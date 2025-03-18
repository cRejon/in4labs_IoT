# Boards configuration
boards_config = {
    'Board_1':{
        'name':'Sensor',
        'role':'Slave',
        'model':'Arduino Nano ESP32', 
        'fqbn':'arduino:esp32:nano_nora', 
        'usb_port':'1',
    },
    'Board_2':{
        'name':'LCD',
        'role':'Master',
        'model':'Arduino Nano ESP32',
        'fqbn':'arduino:esp32:nano_nora',
        'usb_port':'2',
    },
    'Board_3':{
        'name':'Fan',
        'role':'Slave', 
        'model':'Arduino Nano ESP32',
        'fqbn':'arduino:esp32:nano_nora',
        'usb_port':'3',
    }
}