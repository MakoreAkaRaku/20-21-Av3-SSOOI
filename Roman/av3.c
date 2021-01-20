// Marc Roman Colom
#include "av3.h"
#include "my_lib.h"
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char **argv){
    if (argc > 1){
        pthread_t threads[NUM_THREADS];
        struct my_stack *stack = my_stack_read(argv[1]);
        struct my_stack *temp_stack = my_stack_init(sizeof(int));   //stack temporal para el caso en que hay mas elementos de los que iterar
        if (!stack){                                            //Si no hay ninguna pila que leer, la crea.
            puts("Creando una nueva pila...");
            stack = my_stack_init(sizeof(int));
        }
        int *ptr;
        int length = my_stack_len(stack);
        printf("Antigua longitud: %d\n", length);
        while(length < N_ELEMENTS){ //Si la pila tiene menos de  N_ELEMENTOS, le añade hasta tener 10.
            ptr = malloc(sizeof(int));
            if (!ptr){                      //Si no se puede hacer espacio en memoria, error.
                perror("Error: No hay suficiente memoria dinamica");
                return EXIT_FAILURE;
            }
            *ptr = 0;
            my_stack_push(stack, ptr);
            length++;
        }
        length = length - (length - N_ELEMENTS);
        for (size_t i = 0; i < N_ELEMENTS; i++){    //Volcamos el maximo de elements a iterar en la pila a la pila de iteración.
            my_stack_push(temp_stack,my_stack_pop(stack));
        }
        printf("Tamaño para iterar del stack: %d\n", length);
        int rc;
        for (int i = 0; i < NUM_THREADS; i++){       //Creamos los threads y los ponemos a trabajar con la función que deben llamar.
            
            rc = pthread_create(&threads[i],NULL,worker,temp_stack);
            if (rc){
                perror("Error al intentar crear el thread");
            }else{
                printf("[%d] Thread %ld created!\n", i, threads[i]);
            }
        }
        printf("Threads: %d Nº Iterations %d\n", NUM_THREADS, N);
        for (size_t i = 0; i < NUM_THREADS; i++){       //Esperamos a que todos los threads acaben.
            pthread_join(threads[i], NULL);
        }

        for (size_t i = 0; i < N_ELEMENTS; i++){        // Devolvemos los elementos iterados a la pila original
            my_stack_push(stack,my_stack_pop(temp_stack));
        }
        

        int nOfNodes = my_stack_write(stack, argv[1]);
        printf("Se han escrito %d elementos\n", nOfNodes);
        my_stack_purge(stack);                          //Liberamos el espacio de la pila
        my_stack_purge(temp_stack);                     //Liberamos el espacio de la pila usada para iterar
        pthread_exit(NULL);
    }else{
        puts("Error: No se ha especificado el nombre de la pila.\nSintaxis: ./av3 <nombre_pila>");
    }
}
void *worker(void *ptr){
    struct my_stack *stack = (struct my_stack *) ptr;
    int *data;
    int newData = 0;
    for (size_t i = 0; i < N; i++){

        pthread_mutex_lock(&mutex);
        
        data = my_stack_pop(stack);
        newData = *data;
        //printf("Thread [%ld]: Hago pop, el valor es [%d]\n",pthread_self(), *data);
        pthread_mutex_unlock(&mutex);
        newData++;
        //sleep(0.01);
        pthread_mutex_lock(&mutex);
        *(data) = newData;
        my_stack_push(stack, data);
        //printf("Thread [%ld]: Hago push, el valor es [%d]\n",pthread_self(), *data);
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL);
}