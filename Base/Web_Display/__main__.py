from flask import Flask
import sqlite3
import time
import logging



app = Flask(__name__)

@app.route("/")
def index():
    # Pull basic data setups?
    return "OK"