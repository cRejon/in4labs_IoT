class Config(object):
    # Labs settings
    labs_config = {
        'server_name': 'test_server',
        'mountings': [{
            'id': '1', 
            'duration': 10, # minutes
            'cam_url': 'https://ULR_TO_WEBCAM/stream.m3u8',
        },],
        'labs': [{
            'lab_name' : 'in4labs_IoT',
            'html_name' : 'Laboratory of Internet of Things',
            'description' : 'This lab performs IoT experiments on Arduino devices',
            'mounting_id': '1',
            'host_port' : 8001,
        }],
    }