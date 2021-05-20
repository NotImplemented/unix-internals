//cpu #1 

while (f == 0) { }
 // Здесь необходим барьер
print x;


//cpu #2:

x = 42;
 // Здесь необходим барьер
f = 1;



LFENCE
SFENCE
MFENCE


