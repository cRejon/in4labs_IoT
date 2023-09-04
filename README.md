In4Labs - Internet of Things remote lab
=====
# Description
Implementation of the IoT remote lab inside a Docker container that will be run by [In4Labs base LTI tool](https://github.com/cRejon/in4labs).   
Tested on Raspberry Pi OS Bullseye (64-bit).  
Requires Python >=3.9.

# Testing
## Setup Raspberry Pi
### Docker installation
Assuming the user is **pi** (default)
```
$ sudo apt update
$ curl -fsSL https://get.docker.com -o get-docker.sh
$ sudo sh get-docker.sh
$ sudo usermod -aG docker pi
$ rm get-docker.sh
```
### Python packages
```
$ pip install docker
```
## Running
Execute the **_test.py_** file inside _test folder_ and go in your browser to the given url.  
The Docker container is created via the Dockerfile <ins>only</ins> the first time this script is run. This will take some time, so please be patient.  
On the login page, enter ```admin@email.com``` as user.

