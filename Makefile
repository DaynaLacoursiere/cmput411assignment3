
OS := $(shell uname -s)

FLAGS = -I/usr/include -I/usr/include/eigen3 -I/opt/local/include/eigen3 -L/usr/lib -L/usr/lib/nvidia-340 -lglut -lGLEW -lGL -lGLU -lX11 -Wno-write-strings -std=c++11


# utilities

attachGenerator: attachGenerator.cpp mesh.* skeleton.* bvh_util.* attachment.*
	g++ attachGenerator.cpp mesh.cpp skeleton.cpp bvh_util.cpp attachment.cpp -o attachGenerator $(FLAGS)


# viewers

modelViewer: modelViewer.cpp camera.h mesh.*
	g++ modelViewer.cpp mesh.cpp -o modelViewer $(FLAGS)

motionViewer: motionViewer.cpp camera.h timer.h skeleton.* motion.* bvh_util.*
	g++ motionViewer.cpp skeleton.cpp motion.cpp bvh_util.cpp -o motionViewer $(FLAGS)

attachmentViewer: attachmentViewer.cpp camera.h mesh.* skeleton.* bvh_util.* attachment.*
	g++ attachmentViewer.cpp mesh.cpp skeleton.cpp bvh_util.cpp attachment.cpp -o attachmentViewer $(FLAGS)

# viewers you need to implement

modelSolidViewer: modelSolidViewer.cpp camera.h light.h mesh.* texture.*
	g++ modelSolidViewer.cpp mesh.cpp texture.cpp -o modelSolidViewer $(FLAGS)

personViewer: personViewer.cpp light.h camera.h timer.h mesh.* texture.* skeleton.* motion.* bvh_util.* attachment.* animation.*
	g++ personViewer.cpp mesh.cpp texture.cpp skeleton.cpp motion.cpp bvh_util.cpp attachment.cpp animation.cpp -o personViewer $(FLAGS)


%: %.cpp
	g++ $*.cpp -o $* $(FLAGS)


clean:
	rm *Viewer *Generator
