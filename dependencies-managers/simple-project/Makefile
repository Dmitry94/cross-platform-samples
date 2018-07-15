# Makefile for project

SUBDIRS = library application

.PHONY: subdirs $(SUBDIRS) clean

subdirs: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

application: library

clean:
	find ../build \! -name "build.sh" -delete
