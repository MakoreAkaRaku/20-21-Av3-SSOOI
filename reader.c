#include "my_lib.c"
#include "reader.h"
int main(int argc, void **argv){
    if (argc > 1){
        int s_length;
        struct my_stack *stack = my_stack_read(argv[1]);
        s_length = my_stack_len(stack);
        printf("Stack length: %d\n",s_length);
        int *numbers[s_length];
        int sum = 0;
        for (size_t i = 0; i < s_length; i++){
            numbers[i] = my_stack_pop(stack);
            sum += *numbers[i];
            printf("[%ld] value: %d\n",i,*numbers[i]);
        }
        printf("Items: %d, sum: %d min: %d max: %d Average: %d", s_length,sum, min(numbers,s_length),max(numbers,s_length) ,sum/s_length);
        
    }else{
        puts("Error: especifica el nombre de la pila!");
        return 1;
    }
}
/**
 * Devuelve el valor mínimo del array
 * */
int min(int *numbers[], int length){
    int val_min = *numbers[0];
    for (size_t i = 1; i < length; i++){
        if (val_min > *numbers[i]){
            val_min = *numbers[i];
        }
    }
    return val_min;
}
/**
 * Devuelve el valor máximo del array
 * */
int max(int *numbers[], int length){
    int val_max = 0;
    for (size_t i = 1; i < length; i++){
        if (val_max < *numbers[i]){
            val_max = *numbers[i];
        }
    }
    return val_max;
}