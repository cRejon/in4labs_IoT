import os
import subprocess
import threading
import time
from datetime import datetime, timedelta
import atexit

import docker

from test_config import Config


# Class to stop the container when the time is up
class StopContainerTask(threading.Thread):
     def __init__(self, container, end_time):
         super(StopContainerTask, self).__init__()
         self.container = container
         self.end_time = end_time
 
     def run(self):
        remaining_secs = (end_time - datetime.now()).total_seconds()
        time.sleep(remaining_secs - 2) # Minus 2 seconds for safety
        self.container.stop()
        print('Container stopped.')


# Import lab config from Config object
lab_duration = Config.labs_config['duration']
lab = Config.labs_config['labs'][0]
lab_name = lab['lab_name']
lab_port = lab['host_port']

# Create docker image if not exists
client = docker.from_env()
image_name = f'{lab_name.lower()}:latest'
try:
    client.images.get(image_name)
    print(f'Docker image {image_name} found.')
except docker.errors.ImageNotFound:
    print(f'Creating Docker image {image_name}. Be patient, this will take a while...')
    basedir = os.path.abspath(os.path.dirname(__file__))
    dockerfile_path = os.path.join(basedir, os.pardir)
    image, build_logs = client.images.build(
        path=dockerfile_path,
        tag=image_name,
        rm=True,
    )
    for log in build_logs: # Print the build logs for debugging purposes
        print(log.get('stream', '').strip())

# Docker environment variables
end_time = datetime.now() + timedelta(minutes=lab_duration)
docker_env = {
    'USER_EMAIL': 'admin@email.com',
    'USER_ID': 1,
    'END_TIME': end_time.strftime('%Y-%m-%dT%H:%M:%S'),
    'CAM_URL': lab.get('cam_url', ''),
}

# Run the container
container = client.containers.run(image_name, 
                                  detach=True, 
                                  privileged=True, 
                                  remove=True, 
                                  tty=True,
                                  volumes={'/dev/bus/usb': {'bind': '/dev/bus/usb', 'mode': 'rw'}},
                                  ports={'8000/tcp': ('0.0.0.0', lab_port)}, 
                                  environment=docker_env)

stop_container = StopContainerTask(container, end_time)
stop_container.start()

# Get the Raspberry pi IP address
hostname = subprocess.check_output(['hostname', '-I']).decode("utf-8").split()[0]
container_url = f'http://{hostname}:{lab_port}'
print(f'The container is running at {container_url} during {lab_duration} minutes.')

# Stop the container when the program exits by pressing Ctrl+C
def exit_handler():
    container.stop()

atexit.register(exit_handler)



