# Wind and Solar Powered ROV
# Overview
This project involves the design and construction of a wind and solar powered remote-operated vehicle (ROV). The primary goal is to harness renewable energy sources to power the vehicle, enhancing its sustainability and operational efficiency. It is intended to be the first prototype and stencil for future teams at Norwich University to compete in ASME competitions. Key features include a charging circuit for both solar and wind, a supercapacitor bank for quick charging and discharging, and a handheld and onboard controller that uses bluetooth communication.

<img src="Media\Prototype\vehicle_prototype.JPG" alt="sample result" width="500" />
<img src="Media\poster_presentation.jpg" alt="sample result" width="500" />

# Design and Operation
<img src="Media\gifs\Driving.gif" alt="sample result" width="500" />

<img src="Media\gifs\Renbot Assembly.gif" alt="sample result" width="700" />

# Supercapacitor Bank 
We connected 4 supercapacitors in series using the SABM8100288 cicruit board to ensure balanced charging and discharging of the supercapacitors, allowing for up to 10.8 V per bank. A voltmeter is connected to measure the voltage of each supercapacitor bank. The supercapacitor banks can be connected in series or parallel based on system requirements.

<img src="Media\Prototype\supercapacitor_bank_prototype.JPG" alt="sample result" width="500" />
<img src="Media\Circuits\multiple_super_capacitor_bank_circuit.PNG" alt="sample result" width="500" />


# Handheld Controller
Shown below is the handheld controller. We implemented 4 push buttons that are debounced using an RC filter. We use an HM-10 bluetooth module for communication and an Ardiuno Nano for the microcontroller. 

<img src="Media\Prototype\handheld_controller_prototype.JPG" alt="sample result" width="500" />
<img src="Media\Circuits\handheld_controller_circuit.PNG" alt="sample result" width="500" />

# Wind Turbine
We use a 3-phase AC motor as our generator. The circuit below converts the 3 phases to DC to charge the supercapacitors.

<img src="Media\Circuits\windmill_circuit.PNG" alt="sample result" width="500" />
