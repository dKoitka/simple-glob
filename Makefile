
SRCS := source/main.cpp source/globUnitTest.cpp source/globFileSystemTest.cpp source/glob.cpp

main.exe: $(SRCS)
	clang++ $(SRCS) -std=c++17 -Iinclude -o artefacts/build.out

clean:
	rm -f artefacts/build.out