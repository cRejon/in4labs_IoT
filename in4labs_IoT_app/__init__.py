import os
from datetime import datetime, timedelta
from pathlib import Path
import subprocess

from flask import Flask, render_template, url_for, jsonify, redirect, flash, send_file, request
from flask_login import LoginManager, UserMixin, login_required, current_user, login_user

from .utils import create_editor, create_navtab


basedir = os.path.abspath(os.path.dirname(__file__))

flask_config = {
    'SECRET_KEY': os.environ.get('SECRET_KEY') or 'super-secret-key',
}

# docker environment variables (for production delete lab_duration and OR statements)
lab_duration = 1
cam_url = os.environ.get('CAM_URL') or 'http://62.204.201.51:8100/Mjpeg/1?authToken=2454ef16-84cf-4184-a748-8bddd993c078'
user_email = os.environ.get('USER_EMAIL') or 'admin@email.com'
end_time = os.environ.get('END_TIME') or (datetime.now() + timedelta(minutes=lab_duration)).strftime('%Y-%m-%dT%H:%M:%S')

boards = {
    'Board_1':{
        'name':'Sensor',
        'model':'Arduino Uno WiFi Rev2',
        'fqbn':'arduino:megaavr:uno2018',
        'serial_number':'02B4ABB7895EBAB2E4A9',
    },
    'Board_2':{
        'name':'TFT',
        'model':'Arduino Uno WiFi Rev2',
        'fqbn':'arduino:megaavr:uno2018',
        'serial_number':'49DE5D448BF406B42B1B',
    },
    'Board_3':{
        'name':'Fan',
        'model':'Arduino Uno Rev3',
        'fqbn':'arduino:megaavr:uno2018',
        'serial_number':'00C8C2FEFC37ABCD660A',
    }
}



login_manager = LoginManager()
# set the default login page
login_manager.login_view = 'login'

app = Flask(__name__, instance_path=os.path.join(basedir, os.pardir, 'arduino'))

app.config.from_mapping(flask_config)


# init login
login_manager.init_app(app)



class User(UserMixin):
    def __init__(self, id, email):
        self.id = id
        self.email = email

user = User(id=1, email=user_email)

@login_manager.user_loader
def load_user(id):
    return user



@app.route('/login', methods=['GET', 'POST'])
def login():
    if current_user.is_authenticated:
        return redirect(url_for('index')) 

    if request.method == 'POST':
        email = request.form['email']
        if email == user_email:
            login_user(user)
            return redirect(url_for('index'))
    return render_template('login.html')


@app.route('/')
@login_required
def index():
    navtabs = []
    editors = []
    for board in boards.keys():
        navtabs.append(create_navtab(board=board, name=boards[board]['name']))
        editors.append(create_editor(board))
    return render_template('index.html', boards=boards, navtabs=navtabs,
                                editors=editors, cam_url=cam_url, end_time=end_time)

@app.route('/get_example', methods=['GET'])
@login_required
def get_example(): 
    board = request.args.get('board')
    example = request.args.get('example')

    if example == 'empty_sketch.ino':
        file_path = os.path.join(app.instance_path, 'examples', 'empty_sketch.ino') 
    else:
        file_path = os.path.join(app.instance_path, 'examples', board, example)

    return send_file(file_path, mimetype='text')

@app.route('/compile', methods=['POST'])
@login_required
def compile():
    board = request.form['board']
    code = request.form['text']

    compilation_path = os.path.join(app.instance_path, 'compilations', board)
    sketch_path = os.path.join(compilation_path, 'temp_sketch')

    with open(os.path.join(sketch_path, 'temp_sketch.ino'), 'w') as f:
        f.write(code)

    command = ['arduino-cli', 'compile', '--fqbn', boards[board]['fqbn'],
    '--build-cache-path', os.path.join(compilation_path, 'cache'), 
    '--build-path', os.path.join(compilation_path, 'build'), 
    sketch_path]

    result = subprocess.run(command, capture_output=True, text=True) 

    resp = jsonify(board=board, error=result.stderr)
    return resp

@app.route('/execute', methods=['POST'])
@login_required
def execute():       
    board = request.form['board']
    target = request.form['target']

    if (target == 'user'): 
        input_file = os.path.join(app.instance_path, 'compilations', board, 'build','temp_sketch.ino.hex')
    else: # target == 'stop'
        input_file = os.path.join(app.instance_path, 'compilations', 'precompiled','stop.hex')

    # Take the last two characters of the serial number as the USB port
    usb_port = boards[board]['serial_number'][-2:] 

    # NOTE: arduino-cli upload command does not work properly with -Pusb flag
    #       so we use avrdude directly instead
    avrdude_path = os.path.join('/', 'root', '.arduino15', 'packages', 'arduino',
                                 'tools', 'avrdude', '6.3.0-arduino17', 'bin', 'avrdude')
    avrdude_conf_path = os.path.join('/', 'root', '.arduino15', 'packages', 'arduino', 
                                     'tools', 'avrdude', '6.3.0-arduino17', 'etc', 'avrdude.conf')
    avrdude_partno = 'atmega4809'
    avrdude_programer_id = 'xplainedmini_updi'
    avrdude_usb_port = '-Pusb:'+ usb_port
    avrdude_baudrate = '115200'
    avrdude_sketch =  '-Uflash:w:'+ input_file +':i'
    avrdude_fuse_2 = '-Ufuse2:w:0x01:m'
    avrdude_fuse_5 = '-Ufuse5:w:0xC9:m'
    avrdude_fuse_8 = '-Ufuse8:w:0x02:m'
    avrdude_boot = os.path.join('/', 'root', '.arduino15', 'packages', 'arduino',
                                 'hardware', 'megaavr', '1.8.8', 'bootloaders', 'atmega4809_uart_bl.hex:i')

    command = [avrdude_path, '-C', avrdude_conf_path, '-V', '-p', avrdude_partno, '-c', avrdude_programer_id, 
               avrdude_usb_port, '-b', avrdude_baudrate, '-e', '-D', avrdude_sketch, avrdude_fuse_2, 
               avrdude_fuse_5, avrdude_fuse_8, avrdude_boot]

    result = subprocess.run(command, capture_output=True, text=True) 
    print(result) # debug info
    resp = jsonify(board=board)
    return resp
