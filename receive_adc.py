# File Name: receive_adc.py
# Assignment: Assignment 4
# Completed by: Isabelle Gaudet, Dylan Wenaas, David Anez
# Development Date: 2024-11-03

import time
import serial
import pandas as pd
import matplotlib.pyplot as plt

ser = serial.Serial(port="COM3", baudrate=4800, bytesize=8, timeout=2, stopbits=serial.STOPBITS_ONE)

times=[] #list to store time
adc=[]  #list to store adc value
voltage=[] #list to store corresponding voltage
time_start=time.time()
print("Start time:", time_start)

# Burn one line
ser.readline()

# Read and record the next 10 inputs
while (time.time() - time_start < 10):  # record data for 1 sec
    data = ser.readline()
    hex = data.decode('utf-8').split()[1] 
    
    adc_value = int(hex, 16)  # convert hex to int
    adc.append(adc_value)  # add adc value to list
    
    voltage_value = round((adc_value / 1023.0) * 3.0, 2)  # convert adc value to voltage and round to 2 decimals
    voltage.append(voltage_value)  # add voltage value to list

    read_time = round(time.time() - time_start, 2)

    print(read_time, adc_value, voltage_value)

    times.append(read_time)  # add time value to list
    time.sleep(0.1)  # read every 0.1s

ser.close() # close serial port

data={
    'times':times,
    'adc':adc,
    'voltage':voltage
}
df=pd.DataFrame(data) # create a dataframe from the data dictionary

df.to_csv('adc_data.csv',index=False) # save the data to a csv file 

#adc vs time plot (x-time,y-adc)
plt.figure()
plt.plot(df['times'],df['adc'])
plt.xlabel('Time(s)')
plt.ylabel('ADC Value')
plt.title('ADC vs. Time')
plt.grid(True)  #add grid lines to read value better, can be removed
plt.show()

#voltage vs time plot (x-time,y-voltage)
plt.figure()
plt.plot(df['times'],df['voltage'])
plt.xlabel('Time(s)')
plt.ylabel('Voltage(V)')
plt.title('Voltage vs. Time')
plt.grid(True)  #add grid lines to read value better, can be removed
plt.show()
