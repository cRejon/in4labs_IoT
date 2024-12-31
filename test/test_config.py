class Config(object):
    # Labs settings
    labs_config = {
        'duration': 15, # minutes
        'labs': [{
            'lab_name' : 'in4labs_IoT',
            'html_name' : 'Laboratory of Internet of Things',
            'description' : 'This lab performs IoT experiments on Arduino devices',
            'host_port' : 8001,
            'cam_url': 'http://ULR_TO_WEBCAM/Mjpeg',
            'extra_containers': [],
        }],
    }