# File Name: receive_adc.py
# Assignment: Assignment 4
# Completed by: Isabelle Gaudet, Dylan Wenaas, David Anez
# Development Date: 2024-11-03

import time
import serial
import pandas as pd
import matplotlib.pyplot as plt

ser=serial.Serial('COM3',9600)
time.sleep(2) # wait 2 secs for cmmunication to be established

time=[] #list to store timr
adc=[]  #list to store adc value
voltage=[] #list to store corresponding voltage
time_start=time.time()


# this assumes that the arduino is sending data in the format "time,adc,voltage" will have to change this if the format is different
while time.time()-time_start<10: #read for 10s
    data=ser.readline().decode('utf-8').strip()
    data=data.split(',')   #split the data into time-adc-voltage
    
    time.append(float(data[0])) # add time value to list
    adc.append(float(data[1]))  # add adc value to list
    voltage.append(float(data[2])) # add voltage value to list
    time.sleep(0.1) # read every 0.1s
    
ser.close() # close serial port

data={
    'time':time,
    'adc':adc,
    'voltage':voltage
}
df=pd.DataFrame(data) # create a dataframe from the data dictionary

df.to_csv('adc_data.csv',index=False) # save the data to a csv file 

#adc vs time plot (x-time,y-adc)
plt.figure()
plt.plot(df['time'],df['adc'])
plt.xlabel('Time(s)')
plt.ylabel('ADC Value')
plt.title('ADC vs. Time')
plt.grid(True)  #add grid lines to read value better, can be removed
plt.show()

#voltage vs time plot (x-time,y-voltage)
plt.figure()
plt.plot(df['time'],df['voltage'])
plt.xlabel('Time(s)')
plt.ylabel('Voltage(V)')
plt.title('Voltage vs. Time')
plt.grid(True)  #add grid lines to read value better, can be removed
plt.show()
