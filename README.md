#### WebServer configuration for basi Domotic system on RaspberryPi

├── kernelLamp                 -> C files to create driver to control Reles by GPIO
│   ├── kernellamp.c
│   ├── kernellamp.mod.c
│   ├── Makefile
│   ├── modules.order
│   ├── Module.symvers
│   └── pwmekrnel.c
├── kernelpwm                 -> C files to create driver for simulated PWM by GPIO.
│   ├── kernelpwm.c
│   ├── kernelpwm.mod.c
│   ├── Makefile
│   ├── modules.order
│   └── Module.symvers
└── webserver                 -> Configuration files to launch the Apache web server
    ├── apache2.conf
    ├── lampctrlsv
        └── pythonctrl            -> Python scripts for webapp controll
            ├── controller.py
            ├── __init__.py
            ├── modules
                ├── __init__.py
                └── lamp.py
            

