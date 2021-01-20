//Marc Roman Colom
#include <limits.h>
#include "my_lib.c"
#include "reader.h"

int main(int argc, char **argv){
    if (argc > 1){
        struct my_stack *stack = my_stack_read(argv[1]);
        if (!stack){ //Si el stack apunta null, es que no existe el fichero
            perror("No existe la pila!");
            return EXIT_FAILURE;
        }
        int s_length = my_stack_len(stack);
        int *node_values[s_length];
        printStack(stack, s_length, node_values);
        int min = minimo(node_values,s_length);
        int max = maximo(node_values,s_length);
        int sum = sumatorio(node_values,s_length);
        int median = sum/ s_length;
        printf("Items: %d, sum: %d min: %d max: %d Average: %d\n", s_length,sum, min,max,median);
        for (size_t i = 0; i < s_length; i++){  //Liberamos los punteros
            free(node_values[i]);
        }
        my_stack_purge(stack);                  //Liberamos el espacio que ocupa la pila
    }else{
        puts("Error: especifica el nombre de la pila!\nSintaxis: ./reader <nombre_pila>");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
/**
 * Devuelve el valor mínimo del array
 * */
int minimo(int *numbers[], int length){
    int val_min = INT_MAX;
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
int maximo(int *numbers[], int length){
    int val_max = 0;
    for (size_t i = 1; i < length; i++){
        if (val_max < *numbers[i]){
            val_max = *numbers[i];
        }
    }
    return val_max;
}

/**
 * Devuelve la suma de todos los valores del array
 * */
int sumatorio(int *numbers[], int length){
    int sum = 0;
    for (size_t i = 0; i < length; i++){
        sum += *numbers[i];
    }
    return sum;
}

/**
 * Imprime por pantalla el contenido del stack, extrayendo sus elementos y pasandolos al puntero de punteros destino.
 * */
void printStack(struct my_stack *stack, int s_length, int **dst){
    printf("Stack length: %d\n", s_length);
    for (size_t i = 0; i < s_length; i++){
        dst[i] = my_stack_pop(stack);
        printf("[%ld] value: %d\n", i, *dst[i]);
    }
}