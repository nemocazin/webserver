SUBDIRS = libs/Reseau/ libs/IPC/ sioux ablette
MAKE = make

all: $(SUBDIRS)

$(SUBDIRS): 
	$(MAKE) -C $@
	
clean: 
	for i in $(SUBDIRS); do \
		$(MAKE) -C $$i clean ; \
	done
	
.PHONY: all $(SUBDIRS)

.PHONY: clean $(SUBDIRS)
	
	
