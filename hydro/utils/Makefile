# Makefile

.SUFFIXES: .o .F90

include ../macros

wh_version := $(shell cat ../.version)
WRF_HYDRO_VERSION=\"$(wh_version)\"

nwm_version  := $(shell cat ../.nwm_version)
NWM_VERSION=\"$(nwm_version)\"

F90FLAGS += -DNWM_VERSION=$(NWM_VERSION) -DWRF_HYDRO_VERSION=$(WRF_HYDRO_VERSION)

OBJS = \
	module_version.o \
	module_hydro_stop.o

all:	$(OBJS)

## The insertion of compile-time constants strangely requires the capital F in the extension.
.F90.o:
	@echo "Utils Makefile:"
	$(COMPILER90) $(CPPINVOKE) $(CPPFLAGS) -o $(@) $(F90FLAGS) $(LDFLAGS) $(MODFLAG) -I$(NETCDFINC) $(*).F90
	@echo ""
	ar -r ../lib/libHYDRO.a $(@)
	cp *.mod ../mod

#
# Dependencies:
#

clean:
	rm -f *.o *.mod *.stb *~
