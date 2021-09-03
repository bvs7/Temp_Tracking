from flask import Flask, render_template
import sqlite3
import time
import logging

app = Flask(__name__)

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
  return "OK"

@app.route("/garden")
@app.route("/garden/<device_type>")
@app.route("/garden/<device_type>/<name>")
def garden(device_type=None, name=None):
  garden_con = sqlite3.connect(GARDEN_DATABASE_FNAME)
  entry = None
  if device_type and name:
    table_name = f"{device_type}_{name}".replace("-","")
    try:
      entry = get_data(garden_con, table_name)
    except sqlite3.OperationalError:
      pass
  
  if entry:
    garden_con.close()
    return f"Garden, {device_type}/{name}: {entry[2]} at {entry[1]}"
  else:
    tables = get_tables(garden_con)
    garden_con.close()
    return f"Garden: {', '.join(tables)}"


if __name__ == "__main__":
  app.run(debug=True, port=1121)
