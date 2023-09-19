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
1. Run the following command to uninstall all conflicting packages:
```
$ for pkg in docker.io docker-doc docker-compose podman-docker containerd runc; do sudo apt-get remove $pkg; done
```
2. Add Docker's official GPG key:
```
$ sudo apt update
$ sudo apt install ca-certificates curl gnupg
$ sudo install -m 0755 -d /etc/apt/keyrings
$ curl -fsSL https://download.docker.com/linux/debian/gpg | sudo gpg --dearmor -o /etc/apt/keyrings/docker.gpg
$ sudo chmod a+r /etc/apt/keyrings/docker.gpg
```
3. Add the repository to Apt sources:
```
$ echo \
  "deb [arch="$(dpkg --print-architecture)" signed-by=/etc/apt/keyrings/docker.gpg] https://download.docker.com/linux/debian \
  "$(. /etc/os-release && echo "$VERSION_CODENAME")" stable" | \
  sudo tee /etc/apt/sources.list.d/docker.list > /dev/null
$ sudo apt update
```
4. Install the Docker packages:
```
$ sudo apt install -y docker-ce docker-ce-cli containerd.io docker-buildx-plugin docker-compose-plugin
```
5. Manage Docker as a non-root user:
``` 
$ sudo groupadd docker
$ sudo usermod -aG docker $USER
$ newgrp docker
```
6. Verify that the installation is successful by running the _hello-world_ image:
```
$ docker run hello-world
```
### Python packages
```
$ pip install docker
```
## Running
Execute the **_test.py_** file inside _test folder_ and go in your browser to the given url.  
The Docker container is created via the Dockerfile <ins>only</ins> the first time this script is run. This will take some time, so please be patient.  
On the login page, enter ```admin@email.com``` as user.

