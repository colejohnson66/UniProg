# UniProg

UniProg is a true DAC-per-pin universal programmer supporting voltages anywhere from -45&nbsp;V to +45&nbsp;V at roughly 22&nbsp;mV (0.022&nbsp;V) resolution.
It is currently a **_work-in-progress_**, and it may never be completed.

## Cost

While the BOM cost is currently unknown, it is estimated to total over US\$500.

## Power Supply

UniProg runs from a single +48&nbsp;V power supply.
It then buck converts to multiple rails: -45&nbsp;V, +45&nbsp;, -5&nbsp;V, +5&nbsp;V, -2.5&nbsp;V, and +2.5&nbsp;V.
These rails provide all of the power for UniProg.

## Operation

UniProg runs on an [Adafruit Grand Central M4 Express](https://www.adafruit.com/product/4064) which features an ATSAMD51P20A ARM processor running at 120&nbsp;MHz.
The "Grand Central" interfaces with the rest of the motherboard through a 16-bit data bus connected on pins 22 through 37.
Other pins select what the motherboard should do; Such as setting a DAC's value, reading an ADC's value, etc.

In addition, there are a few pins used for interrupts.
One of which is used for overcurrent detection and will shut down programming if triggered.

### Pin Driver Board Operation

Each pin of the ICUT (IC under test) can be individually controlled, and can be in one of three states: (1) floating, (2) connected to ground, or (3) connected to a pin-specific voltage.
Each pin driver contains a DAC, an op-amp, and two MOSFETs to connect the pin to the DAC/op-amp output, or to ground.
Optionally, the two MOSFETs can be turned off and leave the pin floating/not connected.

Each pin driver board contains eight individually addressable pin drivers, two 8-bit latches to control the MOSFET drivers, and 16 MOSFET drivers (one for each MOSFET).

#### DAC / Op-Amp

The DACs are powered from plus and minus 5 volts with reference voltages at plus and minus 2.5 volts.
These DACs output a value from -2.048&nbsp;V and +2.048&nbsp;V with millivolt resolution.
The outputs are run through op-amps with a gain of 22 to boost the output up to anywhere from -45&nbsp;V and +45&nbsp;V with 22&nbsp;mV resolution.
The output of the op-amps are then routed to the VPP MOSFET.

#### Latches / MOSFET Drivers

In addition to the eight DAC/op-amp combos, each pin driver board contains two 8-bit latches and 16 MOSFET drivers.
One latch controls the VPP MOSFET drivers, while the other controls the GND MOSFET drivers.
When the VPP MOSFET is turned on, the voltage from the DAC/op-amp combo will be routed to the pin.
If the GND MOSFET is turned on instead, the pin on the ICUT (IC under test) will be connected to ground.

As a precaution, each op-amp contains an overcurrent detector and will shutdown if too much current is being drawn from it.
This can happen if the ICUT (IC under test) draws too much through the specific pin, or if, somehow, VPP is shorted to ground.
This overcurrent flag is routed to the CPU and will trigger an interrupt that will halt the operation.

## Hardware Used

UniProg uses the following "main" components (this is subject to change):

- WDC65C816S - "65816" - Main CPU (running at 10&nbsp;MHz)
- DAC7614U - DAC supporting positive and negative supply rail operation and output
- OPA454 - High voltage op-amp (100&nbsp;V delta) supporting high current (100&nbsp;mA) output

## License

UniProg, and UniProg's companion software, upro, are licensed under the [GNU General Public License 3.0](https://www.gnu.org/licenses/gpl-3.0.en.html) or later.

UniProg's KiCad schematics use portions from [arduino-kicad-lib](https://github.com/Alarm-Siren/arduino-kicad-library/tree/307d55d7057d2f4e7d28a63ea696091f076a815f) by [Alarm-Siren](https://github.com/Alarm-Siren).
arduino-kicad-lib is licensed under the [GNU General Public License 2.1](https://www.gnu.org/licenses/old-licenses/lgpl-2.1.en.html) which is compatible with the GNU GPL 3.0+ used for the project.
