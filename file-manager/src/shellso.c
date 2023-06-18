#include "main.h"

/*Function to read input and generate an history*/
int readInput(char* cmd_line){

    char *buffer;
    
    buffer = readline(">> ");

    //Receive a crtl+d signal
    if(!buffer){
        return 2;
    }
    //Receive an empty string
    if(strlen(buffer) < 1){
        return 1;
    }
    
    add_history(buffer);
    strcpy(cmd_line,buffer);
    free(buffer);
    return 0;
}

/*Process the cmd line, separating each command and their args*/
char **parserStr(char *str, char* sep, int* argc){

    char **command = (char **)calloc(sizeof(char *), MAX_ARGS);
    char *token;
    int i;
    (*argc) = 0;
    //Get each commands in line separated by pipe
    token = strtok(str, sep);
    for (i = 0; token != NULL; i++){
        if (strlen(token) == 0)
            i--;
        else{
            command[i] = token;
            (*argc)++;
        }
        token = strtok(NULL, sep);
    }

    return command;
}


/*Execute builtin commands or return 0 if its not a builtin*/
int builtinCommands(char **argv){

    char** path = NULL;
    int sz = 0;

    if(strcmp(argv[0],"init") == 0){
        init();
    }
    else if(strcmp(argv[0],"load") == 0){
        load();
    }
    else if(strcmp(argv[0],"mkdir") == 0){
        create(argv,1);
    }
    else if(strcmp(argv[0],"create") == 0){
        create(argv,0);
    }
    else if(strcmp(argv[0],"ls") == 0){
        ls(argv);
    }
    else if(strcmp(argv[0],"unlink") == 0){
        ulink(argv);
    }
    else if(strcmp(argv[0],"write") == 0){
        cwrite(argv);
    }
    else if(strcmp(argv[0],"read") == 0){
        cread(argv);
    }
    else if(strcmp(argv[0],"append") == 0){
        append(argv);
    }
    else if(strcmp(argv[0],"help") == 0){
        help();
    }
    else if(strcmp(argv[0],"quit") == 0){
        printf("See you later!\n");
        exit(0);
    }
    else{
        printf("\nCommand not find!\nEnter help to see available commands\n");
        return 0;
    }
    if(path != NULL) free(path);

    return 1;
}

void help(){

    printf("Available commands:\n"
        "• init                             - inicializar o sistema de arquivos\n"
        "• load                             - carregar o sistema de arquivos do disco\n"
        "• ls [/caminho/diretorio]          - listar diretório\n"
        "• mkdir [/caminho/diretorio]       - criar diretório\n"
        "• create [/caminho/arquivo]        - criar arquivo\n"
        "• unlink [/caminho/arquivo]        - excluir arquivo ou diretório (o diretório precisa estar vazio)\n"
        "• write string [/caminho/arquivo]  - escrever dados em um arquivo (sobrescrever dados)\n"
        "• append string [/caminho/arquivo] - anexar dados em um arquivo\n"
        "• read [/caminho/arquivo]          - ler o conteúdo de um arquivo\n"
        "• quit                             - close the program\n");
}

int shell(){

    char cmd_line[MAX_CMD];
    char** command;
    int read_sig;
    int argc;

    system("clear");
    while (1){
        
        read_sig = readInput(cmd_line);
        //Receive a ctrl+d signal
        if(read_sig == 2)
            exit(0);

        //Receive an empty string
        if(read_sig == 1)
            continue;

        command = parserStr(cmd_line, " ", &argc);

        builtinCommands(command);
 
        free(command);
    }

    return 0;
}

void erro(int err){
    fprintf(stderr,"error: %s\n",strerror(err));
}