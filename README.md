# Cat Doorbell

A proximity sensor to detect my cat outside my door and flash an LED at my desk.


## Summary

I generally have noise cancelling headphones on and my cat will sit outside my door
meowing late at night when it wants to come in. It's probably annoying to my parents
and I think it breaks my cat's heart when it can't come in.

This project has two modules, a transmitter and receiver.
The transmitter module will detect my cat using a proximity sensor and send an RF
signal to the receiver module. The receiver module will activate an LED for three seconds
upon receiving an RF signal.

On the receiver module there is an optional piezo speaker that can be muted/unmuted to 
trigger at the same time as the LED. I feel like it might get annoying hence the mute button.

## Part List

- 2x Arduino Micro Pro (ATMEGA32U4)
- Ultrasonic Module HC-SR04
- HC-SR501 PIR Infared Sensor

## Notes

- Get serial port - ```ls /dev/tty*```

## References

- [Platformio Arduino Leonardo](https://docs.platformio.org/en/latest/boards/atmelavr/leonardo.html)
- [HC-SR04 Ultrasonic Sensor - Last Minute Engineers](https://lastminuteengineers.com/arduino-sr04-ultrasonic-sensor-tutorial/)
- [433 MHz RF Modules](https://www.youtube.com/watch?v=b5C9SPVlU4U)

