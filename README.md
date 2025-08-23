inputlag 2025
====

This repository contains engineering materials of the inputlag team's self-driven vehicle's model participating in the World Robotics Olympiad Future Engineers competition. The robot was designed and built by a team of three students.

## Content

* `t-photos` contains 2 photos of the team (an official one and one funny photo with all team members)
* `v-photos` contains 6 photos of the vehicle (from every side, from top and bottom)
* `video` contains the video.md file with the link to a video where driving demonstration exists
* `schemes` contains one or several schematic diagrams in form of JPEG, PNG or PDF of the electromechanical components illustrating all the elements (electronic components and motors) used in the vehicle and how they connect to each other.
* `src` contains code of control software for all components which were programmed to participate in the competition
* `models` is for the files for models used by 3D printers, laser cutting machines and CNC machines to produce the vehicle elements. If there is nothing to add to this location, the directory can be removed.
* `other` is for other files which can be used to understand how to prepare the vehicle for the competition. It may include documentation how to connect to a SBC/SBM and upload files there, datasets, hardware specifications, communication protocols descriptions etc. If there is nothing to add to this location, the directory can be removed.

## Introduction

_This part must be filled by participants with the technical clarifications about the code: which modules the code consists of, how they are related to the electromechanical components of the vehicle, and what is the process to build/compile/upload the code to the vehicle’s controllers._


## The Team

### Ansel Kee Jun Xi

### Shahrel Chua Zong Yuan

### Loh Zheng Xiang


## The Challenge 

The **[WRO 2024 Future Engineers - Self-Driving Cars](https://wro-association.org/)** challenge invites teams to design, build, and program a vehicle capable of driving autonomously on a racetrack. The competition includes two main tasks: completing laps while navigating randomized obstacles and successfully performing a precise parallel parking maneuver. Teams must integrate advanced robotics concepts such as computer vision, sensor fusion, and kinematics, focusing on innovation and reliability.

This challenge emphasizes aspects of the engineering process, such as:
- **Mobility Management:** Developing efficient vehicle movement mechanisms and functions.
- **Obstacle Handling:** Strategizing to detect and navigate traffic signs (red and green markers) within specified rules.
- **Documentation:** Showcasing engineering progress, design decisions, and open-source collaboration through a public GitHub repository.

Points are awarded based on performance in the challenge rounds, quality of the engineering documentation, and the ability to create an innovative and robust solution.

Learn more about the challenge [here](https://wro-association.org/wp-content/uploads/WRO-2024-Future-Engineers-Self-Driving-Cars-General-Rules.pdf).


## Photos of our Robot
//Insert photos here

## Video of our Robot 
[Link] //Insert link here


# Mobility Management
The robot's mobility is managed by a combination of the chassis, powertrain and steering. These elements work together to ensure fast and efficient movement of the robot.


## Powertrain
We used 2 NEMA 17 Stepper Motors to act as our main driving force, and to control the speed of the steppers, we utilised a TB6600 Stepper Motor Driver

## Steering
After experimenting with various steering mechanisms, we decided to settle for a simple yet efficient system of front wheel steering. We chose to use a MG996R servo motor as our key component, favouring it for its wide range of mobility and fast response. The servo is connected pointing downwards to a system of two axles, allowing for a full 360 degrees of rotation.

## Chasis
To maximise speed, we decided to use an entirely 3D-printed chassis after discussion. This allowed our robot to become more lightweight and compact as compared to previous iterations.
The chassis makes use of a simple streamlined shape, consisting of an elevated platform on the back to house the arduino, and allow for more room for wiring. The front of the chassis has a hole to accomodate the MG996R servo.












