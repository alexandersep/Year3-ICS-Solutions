# 2013 has no Makefile, ignoring that 
YEARS := 2008 2009 2010 2011 2012 2014 2015 2016 2017 2018 
PARTS := part-b part-c

.PHONY: all $(YEARS)

all: $(YEARS)

$(YEARS):
	for part in $(PARTS); do \
		if [ -d $(CURDIR)/$@-code/$$part ]; then \
			$(MAKE) -C $(CURDIR)/$@-code/$$part all && \
			$(MAKE) -C $(CURDIR)/$@-code/$$part clean || \
			exit 1; \
		fi \
	done
