# Secplus GDO for ESPHome

Secplus GDO is an ESPHome component for controlling Security+ and Security+ 2.0 garage openers
made by Chamberlain Group, including garage door openers sold since 1997 under the Chamberlain,
LiftMaster, Craftsman and Merlin brands.

#### This component is maintained by Konnected and powers the [Konnected GDO blaQ](https://konnected.io/products/smart-garage-door-opener-blaq-myq-alternative) Konnected's smart garage door opener accessory for Security+ garage door openers.

## Adapted from [ratgdo](https://github.com/ratgdo)

This component was adapted from and inspired by [ratgdo](https://paulwieland.github.io/ratgdo/), however it's not a copy and not feature-equivalent. We initially set out to fork ratgdo and contribute back some improvements, however it ultimately became a complete rewrite. Konnected is publishing this derivative work under the GPLv3 license.

### Project Goals
The ratgdo and [secplus](https://github.com/argilo/secplus) developers found a way to communicate with Security+ garage door openers over a reverse-engineered serial wireline protocol. This novel development enabled owners of Security+ garage openers to read and send commands directly to the garage unit. Konnected set out to incorporate this technology into a finished consumer product. To do that, we wanted to adapt the code to:

* Remove dependency on Arduino
* Run on the ESP-IDF platform
* Utilize the ESP32's hardware UART (instead relying on the SoftwareSerial arduino library)
* Decouple the Security+ wireline protocol from the ESPHome component

### Other Differences from ratgdo

1. *Event-driven instead of polling*. This library uses an event-driven architecture to receive data from the garage door unit, instead of polling for updates. This results in a measurably faster response time to state changes.
1. *Two wires instead of three*. The obstruction sensor state can now be read via the serial protocol, therefore it doesn't need to be wired separately to a GPIO. This makes installation simpler, only requiring the red and white wire to the wall button inputs on the garage door opener.
1. *Supports a pre-closing warning*. To comply with [U.S. 16 CFR 1211.14(f)](https://www.ecfr.gov/current/title-16/part-1211#p-1211.14(f))
1. *Automatic detection of Security+ vs Security+ 2.0*. Less for the end-user to think about.
1. *Removes dry contact trigger support*. We feel that this is better handled with a _template cover_ in ESPHome, and this library should focus on the Security+ interaction only.
1. *Removes relay outputs*. Again, simplifying the library to do Security+ only.

## Dependencies

1. [gdolib](https://github.com/konnected-io/gdolib) (to be published soon)