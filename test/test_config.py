class Config(object):
    # Labs settings
    labs_config = {
        'server_name': 'test_server',
        'mountings': [{
            'id': '1', 
            'duration': 10, # minutes
            'cam_url': 'http://ULR_TO_WEBCAM/Mjpeg',
        },],
        'labs': [{
            'lab_name' : 'in4labs_IoT',
            'html_name' : 'Laboratory of Internet of Things',
            'description' : 'This lab performs IoT experiments on Arduino devices',
            'mounting_id': '1',
            'host_port' : 8001,
        }],
    }