# Arduino Robot Controller

An educational Arduino-based robot project combining motor control, sensor readings, Bluetooth serial communication, and animated LED-matrix eyes.

> **Project status:** Hardware and software details are still being documented. Confirm each sketch against the actual wiring before powering the robot.

## Planned features

- **Drive control:** DC motors controlled through an L298N motor driver.
- **Bluetooth control:** HC-05 serial link for commands and status messages.
- **Sensor monitoring:** Pulse-sensor readings and temperature sensing.
- **Animated eyes:** LED-matrix displays for simple robot expressions.
- **Split-controller design:** Two Arduino boards can be used to separate movement/sensor tasks from display or interface tasks.

These are project goals based on the current build notes; exact pin assignments and working status should be recorded with the matching sketches.

## Repository layout

- `sketches/` — Arduino IDE sketches, organized by controller or test.
- `docs/` — wiring notes, pin maps, setup, and troubleshooting.
- `hardware/` — optional parts list and circuit references.

## Getting started

1. Open the relevant `.ino` sketch in the Arduino IDE.
2. Select the correct board and COM port.
3. Install any libraries named at the top of that sketch.
4. Check the sketch's pin map against your physical wiring.
5. Test one subsystem at a time with the wheels lifted off the ground.
6. Connect motor power only after checking polarity, driver wiring, and common ground.

## Safety notes

- Never connect motor power directly to an Arduino I/O pin.
- Use a suitable motor driver and a power supply rated for the motors.
- Keep motor power separate from the Arduino's regulated logic supply where appropriate, and connect grounds as required by the driver circuit.
- Do not connect mains voltage to this project.
- Verify sensor voltage and signal levels before connecting them to an Arduino.
- Disconnect power before changing wiring.

## Documentation

See [the documentation folder](docs/README.md) for the current documentation plan and [the sketches folder](sketches/README.md) for sketch organization.

## Contributions

Keep each sketch focused on one controller or subsystem. Include the board type, required libraries, pin map, serial commands, and test status in its header or a nearby README.

## License

No license has been added yet. Add a license only after deciding how you want others to reuse this project.
