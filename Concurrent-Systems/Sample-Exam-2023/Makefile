QUESTION := Question-1 Question-2 

.PHONY: all $(QUESTION)

all: $(QUESTION)

$(QUESTION):
	if [ -d $(CURDIR)/$@ ]; then \
		$(MAKE) -C $(CURDIR)/$@ all && \
		$(MAKE) -C $(CURDIR)/$@ clean || \
		exit 1; \
	fi \
