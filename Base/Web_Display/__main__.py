from flask import Flask, render_template
import sqlite3
import time
import logging
import json

app = Flask(__name__)

with open("main_context.json", 'r') as f:
  main_context = json.load(f)

GARDEN_DATABASE_FNAME = "../../data/garden_data.db"

select_last = """SELECT * FROM {table_name}
ORDER BY id DESC LIMIT 1"""

def get_data(con, table_name):
  cur = con.execute(select_last.format(table_name=table_name))
  entry = cur.fetchone()
  cur.close()
  return entry # entry[0] = id, entry[1] = timestamp, entry[2] = data

def get_tables(con):
  cur = con.execute("SELECT name FROM sqlite_master WHERE type='table'")
  tables = cur.fetchall()[1:]
  cur.close()
  return [item[0] for item in tables]


@app.route("/")
def index():
  return render_template("base.html", **main_context)


@app.route("/garden/")
def garden():
  garden_con = sqlite3.connect(GARDEN_DATABASE_FNAME)
  tables = get_tables(garden_con)
  tables = [f"garden/{t}" for t in tables]
  garden_con.close()
  if len(tables) == 0:
    return garden()
  built_context = main_context.copy()
  built_context["table_names"] = tables
  return render_template("device_nav.html", **built_context)

@app.route("/garden/<device_type>/")
def garden_device(device_type=None):
  garden_con = sqlite3.connect(GARDEN_DATABASE_FNAME)
  tables = get_tables(garden_con)
  garden_con.close()
  device_tables = [d for d in tables if device_type in d]
  device_tables = ["../garden"] + device_tables
  if len(device_tables) == 0:
    return garden()
  built_context = main_context.copy()
  built_context["table_names"] = device_tables
  return render_template("device_nav.html", **built_context)

@app.route("/garden/<device_type>/<name>/")
def garden_device_name(device_type=None, name=None):
  garden_con = sqlite3.connect(GARDEN_DATABASE_FNAME)
  entry = None
  if device_type and name:
    table_name = f"[{device_type}/{name}]"
    try:
      garden_con = sqlite3.connect(GARDEN_DATABASE_FNAME)
      entry = get_data(garden_con, table_name)
      garden_con.close()
    except sqlite3.OperationalError:
      pass
  if not entry:
    return garden_device(device_type)
  # TODO: Create a Data Entry Object?
  data_list = [
    {
      "name" : device_type,
      "details":[
        ("Name",name),
        ("Timestamp",entry[1]),
        ("Status",entry[2])
      ]
    }
  ]
  built_context = main_context.copy()
  built_context["data_list"] = data_list
  # return f"Garden, {device_type}/{name}: {entry[2]} at {entry[1]}"
  return render_template("data_display.html", **built_context)


if __name__ == "__main__":
  app.run(debug=True, port=1121)
