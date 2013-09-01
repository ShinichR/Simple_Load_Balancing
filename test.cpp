
#include        <iostream>
#include        "logger.h"
using namespace std;
int main(){
logger *lo;
lo = new logger();
while(true){
    char tmp[200];
sprintf(tmp,"Hell");
    lo->output_log(tmp);
}
}
