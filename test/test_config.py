from tempfile import mkdtemp


class Config(object):
    
    # Flask settings
    ENV = 'development'
    CACHE_TYPE = 'simple'
    CACHE_DEFAULT_TIMEOUT = 600
    SECRET_KEY = 'in4labs-top-secret-key',
    SESSION_TYPE= 'filesystem',
    SESSION_FILE_DIR = mkdtemp(),
    SESSION_COOKIE_NAME = 'in4labs-app-sessionid'
    SESSION_COOKIE_HTTPONLY = True
    SESSION_COOKIE_SECURE = False   # should be True in case of HTTPS usage (production)
    SESSION_COOKIE_SAMESITE = None  # should be 'None' in case of HTTPS usage (production)
    DEBUG_TB_INTERCEPT_REDIRECTS = False

    # Labs settings
    labs_config = {
        'duration': 5, # minutes
        'labs': [{
            'lab_name' : 'in4labs_IoT',
            'html_name' : 'Laboratory of Internet of Things',
            'description' : 'This lab performs IoT experiments on Arduino devices',
            'host_port' : 8001,
            'cam_url': 'http://62.204.201.51:8100/Mjpeg/1?authToken=2454ef16-84cf-4184-a748-8bddd993c078',
        },],
    }