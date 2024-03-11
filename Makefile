CC:=gcc

OBJDIR:=bin

FLIP:=$(OBJDIR)/flip.o

DMPTTL:=dumptitle
ROMUTL:=romutil
SMPUTL:=sampleutil
UROM:=unrom

CFLAGS:= -O2 -s -Wall -Wextra -Wpedantic -ansi
LDFLAGS:=-lsndfile

build: DUMPTITLE ROMUTIL SAMPLEUTIL UNROM

$(OBJDIR)/%.o: %.c
	$(CC) $< $(CFLAGS) -c -o $@

$(DMPTTL): $(OBJDIR)/$(DMPTTL).o $(FLIP)
	$(CC) -o $@ $^

$(ROMUTL): $(OBJDIR)/$(ROMUTL).o $(FLIP)
	$(CC) -o $@ $^

$(SMPUTL): $(OBJDIR)/readlist.o $(FLIP)
	$(CC) -o $@ $^ $(LDFLAGS)

$(UROM): $(OBJDIR)/$(UROM).o
	$(CC) -o $@ $^ $(LDFLAGS)

.PHONY: clean make_bin DUMPTITLE ROMUTIL SAMPLEUTIL UNROM

DUMPTITLE: make_bin $(DMPTTL)
ROMUTIL: make_bin $(ROMUTL)
UNROM: make_bin $(UROM)
SAMPLEUTIL: make_bin $(SMPUTL)

make_bin:
ifeq ($(wildcard $(OBJDIR)/.*),)
	@mkdir $(OBJDIR)
endif

clean:
	@rm -f $(OBJDIR)/*.o *~ dumptitle romutil sampleutil unrom
ifneq ($(wildcard bin/.*),)
	@rmdir bin
endif
