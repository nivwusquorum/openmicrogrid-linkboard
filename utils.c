#include "utils.h"

#include "p33FJ64GS406.h"


void blink(int times) {
    long long l;
    while(times--) {
        LATFbits.LATF5 = 1;
        for (l=0; l<500000L; ++l);
        LATFbits.LATF5 = 0;
        for (l=0; l<500000L; ++l);
    }
}
