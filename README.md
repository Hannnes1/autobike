# Autobike project

> **_NOTE:_** This readme is a work in progress.

This repository contains the code that is beeing used within the Autobike project on Chalmers. The main part of the code is made with the red bike in mind,
but it should work with other versions of the bike as well even if some minor adaptions might be necessary.

Below is a list of the important files and folders within the repository. Folders not mentioned in the list below is used for testing during development,
and will likely be removed in the future.

* ### `labview`

The LabVIEW code that controls the bike. The main file in this project is the one called `MainTest.vi`.

* ### `balancing`

This folder contains C code to controll the steering motor in a way that prevents the bike from falling over. This code does not consider the bikes
path at the moment.

* ### `vesc_encoder`

C code to calculate UART commands that should be sent to the VESC, based the input. Functions exist for both RPM and current control.

* ### `Ni Linux Real-time Template`

A template that can be used as a base for creating `.so` files for the MyRIO. The `.vscode` folder is specific to Visual Studio Code, and enables
compilation and linking without the use of a terminal.

* ### `Python_backup_20220402`

Snapshot of the python code that is used to control the black bike. Some parts of this code has been translated into C, and can be found in the various
other folders. The majority of the code is not relevant for the red bike, however, since it has the same functionality as the LabVIEW code.