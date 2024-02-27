
SRCS := source/**

main.exe: $(SRCS)
	clang++ $(SRCS) -std=c++17 -Iinclude -o $(PWD)/build.out

clean:
	rm -f artefacts/build.out