APPLICATION = coffeebot-batteries

RIOTBASE ?= $(CURDIR)/RIOT
QUIET ?= 1
BOARD ?= particle-xenon
DEVELHELP ?= 1

FEATURES_REQUIRED += periph_gpio
FEATURES_REQUIRED += periph_gpio_irq

USEMODULE += shell
USEMODULE += shell_commands
USEMODULE += ztimer_msec

include $(RIOTBASE)/Makefile.include
