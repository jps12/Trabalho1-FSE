#include <string> 

std::string convert_to_string(int value){
    unsigned char *p = (unsigned char *) &value;
    std::string retorno = "****";
    for (int i=0;i<4;i++){
        retorno[i] = *p;
        p++;
    } 
    return retorno;
}