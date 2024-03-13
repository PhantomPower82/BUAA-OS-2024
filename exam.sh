mkdir test
cp -r code test
cd code
cat 14.c
cd ../test/code
for i in {0..15}; do
	gcc -c $i.c -o $i.o
done
gcc *.o -o hello
./hello 2> err.txt
mv err.txt ../..
cd ../..
chmod 655 err.txt
sed -n "$[${1:-1}+${2:-1}]p" err.txt >&2
