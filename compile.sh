mount -o loop disk/disk.img mnt


cd src/build

gcc -c -masm=intel -m32 ../lib/*.c -ffreestanding

gcc -c -masm=intel -m32 ../multiboot.S ../kmain.c -ffreestanding

ld -m elf_i386 -T ../script.txt -o kernel *.o


cd ../..
\cp src/build/kernel mnt/system


umount mnt
