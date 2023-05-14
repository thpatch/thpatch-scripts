CXX=i686-w64-mingw32-g++
CXXFLAGS=-O2 -Wall -Wextra
CPPFLAGS=-DUNICODE=1 -D_UNICODE=1 -D__USE_MINGW_ANSI_STDIO=0
LDFLAGS=
LDLIBS=-lshlwapi
.PHONY: all clean copy-deps
all: msg2wiki.exe
clean:
	$(RM) msg2wiki.exe
msg2wiki.exe: main.cpp msg2wiki.cpp msg2wiki.h parse.h
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS) main.cpp msg2wiki.cpp $(LDLIBS) -o msg2wiki.exe
copydeps:
	cp -f `$(CXX) -print-file-name=libgcc_s_dw2-1.dll` libgcc_s_dw2-1.dll
	cp -f `$(CXX) -print-file-name=libstdc++-6.dll` libstdc++-6.dll
