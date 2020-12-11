# LSP-Assignments-Set-1
1_fork2c : fork two children; have the parent wait for both to die; ensure zombies are prevented using the signalling mechanism discussed for 2.6 Linux [Weightage: 8]

2_sigcomm: have a process A send a 'message' to another cooperating process B via a real-time signal; the message is an integer value supplied to A as a parameter(s); verify that B receives it. This must continue indefinitely, or until B receives the special 'stopping' value -1 [Weightage: 5]

3_libsleepsafe: write a small C library; it must contain the function sleepsafe(); - your implementation of a ‘safe and correct’ sleep() functionality (the sleep must not abort if ‘disturbed’ by signals). Create a shared library (call it mylib.so) on Linux and place the function there; test by calling it from an app (you will need to research how to create a shared library on Linux)  [Weightage: 10]

4_segv: write a sophisticated signal handler to handle the Segmentation Violation case; it must display all relevant information that the kernel makes available; bonus points for using an alternate signal stack for handling the signal [Weightage: 8-10]

5_hxdump_buggy: Debug the provided buggy program (5_hxdump_buggy.c) with whichever tools you wish; fix it (call it 5_hxdump_correct.c), and generate a patch [Weightage: 5]
