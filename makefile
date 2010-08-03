all : botlib objs mods jackbot
bot : botlib objs jackbot

botlib :
	if ! test -d bin/lib; then mkdir bin/lib; fi
	$(MAKE) -C dev/src/lib/

objs :
	$(MAKE) -C dev/src/

mods :
	if ! test -d bin/mods; then mkdir bin/mods; fi
	$(MAKE) -C dev/src/mods/

jackbot : 
	$(MAKE) -C bin/
	@echo 'All done... ready to start the JacKBot!'
