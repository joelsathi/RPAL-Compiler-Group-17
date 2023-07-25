ifeq ($(OS),Windows_NT)
    EXECUTABLE = rpal20.exe
else
    EXECUTABLE = rpal20
endif

rpal20: rpal20.cpp
	g++ -o $(EXECUTABLE) rpal20.cpp -Wall

clean:
	rm -f $(EXECUTABLE)