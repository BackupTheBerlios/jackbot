DEBUG=0

all : botlib objs mods jackbot
bot : botlib objs jackbot

botlib :
	if ! test -d bin/lib; then mkdir bin/lib; fi
	$(MAKE) DEBUG=$(DEBUG) -C src/lib/

objs :
	$(MAKE) DEBUG=$(DEBUG) -C src/

mods :
	if ! test -d bin/mods; then mkdir bin/mods; fi
	if ! test -d bin/mods/logs; then mkdir bin/mods/logs; fi
	$(MAKE) DEBUG=$(DEBUG) -C src/mods/

jackbot : 
	$(MAKE) DEBUG=$(DEBUG) -C bin/
	@echo 'All done... ready to start the JacKBot!'
