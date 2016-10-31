EXE=face

CC=g++
LIBS=/usr/local/lib/libopencv_core.so /usr/local/lib/libopencv_highgui.so /usr/local/lib/libopencv_imgproc.so /usr/local/lib/libopencv_objdetect.so

all: $(EXE)

$(EXE): $(EXE).cpp
	$(CC) $^ $(LIBS) -o $@

clean:
	rm -f $(EXE)
