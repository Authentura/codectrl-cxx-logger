OUT_DIR="build"

all: build

reconfigure: $(OUT_DIR)
	meson --reconfigure $(OUT_DIR)

configure:
	meson $(OUT_DIR)

build: configure
	ninja -C $(OUT_DIR)

test: build
	ninja test -C $(OUT_DIR)

test_view_output: test
	less $(OUT_DIR)/meson-logs/testlog.txt

clean:
	rm -rf $(OUT_DIR)