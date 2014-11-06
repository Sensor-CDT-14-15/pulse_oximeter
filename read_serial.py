#!/usr/bin/python

import serial

redFile = open('red.dat', 'w')
irFile = open('ir.dat', 'w')
photodiodeFile = open('photodiode.dat', 'w')
logLine = "";

headerLine = "Time\tReading\n"
redFile.write(headerLine)
irFile.write(headerLine)
photodiodeFile.write(headerLine)

f = serial.Serial('/dev/tty.usbmodem1411', 57600)

try:
    while True:
      line = f.readline()
      line = line.strip()
      print(line)
      try:
        [time,object,reading] = line.split(" ")
      except ValueError:
        # We've caught slightly less than a full line of serial output, so skip it
        continue
      logLine = time + "\t" + reading
      if (object == "RED"):
        logFile = redFile
      elif (object == "IR"):
        logFile = irFile
      elif (object == "PHOTODIODE"):
        logFile = photodiodeFile
      else:
        continue
      logFile.write(logLine + "\n")
except KeyboardInterrupt:
  pass

f.close()
redFile.close()
irFile.close()
photodiodeFile.close()
