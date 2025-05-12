# Regeln fuer die bedingte Uebersetzung von vollstaendigen Dateien
# ----------------------------------------------------------------
#
# Variablen:
# SRC := Alle Uebersetzungseinheiten, sprich C/C++-Dateien (*.c und *.cc)
# ASPECTSRC := Alle Aspekt-Dateien (*.ah)
#
# Die Namen aller KConfig-Merkmale sind hier mit dem Prefix CONFIG_ versehen.
#

# Konfiguration von zwei Aspekten zur Debugging-Unterstuetzung
ifneq ($(CONFIG_DEBUG_PANIC),y)
  ASPECTSRC := $(filter-out %/show_panic.ah,$(ASPECTSRC))
endif

ifneq ($(CONFIG_DEBUG_GUARD),y)
  ASPECTSRC := $(filter-out %/locker_checking.ah,$(ASPECTSRC))
endif

# Konfiguration von Aspekten für verschiedene Hardware-Versionen
ifneq ($(CONFIG_BLACK_PCB),y)
  ASPECTSRC := $(filter-out %/black_pcb.ah,$(ASPECTSRC))
endif

ifneq ($(CONFIG_WHITE_PCB),y)
  ASPECTSRC := $(filter-out %/white_pcb.ah,$(ASPECTSRC))
endif

ifneq ($(CONFIG_ANY_PCB),y)
  ASPECTSRC := $(filter-out %/any_pcb.ah,$(ASPECTSRC))
endif
