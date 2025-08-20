# movingeyes
Make eyes move based on sensor input

## Overview

This project uses an ESP32 based microcontroller with a touch screen, a gesture sensor and an Arduino Sketch to move a pair of on-screen eyes based on the gesture made.

## Background

I have a collection of microcontrollers and sensors. I am working my way through the collection of equipment I have and looking at fun projects I can put together, and also expand my knowledge base. 

## What did I use?

Here’s a picture of the components I used for this project:

<img width="50%" height="50%" alt="image" src="https://github.com/user-attachments/assets/0b86b3d1-6eeb-4af2-a430-eb9047f0d228" />

The project uses a microcontroller, a gesture sensor and a cable to connect the two. Here’s a closer look at the gesture sensor:

<img width="50%" height="50%" alt="image" src="https://github.com/user-attachments/assets/6f3cb507-8f20-437e-9f62-3ffe7b0628ff" />

The front of the sensor has some basic information about its pinout. Here’s the back, which has the actual sensor:

<img width="50%" height="50%" alt="image" src="https://github.com/user-attachments/assets/c985ecfc-2031-4aee-a0dd-2f8fd1bf1e1a" />

## What does it look like?

This is what the project looks like when it’s running:

<img width="50%" height="50%" alt="image" src="https://github.com/user-attachments/assets/c72e5d84-b429-405e-8710-6dde7509698c" />

There are three sizes of eyes – small, medium and large (shown). The iris and pupil will follow input from the gesture sensor. The eyes have already moved to look a little to the right. There are a series of buttons, on the bottom of the microcontroller’s screen, that can be used to change the color of the eyes. Here are the large yellow eyes looking up:

<img width="50%" height="50%" alt="image" src="https://github.com/user-attachments/assets/4b6b7ff9-1ec3-4929-a1b7-962fed813327" />

Here are the small size eyes:

<img width="50%" height="50%" alt="image" src="https://github.com/user-attachments/assets/98702609-d457-4459-8558-41db67cf68db" />

Here's a video showing the project working:

