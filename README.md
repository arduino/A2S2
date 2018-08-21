# A2S2 by Arduino

This program is a proof of concept built from the code created for the
S2A project, a python bridge that helps communicating the offline version
of Scratch 2 (on Windows) with an Arduino board programmed with Firmata Plus

This code has been partly written from scratch partly reused from the
original S2A project. Work has been devoted to clear up the code to make
it easier to contribute. The UI has been left in English only as the
software requires a massive UX revision prior to including things like
multiple languages, adding extra Scratch programs, etc.

## LIMITATIONS

This is Windows-only application. Its guts are made in Python, which means it
can probably be converted to other OSs. It was built with Embarcadero's C++
builder, version 10.2, codename Tokyo.

For this program to run you will need to have installed Scratch 2 previously,
for it to work you will need to install Adobe AIR. You can find step-by-step
instructions on how to do this at: https://scratch.mit.edu/download

## TODO
* improve the process of capturing information from STDOUT and STDERR, a good article on the topic is: https://support.microsoft.com/en-us/help/190351/how-to-spawn-console-processes-with-redirected-standard-handles
* make it multilingual using a *.po file system to allow easy ontributions by the community
* fix the UX flow of the program helping newcomers to install whatever they require for the program to work
* Making an application that runs only one instance at the time: https://www.tek-tips.com/viewthread.cfm?qid=1446136 (1st answer)

## CREDITS / REFERENCES
* Firmata: http://firmata.org/wiki/Main_Page
* Firmata's Github page: https://github.com/firmata/arduino
* ScratchEd page where everything started: http://scratched.gse.harvard.edu/resources/scratch2-and-arduino
* The project that inspired S2A: https://github.com/MrYsLab/s2a_fm
* The French launcher that inspired us: https://github.com/technologiescollege/s2a_fr
* Exporting apps embedding DLLs: https://community.embarcadero.com/forum/programming/941-how-to-create-single-stand-alone-exe-file

## LICENSE

This code is licensed under GPLV3 or later, see license file for more information

**(c) 2018 Arduino LLC**

*Code: D. Cuartielles* / *Graphics: L. Balboa*

Based on code by:
* Alan Yorinks (creator of the S2A python server)
* Sébastien Canet (creator of the first UI for S2A on Windows)
* The AVRdude community

## RELEASES

* 20180821 - first official release, v 0.0.1d, codename: Ankara

### USE IT!

1) download the latest release
2) unzip it anywhere on your computer
3) open the file A2S2_prj.exe (should have installed Scratch 2 before)
4) connect an Arduino Uno to your computer

![A2S2 user interface](https://github.com/arduino/A2S2/raw/master/doc/img/A2S2_UI.PNG "A2S2 user interface")

5) the system will autodetect your board
6) "Fix Board" will install Firmata Plus on your board (the software needed to communicate with Scratch). It will do so by opening a command window and calling AVRDUDE, a program that uploads code to Arduino

![AVRDUDE programming Arduino](https://github.com/arduino/A2S2/raw/master/doc/img/AVRDUDE.PNG "command window programming an Arduino Uno with Firmata Plus")

7) "Launch A2S2" opens the Python server communicating Arduino to Scratch. It does so by opening a command window, do not close it! Also, you should wait until it is done launching the server (10s)

![S2A Python server](https://github.com/arduino/A2S2/raw/master/doc/img/S2A.PNG "S2A Python server in action")

8) "Scratch 2" is the button that will open Scratch including the extension that can communicate to Firmata Plus

![Scratch 2](https://github.com/arduino/A2S2/raw/master/doc/img/Scratch.PNG "Scratch 2 with a blink example running on an Arduino Uno")

