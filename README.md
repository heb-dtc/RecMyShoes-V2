RecMyShoes-V2
=============

Second iteration of the RecMyShoes project, same concept, more gadgets 

Tools:
One USB camera, one USB hard drive, one Raspbery Pi, one Arduino UNO
one LCD screen and one push button

To do what:
The idea here is to start filming when the button is pressed.
The LCD screen start counting the time when the filming starts.
When the button is pressed again, the recording should stop and the video saved on the hard disk.

How:
The push button and LCD screen are controlled by the Arduino.
The camera and the hard drive are connected to the Rpi.

Both devices communicate using Serial communication.
On the Rpi, a pyhton script is always running, listening for input on the Serial port.
When a command is received, it either kick off the camera or stops it.

The all setup has to be plug/unplugged every day, so a CRON job is used for the python script (to have it start when the Rpi starts).

