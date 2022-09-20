from flask import Flask
import json
from datetime import datetime

from GardenDataHandler import Analyzer


app = Flask(__name__)

@app.route('/')
def hello_world():
    return 'Hello, World!'

@app.route('/current')
def current():
    return json.dumps(app.a.current())

@app.route('/current/<device_name>')
def current_device(device_name):
    return json.dumps(app.a.get_current_state(device_name))

@app.route('/history/<device_name>/<int:days>')
def history(device_name=None, days=7):
    try:
        result = json.dumps(app.a.get_history(device_name, days))
    except Exception as e:
        result = str(e)
    return result

@app.route('/on_time/<device_name>/<date>')
def on_time(device_name=None, date=None):
    try:
        date = datetime.strptime(date, '%Y-%m-%d')
        result = json.dumps(app.a.get_on_time(device_name, date))
    except Exception as e:
        result = str(e)
    return result

@app.route('/calendar.ics')
def calendar():
    with open('data/calendar.ics', 'r') as f:
        return f.read()

if __name__ == '__main__':
    app.a = Analyzer()
    app.run(debug=True)