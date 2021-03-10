Linux			:
			  make -f Makefile.Linux

all			:
			  @echo "make Linux || make WIN32"

WIN32			:
			  make -f Makefile.WIN32

clean			:
			  make -f Makefile.Linux clean

junkclean		:
			  rm -rf $(wildcard *~ Web/*~ Web/css/*~ Web/js/*~ $(PLATFORM)/*~ Bin/*~ core)

stagebin		: baysim
			  cp baysim Bin 
			  strip Bin/baysim
			  cd Bin
			  rm -rf Bin/Web
			  cp -r Web Bin
			  cp slcan_add Bin
			  cp slcan_remove Bin
			  cp slcan_show Bin
			  cp start.sh Bin
			  cp createBayConfig.sh Bin

cleanbin		:
			  rm -rf Bin/slcan_add Bin/slcan_remove Bin/slcan_show
			  rm -rf Bin/*.sh
			  rm -rf Bin/Web
			  rm -rf Bin/baysim
