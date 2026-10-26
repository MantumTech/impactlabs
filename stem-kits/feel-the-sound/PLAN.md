# Feel the Sound — STEM Kit Plan (MVP)

A wearable-sized device that **vibrates when it hears sound — louder sound, stronger buzz**.
Students build it in three lessons and learn how microphones, microcontrollers, transistors,
and motors work, while designing for people who are Deaf or hard of hearing.

Status: **MVP plan**. Not yet prototyped. The parts, values, and code below need a bench test before the kit ships.

---

## 1. Mission (student-facing story)

> Maya is Deaf. At home she can't hear the doorbell, a kettle whistling, or her little brother
> calling her name from the next room. Phones can flash, but not when they're in another room or in her bag.
> Your mission is to build a small device she can clip on or keep in her pocket. It **turns sound into
> vibration** so she can *feel* when something nearby is loud.

Discussion prompts:
- What sounds matter in daily life (alarms, doorbells, voices, traffic)?
- How do Deaf people already handle this (flashing doorbells, vibrating alarm clocks, service dogs)?
- Why might a design that works for a hearing designer still fail for the people who'd use it?
  The answer is to **ask the users**. Where possible, invite a Deaf speaker or show a video made by Deaf creators.

## 2. MVP scope

**In (must work):**
- The mic picks up the sound level in the room.
- The motor's vibration strength follows the loudness: quiet is off, loud is a strong buzz.
- A knob sets the sensitivity, so it works in a quiet bedroom or a noisy classroom.
- It runs on batteries and fits in a small box.

**Out (later versions):**
- Telling sounds apart (doorbell vs. voice) with frequency filtering or ML.
- Bluetooth or phone app.
- Rechargeable LiPo and charging circuit.
- Custom PCB, wristband enclosure.
- A no-code analog version (op-amp plus transistor only) as a simpler "Level 0" kit.

**Why a microcontroller for the MVP:** it makes tuning easy (threshold, smoothing,
max strength in code) and teaches coding. A purely analog circuit is cheaper but much harder
for students to debug.

## 3. How it works

```
 sound → [Electret mic + amp] → analog voltage → [Arduino Nano] → PWM → [NPN transistor] → [coin vibration motor]
                                                      ↑
                                         [10k potentiometer = sensitivity]
```

1. The mic module outputs an audio waveform centered around about 2.5 V.
2. The Nano samples it for 50 ms and measures **peak-to-peak**, which is the loudness.
3. It subtracts a noise threshold set by the knob, then maps the result to PWM 0–max.
4. PWM switches a transistor, which drives the motor. The motor can't run straight from a pin because it needs about 80 mA, and a pin is safe at about 20 mA.
5. A diode across the motor absorbs the voltage spike when the motor switches off.

## 4. Bill of materials (per kit)

| Qty | Part | Notes | Est. cost |
|---|---|---|---|
| 1 | Arduino Nano (CH340 clone) + USB cable | Pre-soldered headers | $4.00 |
| 1 | Electret mic module with amplifier (MAX4466 or MAX9814) | Analog out. **Avoid KY-038**: its analog output is too weak for this | $2.00 |
| 1 | Coin vibration motor, 3 V (10 mm, ~80 mA) | With leads | $0.60 |
| 1 | NPN transistor 2N2222 / PN2222 | | $0.05 |
| 1 | 1 kΩ resistor | Base resistor | $0.02 |
| 1 | 1N4148 (or 1N4001) diode | Flyback protection | $0.03 |
| 1 | 10 kΩ potentiometer | Sensitivity knob | $0.30 |
| 1 | Mini breadboard (400 tie) | | $1.50 |
| ~12 | Jumper wires M-M | | $0.60 |
| 1 | USB power bank (or 4×AA holder with switch) | See power note | $0–3 |
| 1 | Small project box / 3D-printed case | Optional for MVP | $1.00 |
| | **Total** | | **≈ $11–13** |

**Power note:** use the **USB cable plus any power bank** for the MVP. It is the simplest and safest choice, and most students have one. 4×AA (6 V) into VIN is marginal on clone Nanos, so test it before choosing it. Do not use the 9 V battery from the wind kit, because it drains fast with a motor.

**Motor voltage:** the 3 V motor runs from the 5 V rail, but the code caps PWM at about 60% so
it never sees more than about 3 V on average. Verify the motor's datasheet rating at bench test.

