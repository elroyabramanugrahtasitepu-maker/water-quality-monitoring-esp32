# ESP32 Water Quality Monitoring System

An IoT-based water quality monitoring system developed using ESP32 and water quality sensors to monitor water conditions in real time.

## Overview

This project is designed to monitor water quality using sensors connected to an ESP32 microcontroller.

The system collects sensor readings, processes the data, and provides water quality information through a monitoring interface.

## Features

- Real-time water quality monitoring
- TDS measurement
- Water temperature monitoring
- Sensor data processing
- ESP32-based IoT system
- WiFi connectivity
- REST API communication
- Database data storage
- Web-based monitoring dashboard

## Hardware

- ESP32
- TDS Sensor
- DS18B20 Temperature Sensor
- OLED Display
- Breadboard
- Jumper wires

## Technologies

- C/C++
- Arduino IDE
- ESP32
- PHP
- MySQL
- REST API
- IoT

## System Architecture

```text
TDS Sensor
     |
     v
DS18B20 Temperature Sensor
     |
     v
    ESP32
     |
     | WiFi
     v
  REST API
     |
     v
   MySQL
     |
     v
Monitoring Dashboard
