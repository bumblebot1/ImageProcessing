FACE=face
DART=dartboard
VIOLATEST=overallViola
DETECTORTEST=overallDetector

CC=g++
CFLAGS= -std=c++11
LIBS=/usr/local/lib/libopencv_core.so /usr/local/lib/libopencv_highgui.so /usr/local/lib/libopencv_imgproc.so /usr/local/lib/libopencv_objdetect.so

all: $(FACE) $(DART) $(VIOLATEST) $(DETECTORTEST)

$(FACE): $(FACE).cpp
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

$(DART): $(DART).cpp
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

$(VIOLATEST): $(VIOLATEST).cpp
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

$(DETECTORTEST): $(DETECTORTEST).cpp
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

clean:
	rm -f $(FACE) $(DART) $(VIOLATEST)
