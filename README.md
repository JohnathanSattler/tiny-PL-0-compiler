# tiny-PL/0 compiler

Compile compiler: make
Compile vm: gcc -o vm vm.c

run compiler: ./compile (input) (output)
run vm: ./vm (input)

Example: ./compile input.pl0 output.pm0
Example: ./vm output.pm0
