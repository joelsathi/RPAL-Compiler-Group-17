ifeq ($(OS),Windows_NT)
    EXECUTABLE = rpal20.exe
	RM = del /Q
else
    EXECUTABLE = rpal20
	RM = rm -f
endif

rpal20: rpal20.cpp
	g++ -o $(EXECUTABLE) rpal20.cpp -Wall

clean:
	$(RM) $(EXECUTABLE)