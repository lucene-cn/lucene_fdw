
EXTENSION    = kafka_fdw
EXTVERSION   = $(shell grep default_version $(EXTENSION).control | sed -e "s/default_version[[:space:]]*=[[:space:]]*'\([^']*\)'/\1/")

DATA 		 = $(filter-out $(EXTENSION)--$(EXTVERSION).sql, $(wildcard *--*.sql)) $(EXTENSION)--$(EXTVERSION).sql
# DOCS         = $(wildcard doc/*.md)
TESTS        = $(wildcard test/sql/*.sql)
REGRESS      ?= $(patsubst test/sql/%.sql,%,$(TESTS))
REGRESS_OPTS = --inputdir=test --load-extension=$(EXTENSION)
EXTRA_CLEAN  = $(EXTENSION)--$(EXTVERSION).sql
MODULE_big   = $(EXTENSION)
OBJS         =  $(patsubst %.c,%.o,$(wildcard src/*.c))
PG_CONFIG   ?= pg_config
PG_CPPFLAGS  = -std=c99 -Wall -Wextra -Wno-unused-parameter

ifndef NOINIT
REGRESS_PREP = prep_kafka
endif

ifdef DEBUG
PG_CPPFLAGS+= -DDO_DEBUG
endif

PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)



PLATFORM 	 = $(shell uname -s)

ifeq ($(PLATFORM),Darwin)
SHLIB_LINK += -lrdkafka -lz -lpthread 
PG_LIBS += -lrdkafka -lz -lpthread 
else
SHLIB_LINK += -lrdkafka -lz -lpthread -lrt 
PG_LIBS += -lrdkafka -lz -lpthread -lrt 
endif

ifdef TEST
REGRESS = $(TEST)
endif


SHLIB_LINK += -ljvm

#export PATH=/usr/local/pgsql/bin:/opt/software/jdk/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/root/bin
#export LD_LIBRARY_PATH=/root/jdk-16.0.1/lib/server/
#cp  /root/jdk-16.0.1/include/*  /usr/local/pgsql/include/server
#cp  /root/jdk-16.0.1/include/linux/*  /usr/local/pgsql/include/server
#cp  /root/jdk-16.0.1/lib/server/*  /usr/local/pgsql/include/server
#ln -s /usr/local/pgsql/lib/pgxs  /usr/pgsql-13/lib/pgxs
# cp  /usr/local/pgsql/lib/kafka_fdw.so /usr/pgsql-13/lib/

all: $(EXTENSION)--$(EXTVERSION).sql

$(EXTENSION)--$(EXTVERSION).sql: sql/$(EXTENSION).sql
	cp $< $@

installcheck: submake $(REGRESS_PREP)
	PGOPTIONS=$(PGOPTIONS) $(pg_regress_installcheck) $(REGRESS_OPTS) $(REGRESS)  

prep_kafka:
	./test/init_kafka.sh

.PHONY:	prep_kafka
