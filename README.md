# UniProg

UniProg is a true DAC-per-pin universal programmer supporting voltages anywhere from -45&nbsp;V to +45&nbsp;V at roughly 22&nbsp;mV (0.022&nbsp;V) resolution.
It is currently a work-in-progress, and it may never be completed.

## Cost

While the BOM cost is currently unknown, it is estimated to total over US$500.

## Power Supply

UniProg runs from a single +48&nbsp;V power supply.
It then buck converts to multiple rails: -45&nbsp;V, +45&nbsp;, -5&nbsp;V, +5&nbsp;V, -2.5&nbsp;V, and +2.5&nbsp;V.
These rails provide all of the power for UniProg.

## Mainboard operation

UniProg runs on a WDC85C816 16-bit microprocessor at 10&nbsp;MHz.
While the "85C816" supports a 24-bit address space, the upper 8-bits are all set to `0` (this may change).
As for why a WDC65C02S wasn't used: the 65C02 has 8-bit registers while the 85C816 has 16-bit registers.
In addition, on the 65C02, the stack is fixed at a 256-byte region located at `0x01xx`; On the 65C816, the stack can be relocated.

### Memory Map

UniProg reserves 16&nbsp;KiB for RAM, 16&nbsp;KiB for MMIO, and 32&nbsp;KiB for the ROM.

| Binary Address                         | Size | Purpose |
| -------------------------------------- | ---- | ----- |
| `0000 0000 00xx xxxx xxxx xxxx`        |  16k | RAM |
| `0000 0000 01xx xxxx yyyy zzzz`        |  16k | Pin Driver Board MMIO (see below) |
| `0000 0000 1xxx xxxx xxxx xxxx`        |  32k | ROM |
| `0001 0000 0000 0000 0000 0000` and up |      | Unused |

#### Pin Driver Board MMIO

Each pin driver board has 256 bytes of address space allocated to it at the address `0000 01xx xxxx yyyy zzzz` (where `x` is the pin driver board, `y` is the I/O bank selector, and `z` selects the pin).

| `y` bit value | Value |
| ------------- | ----- |
| `0000`        | DAC value (16-bits each) |
| `0001`        | MUX value (8-bits each; LSB selects VPP, MSB selects GND) |
| `0010`        | ADC value (16-bits each) |
| `0011`        | Unused |
| `0100`        | Unused |
| `0101`        | Unused |
| `0110`        | Unused |
| `0111`        | Unused |
| `1000`        | Unused |
| `1001`        | Unused |
| `1010`        | Unused |
| `1011`        | Unused |
| `1100`        | Unused |
| `1101`        | Unused |
| `1110`        | Unused |
| `1111`        | Unused |

As an example, if `x` is `0x00D`, `y` is `0x2`, and `z` is `0xB`, this selects the 13th (zero-based) pin board (`0x00D`), the ADC input (`0x2`), and pin 5's (zero-based) MSB (if `z` was `0xA`, this would select the LSB of pin 5).
Also, if `x` is `0x00D`, `y` is `0x1`, and `z` is `0x3`, this selects the 13-th (zero-based) pin board, the MUXes (`0x1`), and pin 1's (zero-based) VPP (`0x3` [`0x0011`]).

## Pin Driver Board Operation

Each pin of the chips can be individually controlled, and can be in one of three states: (1) floating, (2) connected to ground, or (3) connected to a pin-specific voltage.
Each pin driver contains a DAC, an op-amp, and two MOSFETs to connect the pin to the DAC/op-amp output, or to ground.
Optionally, the two MOSFETs can be turned off and leave the pin floating/not connected.

Each pin driver board contains eight individually addressable pin drivers, two 8-bit latches to control the MOSFET drivers, and 16 MOSFET drivers (one for each MOSFET).

### DAC / Op-Amp

The DACs are powered from plus and minus 5 volts with reference voltages at plus and minus 2.5 volts.
These DACs output a value from -2.048&nbsp;V and +2.048&nbsp;V with milivolt resolution.
The outputs are run through op-amps with a gain of 22 to boost the output up to anywhere from -45&nbsp;V and +45&nbsp;V with 22&nbsp;mV resolution.
The output of the op-amps are then routed to the VPP MOSFET.

### Latches / MOSFET Drivers

In addition to the eight DAC/op-amp combos, each pin driver board contains two 8-bit latches and 16 MOSFET drivers.
One latch controls the VPP MOSFET drivers, while the other controls the GND MOSFET drivers.
When the VPP MOSFET is turned on, the voltage from the DAC/op-amp combo will be routed to the pin.
If the GND MOSFET is turned on instead, the pin on the ICUT (IC under test) will be connected to ground.

As a precaution, each op-amp contains an overcurrent detector and will shutdown if too much current is being drawn from it.
This can happen if the ICUT (IC under test) draws too much through the specific pin, or if, somehow, VPP is shorted to ground.
This overcurrent flag is routed to the CPU and will trigger an interrupt that will halt the operation.

## License

UniProg is licensed under [GNU General Public License 3.0](https://www.gnu.org/licenses/gpl-3.0.en.html) or later.