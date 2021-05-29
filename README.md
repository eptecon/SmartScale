SmartScale: Connected Scale for weight measurement
======

WiFi connected scale.


### Hardware

ESP32 Microcontroller, sparkfun's [OpenScale](https://www.sparkfun.com/products/13261).

### Firmware

The firmware consists of RTOS and applications for measuring and analysing of weight data from sparkfun's [OpenScale](https://www.sparkfun.com/products/13261). 
The data from the scale is sent over UART to ESP32. ESP32 is also able to configure the scale. It connects to a cloud service and provides weight data over MQTT.

## Software

Immediately after connecting, the device starts to send the measured weights to the cloud service ([GlueLogics](https://github.com/eptecon/gluelogics)) enabling analytics for your working environment, even without invasive notifications.

---
