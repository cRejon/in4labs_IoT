In4Labs - Internet of Things remote lab
=====
# Description
Implementation of the IoT remote lab inside a Docker container that will be run by [In4Labs base LTI tool](https://github.com/cRejon/in4labs).   
Tested on Raspberry Pi OS Lite Bullseye (64-bit).  
Requires Python >=3.9.

# Installation
Connect each Arduino board to the Raspberry Pi according to boards configuration.
```
# Boards configuration
boards = {
    'Board_1':{
        'name':'Sensor',
        'model':'Arduino Uno WiFi Rev2',
        'fqbn':'arduino:megaavr:uno2018',
        'usb_port':'1-1.1',
    },
    'Board_2':{
        'name':'TFT',
        'model':'Arduino Uno WiFi Rev2',
        'fqbn':'arduino:megaavr:uno2018',
        'usb_port':'1-1.2',
    },
    'Board_3':{
        'name':'Fan',
        'model':'Arduino Uno WiFi Rev2',
        'fqbn':'arduino:megaavr:uno2018',
        'usb_port':'1-1.3',
    }
}
```
If you look at the USB hub from the front, the port numbering is as follows.

                _______    _______ 
               | _____ |  | _____ | 
        3-->   ||_____||  ||_____||  <-- 1
               | _____ |  | _____ | 
        4-->   ||_____||  ||_____||  <-- 2
               |_______|__|_______|

# Testing
## Setup Raspberry Pi
### Docker installation
```
$ sudo apt update
$ curl -fsSL https://get.docker.com -o get-docker.sh
$ sudo sh get-docker.sh
$ rm get-docker.sh
```
Run the Docker daemon as a non-root user ([Rootless mode](https://docs.docker.com/engine/security/rootless/)).
```
$ sudo apt-get install -y uidmap fuse-overlayfs
$ sudo systemctl disable --now docker.service docker.socket
$ /usr/bin/dockerd-rootless-setuptool.sh install
$ export PATH=/usr/bin:$PATH
$ export DOCKER_HOST=unix:///run/user/1000/docker.sock
```
To launch the daemon on system startup, enable the systemd service and lingering:
```
$ systemctl --user enable docker
$ sudo loginctl enable-linger $(whoami)
```
### Python packages
```
$ pip install docker
```
## Running
Execute the **_test.py_** file inside _test folder_ and go in your browser to the given url.  
The Docker container is created via the Dockerfile <ins>only</ins> the first time this script is run. This will take some time, so please be patient.  
On the login page, enter ```admin@email.com``` as user.

