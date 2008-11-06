all : botlib objs mods jackbot
bot : botlib objs jackbot

tar :
	@tar cf jackbot_`date +%d%m%Y`.tar `find ./ -type f \( -name "*.c" -or -name "*.h" -or -name "makefile" \)` COPYRIGHT
	@echo "JacKBot backed up!"

botlib :
	if ! test -d dev/lib; then mkdir dev/lib; fi
	$(MAKE) -C dev/src/lib/

objs :
	$(MAKE) -C dev/src/

mods :
	if ! test -d bin/mods; then mkdir bin/mods; fi
	$(MAKE) -C dev/src/mods/

jackbot : 
	$(MAKE) -C bin/
	@echo 'All done... ready to start the JacKBot!'
