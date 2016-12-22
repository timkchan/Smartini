# Smartini, the bartender of the future
![](demo.png?raw=true)

### 1. Overview of Smartini
The purpose of our research and the making of the Smartini Cocktail Robot was to create an interactive and learning-capable robot that address the challenge of human-robot interaction and modern cocktail making through state-of-the-art technology. We explored different modes of communication, including eye-contact, gestures and speech. We included an entertainment system in our Smartini, composed of jokes and news telling. Smartini can also learn customer trends on the taste of the drinks and adjust its recipes accordingly.

### 2. Drink List
* Smartini
* Sunrise
* Tropical
* Passion
* Classic

### 3. Files

Files in this project (not all are shown):

* `README.md`:          	This file
* `app/`:                 	Subdirectory holding ???:
    * `arduino`:            ???
    * `brainModule`:        ???

* `module/`:                Subdirectory holding ???:
    * `arduino`:           	???
    * `brainModule`:        ???

* `serialCommunication/`:   Subdirectory holding arduino codes:
    * `serialCommunication.ino`:	Codes to run the arduino.


### 4. Running Smartini with iCub simulator

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




### 7. Demo video
[vdo]

[vdo]: <https://youtu.be/k7Cp1eb-CAM>