//Marc Roman Colom, Laura Cavero Loza y Daniel Sastre Carrasco
#include "my_lib.h"

/**
 * Retorna la longitud de l'string passat per punter
 */
size_t my_strlen(const char *str){
    size_t size = 0;
    while (*(str++)){   //Mentres no es detecti el caracter \0 suma 1 a size
        size++;
    }
    return size;
}
/**
 * Copia l'string que apunta src a l'string que apunta dst
 */
char *my_strcpy(char *dest, const char *src){
    size_t len = my_strlen(src);
    for (size_t i = 0; i <= len; i++){  //Copiar dins  la posicio i de dst el que hi ha a la posicio i de src
        *(dest + i) = *(src + i);       //mentres i <= longitud de l'string que apunta src.
    }
    return dest;
}
/**
 * Retorna la resta ASCII entre els primers dos caracters distints de l'string str1 i str2.
 * La resta es de character1 - charactare2. Si un string te major longitud que un altre, es
 * retorna el valor del caracter  en positiu (si es de str1) o en negatiu (si es de str2).
 */
int my_strcmp(const char *str1, const char *str2){
    size_t len1 =  my_strlen(str1);
    size_t len2 = my_strlen(str2);
    int cmp = 0;
    for (size_t i = 0; cmp == 0 && i < len1 && i < len2; i++){
        cmp = *(str1 + i) - *(str2 + i);
    }
    return cmp;
}

/**
 * Retorna el punter dest; copia n caracters de src a dest.
 */
char *my_strncpy(char *dest, const char *src, size_t n){
    size_t i;
    for (i = 0; i < n && *(src + i) != '\0'; i++){
        *(dest + i) = *(src + i);
    }
    while (i < n){          //Si s'ha copiat tot l'str de src i encara queden n caracters, copia
        *(dest + i) = '\0'; //el final d'string n vegades restants.
        i++;
    }
    return dest;
}
/**
 * Concatena l'string src a l'string dest i retorna el punter dest.
 */
char *my_strcat(char *dest, const char *src){
    size_t i = my_strlen(dest);
    size_t j = my_strlen(src);
    for (size_t k = 0; k <= j; k++){    //Mentres k <= longitud src, copia caracter k de src a final de dest mes k.
        *(dest + i) = *(src+k);
        i++;
    }
    return dest;
}
/**
 * Inicialitza una pila amb el tamany size.
 **/
struct my_stack *my_stack_init(int size){
    struct my_stack *newStackPile = malloc(sizeof(struct my_stack));
    if (!newStackPile){
        puts("Error: no hay suficiente memoria dinámica.");
        return NULL;
    }
    newStackPile->size = size;
    newStackPile->top = NULL;
    return newStackPile;
}

/**
 * Crea un node dins la pila que apunta a dades. Retorna EXIT_SUCCESS si l'operacio ha anat be, EXIT_FAILURE en cas contrari
 * */
