#include <stdio.h>

int main(){
    int flag[22] = {-99, -103, -128, -124,-117, -101, -63, -98, -108, -61, -126, -81, -126, -61, -122, -61, -126, -125, -61, -115, -16};
    
    for(int i = 0;i<22;i++){
        flag[i] ^= ~15;
    }

    printf("Done! Flag in array");

    return 0;
}