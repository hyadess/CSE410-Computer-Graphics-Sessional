g++ -c *.cpp            # Compile all source files into object files
g++ *.o -o prog   # Link the object files into the executable
rm -r *.o
./prog            # Run the compiled program