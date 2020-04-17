/**
 * Marlin 3D Printer Firmware
 *
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 * SAMD51 HAL developed by Giuliano Zaro (AKA GMagician)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#pragma once

#include "UniProg.hpp"
#include <array>

UNIPROG_INLINE auto get_samd_port(u32 dio_pin) -> i32
{
    i32 internal_pin{pin_to_samd_pin(dio_pin)};
    if (internal_pin == -1)
        return -1;
    return internal_pin >> 5;
}
UNIPROG_INLINE auto get_samd_pin(u32 dio_pin) -> i32
{
    i32 internal_pin{pin_to_samd_pin(dio_pin)};
    if (internal_pin == -1)
        return -1;
    return internal_pin & 0x1fu;
}

UNIPROG_INLINE auto pin_to_eiline(u32 dio_pin) -> i32
{
    return _samd_port_pin_to_eiline(get_samd_port(dio_pin), get_samd_pin(dio_pin));
}

// Get adc/ain associated to specified arduino pin
UNIPROG_INLINE auto pin_to_adc(u32 dio_pin) -> i32
{
    i32 internal_pin{anapin_to_adcain(dio_pin)};
    if (internal_pin == -1)
        return -1;
    return static_cast<i32>(static_cast<u32>(internal_pin) >> 8);
}
UNIPROG_INLINE auto pin_to_ain(u32 dio_pin) -> i32
{
    i32 internal_pin{anapin_to_adcain(dio_pin)};
    if (internal_pin == -1)
        return -1;
    return static_cast<i32>(static_cast<u32>(internal_pin) & 0xFF);
}

// Get external interrupt line
UNIPROG_INLINE auto _samd_port_pin_to_eiline(u32 port, u32 pin) -> i32
{
    if (port == 0 && (pin >= 0 && pin <= 31) &&
        pin != 8 && pin != 26 && pin != 28 && pin != 29)
        return static_cast<i32>(pin & 0xF);

    if (port == 1 && ((pin >= 0 && pin <= 25) || (pin == 30 || pin == 31)))
        return static_cast<i32>(pin & 0xF);
    if (port == 1 && (pin >= 26 && pin <= 29))
        return 12 + static_cast<i32>(port) - 26;

    if (port == 2 && ((pin >= 0 && pin <= 6) || (pin >= 10 && pin <= 31)) && pin != 29)
        return static_cast<i32>(pin & 0xF);
    if (port == 2 && pin == 7)
        return 9;

    if (port == 3 && (pin == 0 || pin == 1))
        return static_cast<i32>(pin);
    if (port == 3 && (pin >= 8 && pin <= 12))
        return 3 + static_cast<i32>(pin) - 8;
    if (port == 3 && (pin == 20 || pin == 21))
        return 10 + static_cast<i32>(pin) - 20;

    // caller must check for `-1` value
    return -1;
}

// Get adc/ain
UNIPROG_INLINE auto anapin_to_adcain(u32 dio_pin) -> u32
{
    return _pin_to_adcain(anapin_to_samd_pin(dio_pin));
}
UNIPROG_INLINE auto _pin_to_adcain(i32 pin) -> i32
{
    return _samd_port_pin_to_adcain(get_samd_port(pin), get_samd_pin(pin));
}

UNIPROG_INLINE auto _samd_port_pin_to_adcain(u32 port, u32 pin) -> i32
{
    if (port == 0 && (pin == 2 || pin == 3))
        return 0x000 + pin - 2;
    if (port == 0 && (pin >= 4 && pin <= 7))
        return 0x000 + pin;
    if (port == 0 && (pin == 8 || pin == 9))
        return 0x100 + 2 + pin - 8;
    if (port == 0 && (pin == 10 || pin == 11))
        return 0x000 + pin;

    if (port == 1 && (pin >= 0 && pin <= 3))
        return 0x000 + 12 + pin;
    if (port == 1 && (pin >= 4 && pin <= 7))
        return 0x100 + 6 + pin - 4;
    if (port == 1 && (pin == 8 || pin == 9))
        return 0x100 + pin - 8;

    if (port == 2 && (pin == 0 || pin == 1))
        return 0x100 + 10 + pin;
    if (port == 2 && (pin == 2 || pin == 3))
        return 0x100 + 4 + pin - 2;
    if (port == 2 && (pin == 30 || pin == 31))
        return 0x100 + 12 + pin - 30;

    if (port == 3 && (pin == 0 || pin == 1))
        return 0x100 + 14 + pin;

    // caller must check for `-1` value
    return -1;
}

#ifndef MASK
#define MASK(PIN) (1 << PIN)
#endif

UNIPROG_INLINE auto read_pin(u32 dio_pin) -> bool
{
    // assumes valid `dio_pin`
    return (PORT->Group[get_samd_port(dio_pin)].IN.reg & MASK(get_samd_pin(dio_pin))) != 0;
}
UNIPROG_INLINE auto write_pin(u32 dio_pin, bool val) -> void
{
    // assumes valid `dio_pin`
    const EPortType port = (EPortType)get_samd_port(dio_pin);
    const u32 mask = MASK(get_samd_pin(dio_pin));

    if (val)
        PORT->Group[port].OUTSET.reg = mask;
    else
        PORT->Group[port].OUTCLR.reg = mask;
}
UNIPROG_INLINE auto toggle_pin(u32 dio_pin) -> void
{
    // assumes valid `dio_pin`
    PORT->Group[get_samd_port(dio_pin)].OUTTGL.reg = MASK(get_samd_pin(dio_pin));
}

UNIPROG_INLINE auto set_input(u32 dio_pin) -> void
{
    // assumes valid `dio_pin`
    const EPortType port = (EPortType)get_samd_port(dio_pin);
    const i32 pin = get_samd_pin(dio_pin);

    PORT->Group[port].PINCFG[pin].reg = (u8)(PORT_PINCFG_INEN);
    PORT->Group[port].DIRCLR.reg = MASK(pin);
}
UNIPROG_INLINE auto set_input_pullup(u32 dio_pin) -> void
{
    // assumes valid `dio_pin`
    const EPortType port = (EPortType)get_samd_port(dio_pin);
    const i32 pin = get_samd_pin(dio_pin);
    const u32 mask = MASK(pin);

    // same as set_input_pullup, but using OUTSET
    PORT->Group[port].PINCFG[pin].reg = (u8)(PORT_PINCFG_INEN | PORT_PINCFG_PULLEN);
    PORT->Group[port].DIRCLR.reg = mask;
    PORT->Group[port].OUTSET.reg = mask;
}
UNIPROG_INLINE auto set_input_pulldown(u32 dio_pin) -> void
{
    // assumes valid `dio_pin`
    const EPortType port = (EPortType)get_samd_port(dio_pin);
    const i32 pin = get_samd_pin(dio_pin);
    const u32 mask = MASK(pin);

    // same as set_input_pullup, but using OUTCLR
    PORT->Group[port].PINCFG[pin].reg = (u8)(PORT_PINCFG_INEN | PORT_PINCFG_PULLEN);
    PORT->Group[port].DIRCLR.reg = mask;
    PORT->Group[port].OUTCLR.reg = mask;
}

UNIPROG_INLINE auto set_output(u32 dio_pin) -> void
{
    // assumes valid `dio_pin`
    const EPortType port = (EPortType)get_samd_port(dio_pin);
    const i32 pin = get_samd_pin(dio_pin);

    PORT->Group[port].DIRSET.reg = MASK(pin);
    PORT->Group[port].PINCFG[pin].reg = 0;
}
UNIPROG_INLINE auto set_output_open_drain(u32 dio_pin) -> void
{
    // assumes valid `dio_pin`
    const EPortType port = (EPortType)get_samd_port(dio_pin);
    const i32 pin = get_samd_pin(dio_pin);

    PORT->Group[port].PINCFG[pin].reg = (u8)(PORT_PINCFG_PULLEN);
    PORT->Group[port].DIRCLR.reg = MASK(pin);
}

UNIPROG_INLINE auto set_pwm(u32 dio_pin) -> void
{
    // assumes valid `dio_pin`
    set_output(dio_pin);
}
UNIPROG_INLINE auto set_pwm_open_drain(u32 dio_pin) -> void
{
    // assumes valid `dio_pin`
    set_output_open_drain(dio_pin);
}

UNIPROG_INLINE auto is_output(u32 dio_pin) -> bool
{
    // assumes valid `dio_pin`
    const EPortType port = (EPortType)get_samd_port(dio_pin);
    const i32 pin = get_samd_pin(dio_pin);

    if ((PORT->Group[port].DIR.reg & MASK(pin)) != 0)
        return true;

    return (PORT->Group[port].PINCFG[pin].reg & (PORT_PINCFG_INEN | PORT_PINCFG_PULLEN)) == PORT_PINCFG_PULLEN;
}
UNIPROG_INLINE auto is_input(u32 dio_pin) -> bool
{
    // assumes valid `dio_pin`
    return !is_output(dio_pin);
}

UNIPROG_INLINE auto is_pwm_pin(u32 dio_pin) -> bool
{
    if (dio_pin >= 2 && dio_pin <= 13)
        return true;
    if (dio_pin == 23 || dio_pin == 24)
        return true;
    if (dio_pin == 44 || dio_pin == 45)
        return true;
    if (dio_pin == 48)
        return true;
    return false;
}

// Return fullfilled ADCx->INPUTCTRL.reg
UNIPROG_INLINE auto pin_to_inputctrl(u32 dio_pin) -> i32
{
    static std::array<u32, 16> pins{
        ADC_INPUTCTRL_MUXPOS_AIN0,
        ADC_INPUTCTRL_MUXPOS_AIN1,
        ADC_INPUTCTRL_MUXPOS_AIN2,
        ADC_INPUTCTRL_MUXPOS_AIN3,
        ADC_INPUTCTRL_MUXPOS_AIN4,
        ADC_INPUTCTRL_MUXPOS_AIN5,
        ADC_INPUTCTRL_MUXPOS_AIN6,
        ADC_INPUTCTRL_MUXPOS_AIN7,
        ADC_INPUTCTRL_MUXPOS_AIN8,
        ADC_INPUTCTRL_MUXPOS_AIN9,
        ADC_INPUTCTRL_MUXPOS_AIN10,
        ADC_INPUTCTRL_MUXPOS_AIN11,
        ADC_INPUTCTRL_MUXPOS_AIN12,
        ADC_INPUTCTRL_MUXPOS_AIN13,
        ADC_INPUTCTRL_MUXPOS_AIN14,
        ADC_INPUTCTRL_MUXPOS_AIN15,
    };

    i32 ain{pin_to_ain(dio_pin)};
    if (ain == -1)
        return -1;
    if (ain >= 15)
        return -1;

    return static_cast<i32>(pins[ain]);

    return 0;
}

UNIPROG_INLINE auto anapin_to_samd_pin(u32 dio_pin) -> i32
{
    if (dio_pin == 0)
        return pin_to_samd_pin(67);
    if (dio_pin == 1)
        return pin_to_samd_pin(68);
    if (dio_pin == 2)
        return pin_to_samd_pin(69);
    if (dio_pin == 3)
        return pin_to_samd_pin(70);
    if (dio_pin == 4)
        return pin_to_samd_pin(71);
    if (dio_pin == 5)
        return pin_to_samd_pin(72);
    if (dio_pin == 6)
        return pin_to_samd_pin(73);
    if (dio_pin == 7)
        return pin_to_samd_pin(74);
    if (dio_pin == 8)
        return pin_to_samd_pin(54);
    if (dio_pin == 9)
        return pin_to_samd_pin(55);
    if (dio_pin == 10)
        return pin_to_samd_pin(56);
    if (dio_pin == 11)
        return pin_to_samd_pin(57);
    if (dio_pin == 12)
        return pin_to_samd_pin(58);
    if (dio_pin == 13)
        return pin_to_samd_pin(59);
    if (dio_pin == 14)
        return pin_to_samd_pin(60);
    if (dio_pin == 15)
        return pin_to_samd_pin(61);
    if (dio_pin == 16)
        return pin_to_samd_pin(12);
    if (dio_pin == 17)
        return pin_to_samd_pin(13);
    if (dio_pin == 18)
        return pin_to_samd_pin(9);

    // caller must check for `-1` value
    return -1;
}

UNIPROG_INLINE auto digital_pin_to_analog_input(u32 dio_pin) -> i32
{
    if (dio_pin >= 67 && dio_pin <= 74)
        return dio_pin - 67;
    if (dio_pin >= 54 && dio_pin <= 61)
        return 8 + dio_pin - 54;
    if (dio_pin == 12 || dio_pin == 13)
        return 16 + dio_pin - 12;
    if (dio_pin == 9)
        return 18;

    // caller must check for `-1` value
    return -1;
}

UNIPROG_INLINE auto pin_to_samd_pin(u32 dio_pin) -> i32
{
    // TODO: https://github.com/adafruit/circuitpython/blob/master/ports/atmel-samd/boards/grandcentral_m4_express/pins.c
    static std::array<i32, 100> pins{
        // 0
        PIN_PB25,
        PIN_PB24,
        PIN_PC18,
        PIN_PC19,
        PIN_PC20,
        PIN_PC21,
        PIN_PD20,
        PIN_PD21,
        PIN_PB18,
        PIN_PB02, // A18
        // 10
        PIN_PB22,
        PIN_PB23,
        PIN_PB00, // A16
        PIN_PB01, // A17
        PIN_PB16,
        PIN_PB17,
        PIN_PC22,
        PIN_PC23,
        PIN_PB12,
        PIN_PB13,
        // 20
        PIN_PB20,
        PIN_PB21,
        PIN_PD12,
        PIN_PA15,
        PIN_PC17,
        PIN_PC16,
        PIN_PA12,
        PIN_PA13,
        PIN_PA14,
        PIN_PB19,
        // 30
        PIN_PA23,
        PIN_PA22,
        PIN_PA21,
        PIN_PA20,
        PIN_PA19,
        PIN_PA18,
        PIN_PA17,
        PIN_PA16,
        PIN_PB15,
        PIN_PB14,
        // 40
        PIN_PC13,
        PIN_PC12,
        PIN_PC15,
        PIN_PC14,
        PIN_PC11,
        PIN_PC10,
        PIN_PC06,
        PIN_PC07,
        PIN_PC04,
        PIN_PC05,
        // 50
        PIN_PD11, // SPI_MISO
        PIN_PD08, // SPI_MOSI
        PIN_PD09, // SPI_SCK
        PIN_PD10, // SPI_SS
        PIN_PB05, // A8
        PIN_PB06, // A9
        PIN_PB07, // A10
        PIN_PB08, // A11
        PIN_PB09, // A12
        PIN_PA04, // A13
        // 60
        PIN_PA06, // A14
        PIN_PA07, // A15
        -1,
        -1,
        -1,
        -1,
        -1,
        PIN_PA02, // A0
        PIN_PA05, // A1
        PIN_PB03, // A2
        // 70
        PIN_PC00, // A3
        PIN_PC01, // A4
        PIN_PC02, // A5
        PIN_PC03, // A6
        PIN_PB04, // A7
        -1,
        -1,
        -1,
        -1,
        -1,
        // 80
        -1,
        -1,
        -1,
        PIN_PB28, // SD_CS
        -1,
        -1,
        -1,
        -1,
        PIN_PC24, // NeoPixel
        -1,
        // 90
        -1,
        -1,
        -1,
        -1,
        -1,
        PIN_PB31, // SD_CD
        -1,
        -1,
        -1,
        -1,
    };

    // caller must check for `-1` value
    return pins[dio_pin];
}
