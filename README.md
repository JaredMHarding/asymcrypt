Jared Harding
jared.harding@wsu.edu

# Asymmetric Encryption Algorithm

## Description

This is an asymmetric encryption algorithm similar to RSA coded in C.
This program should be able to encrypt any plaintext.
The program uses the Miller-Rabin algorithm for finding prime numbers. Sources for those functions are included below...

[Miller-Rabin Pseudocode](https://en.wikipedia.org/wiki/Miller%E2%80%93Rabin_primality_test#Computational_complexity)

[Modular Multiplication](https://www.geeksforgeeks.org/how-to-avoid-overflow-in-modular-multiplication/)

## List Of Files

* main.c
    - Source code that runs the main program.
* asymcrypt.c
    - Source code that has the encrypt/decrypt algorithms
* asymcrypt.h
    - Header file for asymcrypt.c
* miller-rabin.c
    - Source code for prime testing, modular exponentiation/multiplication
* miller-rabin.h
    - Header file for miller-rabin.c
* Makefile
    - File for correctly assembling and compiling the "asymcrypt" program
* README.md
    - This file

## How To Use The Program

After decompressing the archive, run the `$ make` command to create an executable in the same directory.
You may also enter `$ make clean` to remove all of the .o and executable files.

To run the program, you may enter the following commands in the terminal...

    * `$ ./asymcrypt keygen <seed>`
    * `$ ./asymcrypt encrypt <seed>`
    * `$ ./asymcrypt decrypt`

The `keygen` command will create two files, pubkey.txt and prikey.txt.
The format of these files will be a single line file that contains the integer key values separated by single spaces, like so:

"p g e2" for pubkey.txt
"p g d" for prikey.txt

The `encrypt` command expects two files to exist, pubkey.txt (as formated above) and ptext.txt (a pure ASCII plaintext file with your message), which has the plaintext you wish to encrypt.
This will create a file called ctext.txt (or overwrite an existing file) which has pairs of ciphertext values in base 10 on each line separated by a single space.

The `decrypt` command expects two files to exist, prikey.txt (as formated above) and ctext.txt (as described above).
This will create a file called ptext.txt (or overwrite an existing file) as described above.

If the user does not provide a numerical seed value on the command line for the ones that need one, they will be prompted for it after pressing enter/return.

### Notes

The command may be `$ ./asymcrypt.exe` if compiled on a Windows machine.
