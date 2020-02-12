build:
	g++ //FILE// -o bin -lglew -lglfw3 -framework Cocoa -framework OpenGL -framework IOKit -framework CoreFoundation -framework CoreVideo
run:build
	./bin