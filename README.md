# Smartini, the bartender of the future
![](figures/setup.jpg?raw=true)
![](figures/demo.png?raw=true)

### 1. Overview of Smartini
The purpose of our research and the making of the Smartini Cocktail Robot was to create an interactive and learning-capable robot that address the challenge of human-robot interaction and modern cocktail making through state-of-the-art technology. We explored different modes of communication, including eye-contact, gestures and speech. We included an entertainment system in our Smartini, composed of jokes and news telling. Smartini can also learn customer trends on the taste of the drinks and adjust its recipes accordingly.

### 2. Drink List
* Smartini
* Sunrise
* Tropical
* Passion
* Classic

### 3. Files

The main code is composed of two parts: c++ code and .ini parameters file. The first goes into modules folder, the second inside app folder. As the name itself suggests, in modules folder it is possible to write the Modules one may want. Modules are particular classes made of 5 compulsory methods:
* Configure method, launched only the first time the modules is created. Here all the 'rpc' (Remote Procedure Call) ports should be instantiated and double checked.
* Respond method, used to launch some actions(functions written in the same class) when an rpc command(correctly formatted), is received
* UpdateModule method,  used  to run some code with a certain period. This whole code is execuited, and then, after a PeriodTime, is it restarted again.
* getPeriod, used to retrieve the period time 
* interruptModule, method to be used when a interrupt signal is sent to the the modules
* closeModule, method to close the modules.


You might need to add any other function as necessary.

The `.ini` (`app/conf/module_name.ini or app/conf/client.ini`)  files are loaded at runtime, so, once c++ code is supposed to be correct, it is possible to change  parameters directly in it and to perform a refined tuning. 


Files in this project (not all are shown):

* `README.md`:          	This file
* `main.tex`:          		Latex file of the final report
* `finalReport.pdf`:        Rendered pdf of the final report
* `app/`:                 	Subdirectory holding ini parameters files:
    * `arduino`:            Parameters files for Arduino Module
    * `brainModule`:        Parameters files for brainModule

* `module/`:                Subdirectory holding c++ source code:
    * `arduino`:           	Source code for Arduino Module
    * `brainModule`:        Source code for brainModule

* `serialCommunication/`:   Subdirectory holding Arduino codes:
    * `serialCommunication.ino`:	Codes to be run on the arduino.

* `findings/`:  			Subdirectory holding raw data collected from customers:
    * `findings.pdf`:		Rendered table view of all results
    * `questionnaire.pdf`:	Questionnaire used to get the findings

* `figures/`:  				Subdirectory holding figures used in the report:
    * `BlockDiagram.pdf`:	Modular view of the ststem
    * `feedback.pdf`:		Flowchart of the Feedback State
    * `pumpCircuit.png`:	The circuit used to control the pumps
    * `setup.jpg`:			Picture showing the physical setup
    * `StateMachine.png`:	Illustrates the state of the system
    * `Survey.pdf`:			Questionnaire used to gather user findings


### 4. Dependency
Notice that the project is built on `wysiwyd`. The files listed above should be run inside `wysiwyd` environment.
Please refer to: https://github.com/robotology/wysiwyd

### 5. Running Smartini with iCub simulator

Initiate Yarp network:

```sh
$ yarpserver --write
```

Check if Yarp server is running
```sh
$ yarpdetect
```

Initiate iCub simulator:
```sh
$ iCub_SIM
```

Initiate all necessary modules:

Cartesian server:
```sh
$ yarprobotinterface --context simCartesianControl
```

Cartesian solver for left and right arms:
```sh
$ iKinCartesianSolver --context simCartesianControl --part left_arm
$ iKinCartesianSolver --context simCartesianControl --part right_arm
```

Gaze control:
```sh
$ iKinGazeCtrl --from configSim.ini
```

wholeBodyDynamics (dependency of ARE):
```
$ wholeBodyDynamics --robot icubSim --autoconnect --dummy_ft --no_legs
```

ActionsRenderingEngine:
```sh
$ actionsRenderingEngine --from sim/config.ini
```

Object Properties Collector (dependency of ARE):
```sh
$ objectsPropertiesCollector --name OPC --no-load-db --no-save-db
```

iSpeak module:
```sh
$ iSpeak
```

Speech Recognition module:
```sh
$ speechRecog
```

Arduino Module
```sh
$ arduino
```

Brain Module
```sh
$ brainModule
```

### 6. Demo video
[View Video]
[Download Video]

### 7. Contribution
This is a 4th yr class (EE4.60 HCR) project at Imperial College London supervised by prof. Yiannis Demiris

Team members:
* Chantal Tognetti - ct2416@ic.ac.uk
* Emanuele Gentili - eg2214@ic.ac.uk
* Fabian Duffhauss - fad16@ic.ac.uk
* Filippo Baldini - fb1713@ic.ac.uk
* Tim K. Chan - timchan@berkeley.edu

Special thanks:
* Maxime Petit - m.petit@imperial.ac.uk

[View Video]: <https://youtu.be/k7Cp1eb-CAM>
[Download Video]: <https://drive.google.com/open?id=0BzXoKHuf6nEuUG9vV1BDRzg3eXM>
