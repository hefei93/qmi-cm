CC:=$(CROSS-COMPILE)gcc
LD:=$(CROSS-COMPILE)ld

CFLAGS+=-Wall
CFLAGS+=-std=gnu99
CFLAGS+=-O2
CFLAGS+=-Wmissing-declarations
CFLAGS+=-Wmissing-prototypes
CFLAGS+=-Wdeclaration-after-statement
CFLAGS+=-Wstrict-prototypes
CFLAGS+=-fno-strict-aliasing
CFLAGS+=-Wno-deprecated-declarations
CFLAGS+=-Wint-to-pointer-cast
CFLAGS+=-Wfloat-equal
CFLAGS+=-Wno-unused-parameter
CFLAGS+=-Wno-sign-compare
CFLAGS+=-Wunused-but-set-variable
CFLAGS+=-Wundef
CFLAGS+=-Wimplicit-function-declaration
CFLAGS+=-Wpointer-arith
CFLAGS+=-Winit-self
CFLAGS+=-Wshadow
CFLAGS+=-Wmissing-include-dirs
CFLAGS+=-Waggregate-return
CFLAGS+=-Wformat-security
CFLAGS+=-Wtype-limits
# CFLAGS+=-Werror
CFLAGS+=-Wunreachable-code
CFLAGS+=-pipe
CFLAGS+=-fstack-check
CFLAGS+=-Wredundant-decls
CFLAGS+=-Wnested-externs
CFLAGS+=-fstack-protector-all

CPPS+=QmiWwanCM.c
CPPS+=GobiNetCM.c
CPPS+=main.c
CPPS+=MPQMUX.c
CPPS+=QMIThread.c
CPPS+=util.c
CPPS+=udhcpc.c
CPPS+=consts.c

LIBS+=-lpthread
LIBS+=-ldl

release: clean qmi-proxy serial
	$(CC) -DNDEBUG $(CFLAGS) -s $(CPPS) $(LIBS) -o qmi-cm

debug: clean
	$(CC) -DDEBUG $(CFLAGS) -g $(CPPS) $(LIBS) -o qmi-cm

qmi-proxy:
	$(CC) -DNDEBUG $(CFLAGS) -s quectel-qmi-proxy.c util.c $(LIBS) -o qmi-proxy

serial:
	cd qserial && make && cp serial ../

clean:
	rm -rf qmi-cm *~
	rm -rf qmi-proxy
	rm -rf serial
