# Embedded-C
Embedded C programs 

Required Parts: 1 DIP switch, Tiva LaunchPad TM4C123, 220Ω Resistors, LEDs, jumpers, breadboard.

Objective: Created a digital lock with 5-binary inputs and 2-binary output. Inputs will be implemented using DIP switches and outputs will be implemented using LEDs (one red and one green).

Procedure: 
Building the lock using LEDs and switches connected to your Tiva LaunchPad.
a. Started by creating a flow chart to plan your algorithm before coding.
b. DIP Switches will be used to provide the digital lock inputs, while LEDs will be used to display the status of the lock, e.g. Locked/Unlocked.
c. Use five input switches to enter the right combination. 
d. One RED LED will display the Locked status. If you enter the wrong combination the LED will toggle twice then turn off. If you enter the right combination the RED LED remains OFF. If the switches are all OFF, the RED led should be off.
e. One GREEN LED will display the unlocked status. If you enter the wrong combination the LED will stay off. If you enter the right combination the GREEN LED will toggle once then turn ON.
g. Press RESET to erase the entered code
h. Used port E PINs for your input and port A PINs for output.
