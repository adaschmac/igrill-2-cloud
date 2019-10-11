# igrill-2-cloud

### Note: this code does not work. Particle's BLE stack at the time of writing (1.4.1-rc.1) does not support the level of security required by the iGrill.

I have a Weber iGrill2 to keep an eye on the temperatures in my smoker. Sadly, it's *just* too far to connect to while I'm indoors.

The goal here is to configure a Particle Boron to communicate with the iGrill and forward temperature readings up to a web service running in the cloud.

Preferably, I don't want to use Particle's cloud to save myself the $3 per month, as most months this won't be used at all. I have a Google Fi data only SIM that should work.