int my_stack_push(struct my_stack *stack, void *data){
    if (stack!=NULL && stack->size > 0){
        struct my_stack_node *newNode = malloc(sizeof(struct my_stack_node));
        if (!newNode){
            puts("Error: no se pudo crear un nuevo nodo");
            return EXIT_FAILURE;
        }
        newNode->data = data;
        newNode->next = stack->top;
        stack->top = newNode;
    }else{
        puts("Error: no existe el stack o el tamaño del dato es menor o igual a 0");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

/**
 * Elimina el node que hi ha damunt la pila i retorna el punter de dades d'aquest.
 * */
void *my_stack_pop(struct my_stack *stack){
    if (!stack){
        puts("Error: la pila no esta inicializada");
    }else if (!stack->top){
    }else{
        void *dataPointer;
        struct my_stack_node *nodePointer = stack->top;
        dataPointer = nodePointer->data;
        stack->top = nodePointer->next;
        free(nodePointer);
        return dataPointer;
    }
    return NULL;
}
/**
 * Funcio recursiva auxiliar que cerca el node que apunta a null.
 * Una vegada trobat, retorna 0 i, pels altres casos, retorna el resultat anterior sumant-li 1.
 * */
int get_stack_len(struct my_stack_node *node){
    if (!node){
        return 0;
    }else{
        return get_stack_len(node->next) + 1;
    }
}
/**
 * Retorna la quantitat de nodes que te la pila.
 * */
int my_stack_len(struct my_stack *stack){
    if (!stack){
        puts("Error: la pila no está inicializada");
        return -1;
    }
    int stackLen = get_stack_len(stack->top);
    return stackLen;
}
/**
 * Elimina la pila i el seus contingut; tant les dades com els nodes.
 * Retorna la quantitat de bytes esborrats en memoria dinamica.
 * */
int my_stack_purge(struct my_stack *stack){
    int result = 0;
    if (!stack){
        puts("Error: la pila no está inicializada");
        return -1;
    } else{
        void *dataPointer;
        int nodeSize = sizeof(struct my_stack_node);
        struct my_stack_node *node = stack->top;
        while (node){
            dataPointer = my_stack_pop(stack);
            node = stack->top;
            free(dataPointer);
            result += stack->size;
            result += nodeSize;
        }
        free(stack);
        result += sizeof(struct my_stack);
    }
    return result;
}
/**
 * Llegeix un fitxer de pila, creant una nova pila i omplint-la amb el seu contingut.
 * Retorna la pila creada.
 * */
struct my_stack *my_stack_read(char *filename){
    int fd = open(filename, O_RDONLY);
    if (fd > 0){                            //Si s'ha obert l'arxiu be, llavors continuam
        int size;
        if(read(fd,&size,sizeof(int))> 0){  //Si llegim el tamany de la pila
            struct my_stack *newStack = my_stack_init(size);    //Inicialitzam la pila amb el seu tamany.
            void *dataPointer = malloc(size);                   //Cream un punter de dades del tamany respectiu.
            while (read(fd,dataPointer,size)>0){                //Mentres llegir dades, crear nodes i ficar-los dins la pila.
                my_stack_push(newStack,dataPointer);
                dataPointer = malloc(size);
            }
            free(dataPointer);                                  //Alliberam el darrer punter que no apunta a res.
            if (close(fd) < 0) {
                puts("Error: el archivo no pudo ser cerrado");
                return NULL;
            }
            return newStack;
        }else{
            puts("Error: no contiene nada el archivo");
            return NULL;
        }   
    }else{
        puts("Error: El archivo no se pudo leer");
        return NULL;
    }
}
/**
 * Funcio recursiva auxiliar per escriure els nodes al fitxer.
 * Mentres no arribem al node que apunta a NULL, cridam la mateixa funció.
 * Una vegada arribat al node NULL, retornam 0, la resta escriu el node dins el fitxer i
 * retorna el valor anterior mes 1.
 * */
int write_nodes(int fileID,int size, struct my_stack_node *node){
    int totalSize = 0;
    if (!node){
        return totalSize;
    }
    totalSize = write_nodes(fileID ,size, node->next);
    write(fileID,node->data,size);
    return totalSize + 1;
}

/**
 * Escriu la pila dins el fitxer; retorna el nombre de nodes escrits dins la pila.
 * */
int my_stack_write(struct my_stack *stack, char *filename){
    if (!stack){                                                    //Si la pila no esta inicialitzada, retorna error.
        puts("Error: la pila no esta inicializada");
        return -1;
    }
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd > 0){                                                    //Si s'ha pogut obrir el fitxer, escriu el tamany de la pila dins el fitxer.
        write(fd,&stack->size,sizeof(int));
        int nNodes = write_nodes(fd, stack->size, stack->top);      //Escriu els nodes dins el fitxer en ordre
                                                                    //del primer ficat a la pila fins el darrer ficat.
        if(close(fd) < 0){
            puts("Error: el archivo no pudo ser cerrado");
            return -1;
        }
        return nNodes;
    }else{
        puts("Error: no se pudo escribir en el file");
        return -1;
    }   
}