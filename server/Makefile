LIGHTTPD_VERSION_ID = (1<<16|4<<8|35) # 1.4.35

MOD_NAME = mod_rc_web.so

SOURCES = $(wildcard src/*.c)

OBJECTS = $(patsubst src/%.c,build/%.o,$(SOURCES))

CFLAGS = -Wall -g -fPIC -I lighttpd-1.4.35/src \
         -D HAVE_SOCKLEN_T -D LIGHTTPD_VERSION_ID="$(LIGHTTPD_VERSION_ID)"

LDFLAGS = 

.PHONY: all clean

OUT_DIRS = $(BUILD_DIR)

all: $(MOD_NAME)

$(MOD_NAME): $(OBJECTS)
	ld -shared $(LDFLAGS) $< -o $@

build/%.o: src/%.c
	mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f mod_rc_web.so $(OBJECTS)
	rm -rf build

