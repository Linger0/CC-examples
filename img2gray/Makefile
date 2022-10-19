CVINCS := -I/usr/local/include/opencv4
CVLIBS := -lopencv_core -lopencv_imgproc -lopencv_highgui \
	-lopencv_imgcodecs -lopencv_objdetect -lopencv_videoio -lopencv_video -lopencv_calib3d \
	-lopencv_stitching -lopencv_xfeatures2d -lopencv_features2d -lopencv_ximgproc

CXX := g++
CXXFLAGS := -Wall
INCS :=
LIBS :=

ifneq ($(MAKECMDGOALS), clean)
ifndef F
$(error specify a target name with variable "F")
endif
endif

main: $(F)
	$(CXX) $(CXXFLAGS) $(INCS) -o $@ $< $(LIBS)

clean:
	rm main

.PHONY: clean