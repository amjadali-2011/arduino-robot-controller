# Arduino sketches

Place each Arduino IDE sketch in its own folder. Arduino IDE expects the main `.ino` file to have the same name as its folder.

Suggested organization:

- `controller_1/` — first board's sketch and a short README.
- `controller_2/` — second board's sketch and a short README.
- `tests/` — isolated sensor, motor-driver, Bluetooth, and display tests.

Before uploading a sketch, verify its pin assignments and library requirements against the hardware. Keep tested sketches separate from experimental versions, and note the board and test status at the top of each file.
