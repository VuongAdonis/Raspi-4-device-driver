# Basic step for a GPIO Driver on Raspberry pi 4

1. Request the GPIO pin from the kernel GPIO subsystem, ensuring it is valid and available.

2. Configure the GPIO direction (input or output).

3. Set the GPIO pin value if output (high/low).

4. Optionally, configure interrupts on GPIO input pin edges.

5. Release the GPIO pin when the driver is unloaded.

