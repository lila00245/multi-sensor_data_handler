# multi-sensor data handler

# author: Lila Wolfanger

# creation date: 12/30/2025

# project goal: This project aims to simulate 2-3 different sensors (such as temperature, pressure, and a camera frame counter). Each sensor will update at a different rate, produce raw integer data, and run in its own thread. It will be able to share data safely via mutexes. The sensor values will be packed into a 32-bit integer with certain bit ranges assigned to each sensor. Failure cases will be added in case of out of range sensors, missed update deadlines, and buffer overflows. The system will print timestamps with the packed data in hexadecimal and print error flags. 


# system components: 

* sensor A (temperature)
* sensor B (pressure)
* sensor C (camera frame counter)


# update rates:

* temp: slow (eg every 500 ms)
* pressure: medium (eg every 200 ms)
* camera: fast (eg every 33 ms)


# output:

* combined sensor packet
* printed / logged periodically
* error flags when values are bad

