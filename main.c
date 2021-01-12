#include "my_lib.c"
#include "main.h"
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char **argv){
    if (argc > 1){
        pthread_t threads[NUM_THREADS];
        struct my_stack *stack = my_stack_read(argv[1]);
        if (!stack){                                    //Si la pila está vacía, la crea.
            puts("Creando una nueva...");
            stack = my_stack_init(sizeof(int));
        }
        int length = my_stack_len(stack);
        int *ptr;
        while(length < N_ELEMENTS && length != -1){     //Si la pila tiene menos de 10 elementos, le añade hasta tener 10
            ptr = malloc(sizeof(int));
            *ptr = 0;
            my_stack_push(stack, ptr);
            length++;
        }
        while(length > N_ELEMENTS){                     //Si la pila tiene mas de 10 elementos, los elimina.
            free(my_stack_pop(stack));
            length--;
        }
        
        int rc;
        for (int i = 0; i < NUM_THREADS; i++){       //Creamos los threads y los ponemos a trabajar con la función que deben llamar.
            printf("[%d] Thread %ld...\n",i, threads[i]);
            rc =pthread_create(&threads[i],NULL,worker,stack);
            if (rc){
                perror("Error al intentar crear el thread");
            }
            
        }
        for (size_t i = 0; i < NUM_THREADS; i++){       //Esperamos a que todos los threads acaben.
            pthread_join(threads[i], NULL);
        }
        my_stack_write(stack, argv[1]);
        my_stack_purge(stack);
        pthread_exit(NULL);
    }else{
        puts("Error: No se ha especificado el nombre de la pila.\nSyntaxis: ./av3 <nombre_pila>");
    }
}
void *worker(void *arg){
    struct my_stack *stack = (struct my_stack *) arg;
    int *data;
    for (size_t i = 0; i < N; i++){
        pthread_mutex_lock(&mutex);
        data = my_stack_pop(stack);
        printf("Thread [%ld]: Hago pop, el valor es [%d]\n",pthread_self(), *data);
        pthread_mutex_unlock(&mutex);
        *(data) = *(data)+1;
        pthread_mutex_lock(&mutex);
        my_stack_push(stack,data);
        printf("Thread [%ld]: Hago push, el valor es [%d]\n",pthread_self(), *data);
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL);
}