## 5. Wiring (MVP)

| From | To |
|---|---|
| Mic VCC / GND / OUT | Nano 5V / GND / **A0** |
| Pot outer pins / wiper | Nano 5V, GND / **A1** |
| Nano **D9** (PWM) | 1 kΩ → transistor **base** |
| Transistor **emitter** | GND |
| Transistor **collector** | Motor (−) lead |
| Motor (+) lead | 5V |
| Diode | Across the motor: **stripe (cathode) to 5V**, other end to collector |

(Kit pictures to be added, matching the wind kit's `images/kit pictures/` style.)

## 6. Lessons

**Lesson 1: Make it buzz (no code).**
- Build: motor + transistor + diode, with a jumper from 5V through 1 kΩ to the base as a "switch."
- Learn: current, why a transistor is a switch/amplifier, why the diode exists, and motor vs. generator (link back to the wind kit).
- Check: the motor buzzes when the base jumper is connected and stops when it's removed.

**Lesson 2: See the sound.**
- Build: mic to A0. Upload `feel_the_sound.ino` (the motor isn't wired yet, so it only prints loudness), then open the **Serial Plotter**.
- Learn: analog vs. digital, sampling, what loudness looks like (clap, talk, silence).
- Check: students record their own numbers for silence, talking, clapping, and a phone alarm in a table.

**Lesson 3: Feel the sound (full device).**
- Build: add the pot and D9 wiring. Upload `feel_the_sound.ino`.
- Learn: thresholds, mapping one range to another (`map()`), PWM, smoothing.
- Check: quiet gives no buzz, talking gives a light buzz, and a clap or alarm gives a strong buzz. The knob shifts the point where buzzing starts.

**Design challenge (stretch):** improve it for a real user. Ideas include pulse patterns instead of a steady buzz, a wrist strap, an LED for "it heard something," or ignoring constant noise like a fan by adapting the threshold.

## 7. Firmware (MVP sketch)

`firmware/feel_the_sound/feel_the_sound.ino`. Not yet compiled or bench-tested.

## 8. Safety rules

- Unplug power before changing wires.
- Check the diode direction (the stripe goes to 5V). If it's backwards, it shorts the supply when the transistor turns on.
- A transistor or motor that gets hot means something is wired wrong. Unplug it and check.
- Keep small parts away from mouths, because they are a choking hazard.
- **This is a learning project, not a safety device.** Don't rely on it for smoke or fire alarms. Real
  alert systems for Deaf people are certified products.

## 9. Vocabulary

Microphone, analog signal, sampling, amplitude, peak-to-peak, threshold, PWM (pulse-width
modulation), transistor, flyback diode, haptics, accessibility, assistive technology, user-centered design.

## 10. Standards alignment (draft)

- NGSS **MS-PS4-1/2** (wave properties, amplitude ↔ loudness).
- NGSS **MS-ETS1-1…4** (define the problem for a user, design, test, iterate).
- CSTA **2-CS-02/03** (combine hardware and software, troubleshoot).
- Target: grades 6–9, about 3 × 45-minute sessions.

## 11. MVP build plan

| Step | Task | Done when |
|---|---|---|
| 1 | Order 3 sets of parts (spares for mistakes) | Parts in hand |
| 2 | Bench prototype on breadboard; confirm the mic module choice, motor current, and PWM cap | Device responds to clap and voice; nothing gets hot |
| 3 | Tune defaults (window, threshold range, smoothing) | Works in a quiet room and a noisy room using only the knob |
| 4 | Test 4×AA vs. power bank | Pick one power option for the BOM |
| 5 | Photograph each build step | Images for the guide |
| 6 | Write `index.html` guide (same layout as the wind kit) and add it to the site index | Page live on GitHub Pages |
| 7 | Pilot with 2–3 students; if possible, get feedback from a Deaf or hard-of-hearing user | Notes and fixes in this file |
| 8 | Finalize the BOM and per-kit cost; package it | Kit v1 |

## 12. Open questions

- Nano clone vs. a cheaper ATtiny85 board (Digispark)? The Nano is easier for students, while the ATtiny is cheaper and smaller.
- MAX4466 (fixed gain, trim pot) vs. MAX9814 (auto gain)? Auto gain can hide loudness differences, so test both.
- Should the kit include a 3D-printed wrist case or leave that as the design challenge?
