from GardenDataHandler import Listener, Analyzer

from datetime import datetime

a = Analyzer()
print(a.get_current_state('beds-1/P0'))
print(a.get_current_state('beds-1/P1'))

print(a.get_on_time('beds-1/P0', datetime(2022, 9, 16, 0, 0, 0)))