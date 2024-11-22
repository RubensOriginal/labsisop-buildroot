BUILDROOT_DIR := ../..
KDIR := $(BUILDROOT_DIR)/output/build/linux-custom
COMPILER := $(BUILDROOT_DIR)/output/host/bin/i686-buildroot-linux-gnu-gcc

all:
	# gcc sched_profiler.c -o sched_profiler -lpthread
	$(COMPILER) sched_profiler.c -o sched_profiler -lpthread
	cp sched_profiler $(BUILDROOT_DIR)/output/target/bin

clean:
	rm sched_profiler
	rm $(BUILDROOT_DIR)/output/target/bin/sched_profiler