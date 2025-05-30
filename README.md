# Resonance of Motion: Walking into Memory
This project is an audiovisual installation, exploring the experience of memory through interaction with wearable sensor technology.

**Link to Video Demonstration:**  https://www.youtube.com/watch?v=JcOfrH7WlD8 
**Link to Media for Download:**  https://www.dropbox.com/scl/fo/a214bfu32qa460qlfjnqw/AIHV8u_RQzc402XXZJpSjB4?rlkey=34aqtpsrnn48nddtwv9bgnz7g&st=mhp8f91c&dl=0 

## How It's Made:

**Tech used:** ESP32-C3-Mini-1, C++, Max/MSP

This project uses ESP-NOW peer to peer communication between two Esp32-C3-Mini-1 devices to read and communicate motion and proximity data. That data is received and unpacked in Max, where it is then mapped to control various audiovisual features. The audiovisual system is made up of personal video footage from childhood, spoken samples clipped from this video footage, and original piano compositions. These elements are processed to create an immersive experience and a sensory exploration of memory.


## Instructions

Make sure that all code and supporting media (available for download at the link above) is located in the same folder before running start.maxpat.

When the patch is opened, most gain sliders start at zero - there are no loadbang objects to set the values. This is because the patch was developed for an interactive installation, with data mapped to control the gain values for each element. To hear all of the elements, manually set the volume levels for the video audio, piano sample, and multichannel piano playback.

The patches used in the final iteration of this project are as follows:
start.maxpat - contains the entire installation system.
allsensorvalues.maxpat - unpacks the sensor data feeding in via serial port.
videoarchive.maxpat - stores all video clips and the markov chain that controls transitions.
audioarchive.maxpat - stores all audio clips and the markov chain that controls transitions.

Other patches / files:

final sewable case design.stl - the final custom-designed housing for the wearable, battery-powered ESP32-C3-Mini-1
nonsewncase.stl - a non-sewable version of the design, used to house the receiver device.
markovmemory.maxpat - used to test the distribution of videos triggered by probabilities
gesturalsystemiteration1.maxpat - the first ever iteration of this project, a patch created  to explore motion interaction with audio elements.
It is possible to explore these max patches without receiving data from a sensor, simply play around with the number objects used to display sensor values to within the system! 
