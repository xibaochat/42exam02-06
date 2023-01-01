clang -Wall -Wextra ft_printf.c tmp_main.c -o ft_printf
./ft_printf  > yy
cat -e yy > y

clang -Wall -Wextra -D REAL tmp_main.c -o printf


./printf | cat -e> r
diff -y --suppress-common-lines r y
rm -rf yy y r ft_printf printf