[![short video](https://img.youtube.com/vi/DLDeIVW_zk4/0.jpg)](https://www.youtube.com/watch?v=DLDeIVW_zk4)

## Technical Overview

The project uses a Core2 AWS microcontroller (https://shop.m5stack.com/products/m5stack-core2-esp32-iot-development-kit-for-aws-iot-edukit) microcontroller from M5Stack (https://m5stack.com/). M5Stack also had a gesture sensor (https://shop.m5stack.com/products/unit-gesture-recognition-sensor-paj7620u2?srsltid=AfmBOood8i7os-8pYig_AcWU4RKFewEWPErj7F52ppLfspHVCewNGR6y) which comes with the Grove cable. Technical information, and Arduino Sketch samples, for the sensor can be found here https://docs.m5stack.com/en/unit/gesture.
       
The microcontroller is 2 x 2 inches (~50 x 50mm) and has a screen resolution of 320 x 240. In the orientation I use, this becomes 240 x 320:

<img width="50%" height="50%" alt="image" src="https://github.com/user-attachments/assets/86b0f4d9-b4ba-43f8-898d-cc264028d25c" />

The eyes are just a black circle (pupil), drawn on top of a blue circle (iris), drawn on top of of a white circle (eyeball):

<img width="50%" height="50%" alt="image" src="https://github.com/user-attachments/assets/c24ce35d-ce3c-4451-b5ef-f8a999c77e82" />

A sprite is used to hold the entire eye as this helps to prevent screen flicker when updating the position of the iris and pupil. Only one sprite is used as the eyes are symmetrical e.g. they are both the same size and will look in the same direction. The sprite is displayed twice to make the pair of eyes.

Coordinates within the sprite are relative to the sprite, on the full screen:

<img width="50%" height="50%" alt="image" src="https://github.com/user-attachments/assets/82790f33-6448-4e94-9b5f-4fd24536cd48" />

No matter where the sprite is on the screen, internal X and Y coordinates will always be 0..49.

The gesture sensor can track up, down, left, right, rotate clockwise, rotate anticlockwise (counterclockwise), forward and back.

The up, down, left and right gestures make the eyes look in that direction. The rotate gestures make the eyes roll in that direction. The forward and back gestures make the eyes larger or smaller.

The rolling eyes effect is created by calculating a handful of points on an orbital circle, drawn at the center of the eyeball:

<img width="50%" height="50%" alt="image" src="https://github.com/user-attachments/assets/12a6c494-6093-46c2-98db-f11dcb8b7515" />

Coordinates are calculated at 45 degree intervals around this orbit. This gives 6 X and Y coordinates that can be used to redraw the iris and pupil.

## What do I need?

You will need:

1. An M5Stack Core2 AWS microcontroller and gesture sensor. The application only uses the standard
   M5Stack and Core2 libraries. 

2. A PC with Windows, Linux, or a Mac to install the Arduino IDE which can be downloaded
   here https://www.arduino.cc/en/software/.

3. A USB A to USB C cable to connect the PC or Mac to the Core2.

4. The git utility to access the GIT repository 
	(git clone https://github.com/davygotgit/movingeyes.git) or visit 
 	https://github.com/davygotgit/movingeyes and download a ZIP file.

M5Stack have a range of integrated microcontrollers and sensors. I like the Core2 AWS version as it a decent touchscreen, plenty of CPU and memory, and it has three Grove ports. 

If you are not familiar with the Grove interface, it uses 4 pins and can only be connected one way. I find this particularly useful. I am not opposed to soldering connections, but the Grove interfaces make things to much easier.

There are three main connection interfaces on M5Stack sensors – I2C, UART and GPIO. The Core2 AWS can accommodate all three types of sensor, but this is something you need to consider with other M5Stack controllers and sensors. Some controllers only have one Grove interface which might work with a given sensor once initialized correctly with the right pin numbers for the controller’s interface.

Seeed Studio (https://wiki.seeedstudio.com/Grove_Sensor_Intro/) also have a range of Grove sensors. These can also be used with M5Stack microcontrollers, but you have to pay attention to the interface type (I2C, UART or CPIO) and the voltage the sensor uses. Sensors from M5Stack tend to be more plug and play.

## How do I install and configure the tools?

Here are some instructions for downloading and installing GIT https://github.com/git-guides/install-git.

Here are some instructions on how to download and install the Arduino IDE https://docs.arduino.cc/software/ide-v2/tutorials/getting-started/ide-v2-downloading-and-installing/.

M5Stack have an excellent quick start guide here https://docs.m5stack.com/en/arduino/m5core2/program. I don’t recall having to install any driver on my version of Ubuntu 24.04.2 LTS. It’s possible this is already included in the kernel. On Linux you must add your account to the dialout group by running the following bash command:

	sudo usermod -a -G dialout <your_account>

For example, if your user account is fantasticfred:

	sudo usermod -a -G dialout  fantasticfred

You must log out your current session and log back in again for this change to become active.

For Linux systems, I would first see if your Arduino IDE can see the Core2 device before attempting to install any drivers.

You know you are connected to the Core2 if you see something similar to the following status (bottom right) in the Arduino IDE:

<img width="519" height="67" alt="image" src="https://github.com/user-attachments/assets/b886ac19-f629-4a19-a945-6e667d29399a" />

## How do I build and install the application?

You need to download the code from the GIT repository. This can be done by visiting https://github.com/davygotgit/movingeyes and downloading, and then extracting, a ZIP file or by running the following terminal command from bash, a Windows Command Prompt or any suitable GIT access tool:

	git clone https://github.com/davygotgit/movingeyes.git

There are a couple of options to build the application for the first time. Option 1 is:

1. Start the Arduino IDE.
2. Create a new project using the File -> New Sketch menu option.
3. Save the project using the name movingeyes by using the File -> Save menu option.
4. Open the src/movingeyes.ino file, from repository, using another editor, 
	and copy/paste the contents over the skeleton project.
       
Option 2 is:
       
1. Start the Arduino IDE.
2. Create a new project using the File -> New Sketch menu option.
3. Save the project using the name movingeyes by using the File -> Save menu option.
4. Use the Sketch -> Show Sketch Folder menu option to get the location of the 
	project (Sketch location). This will be similar to Home/Arduino/movingeyes on Linux.
5. Close the IDE.
6. Copy the movingeyes.ino file from the src subdirectory of the repository 
	to the Sketch location.
7. Start the Arduino IDE and load the movingeyes project.
       
Once you have the initial project saved, you can just load it from File -> Open Recent menu option.

With the Sketch loaded, connect the Core2 using the USB A to USB C cable,. Ensure the M5Core2 board is selected and the USB port shows a connected status. Press the Upload button on the toolbar. The Sketch will be compiled and transferred to the Core2. The application will start after the transfer completes.
Do you have any tips for using M5Stack microcontrollers and sensors?

This is a tip for using M5Stack sensors. The gesture sensor has technical information and a link to an Ardunio Sketch sample here https://docs.m5stack.com/en/unit/gesture. M5Stack have short, concise code snippets for all their sensors.

I tend to use the sensor, with a microcontroller, with one of these Sketches first. This tells me that the sensor is working as expected and I can review data that it produces.

This can reduce debugging time when you start to add more sensors and your own code. As you know the sensors work independently, any issues have to be with your code or the interaction of multiple sensors.

## Were there any challenges creating this project?

I ran into two challenges with this project.

The first challenge was with the way I initially used the M5Stack sprites. My first design paired the iris and pupil in one sprite which was redrawn on top of the eyeball. This was not a good design due to the sprite being square and artifacts of the sprite could be seen outside the eyeball. There was also a lot of screen flicker with this design. Redrawing the eyeball, iris, and pupil in one sprite stopped the artifacts as everything was now a circle. It also stopped most of the screen flicker.

The other challenge was quite odd. I was unable to set the eye color to yellow. The button for this color would not register a touch event on the screen. I also had trouble getting the other buttons to register touch events, but they did eventually work. I installed another Sketch that I knew worked that had more on-screen buttons, and that Sketch no longer worked as expected. There had been a recent update to the M5Stack libraries and wondered if this had broken something. I was just about to downgrade the library updates when I noticed the screen was quite dirty. I cleaned the screen and the buttons started working as expected. In all the time I have used M5Stack microcontrollers, I have never run into this issue before. The dirt was causing the screen to permanently register a touch event at a certain point on the screen, preventing other touch events.

