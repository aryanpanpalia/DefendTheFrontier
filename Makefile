GITBINARY := git
PINGURL := google.com
LIBRARYREPO := simulator_libraries

ifeq ($(OS),Windows_NT)	
	SHELL := CMD
endif

all: update
ifeq ($(OS),Windows_NT)	
	@cd $(LIBRARYREPO) && mingw32-make
else
	@cd $(LIBRARYREPO) && make
endif

update:
ifeq ($(OS),Windows_NT)	
# check for internet connection
# if there's internet, check to see if Libraries folder exists
# if it does, remove it before cloning the repo
	@ping -n 1 -w 1000 $(PINGURL) > NUL & \
	if errorlevel 1 \
	( \
		( echo Warning: No internet connection! ) \
	) \
	else \
	( \
		( if exist "$(LIBRARYREPO)" \
		( \
			cd $(LIBRARYREPO) && \
			$(GITBINARY) stash && \
			$(GITBINARY) pull && \
			cd .. \
		) \
		else \
		( \
			$(GITBINARY) config --global http.sslVerify false  && \
			$(GITBINARY) clone https://code.osu.edu/fehelectronics/proteus_software/$(LIBRARYREPO).git \
		) \
		) \
	) 
else
# Mac/Linux
	@ping -c 1 -W 1000 $(PINGURL) > /dev/null ; \
	if [ "$$?" -ne 0 ]; then \
		echo Warning: No internet connection!; \
	else \
		if [ -d "$(LIBRARYREPO)" ]; then \
			cd $(LIBRARYREPO) ; \
			$(GITBINARY) stash ; \
      		$(GITBINARY) pull ; \
      		cd .. ; \
		else \
      		$(GITBINARY) clone https://code.osu.edu/fehelectronics/proteus_software/$(LIBRARYREPO).git ; \
		fi \
	fi \

endif

clean:
ifeq ($(OS),Windows_NT)	
	@cd $(LIBRARYREPO) && mingw32-make clean
else
	@cd $(LIBRARYREPO) && make clean
endif