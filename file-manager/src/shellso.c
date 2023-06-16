#include "shellso.h"

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
char **processInput(char *cmd){

    char **command = (char **)calloc(sizeof(char *), MAX_ARGS);
    char *token;
    int i;

    //Get each commands in line separated by pipe
    token = strtok(cmd, " ");
    for (i = 0; token != NULL; i++){
        if (strlen(token) == 0)
            i--;
        else
            command[i] = token;

        token = strtok(NULL, "|");
    }

    return command;
}


/*Execute builtin commands or return 0 if its not a builtin*/
int builtinCommands(char **command){

    if(strcmp(command[0],"init") == 0){
        init();
    }
    else if(strcmp(command[0],"load") == 0){
        load();
    }
    else if(strcmp(command[0],"help") == 0){
        help();
    }
    else if(strcmp(command[0],"quit") == 0){
        printf("See you later!\n");
        exit(0);
    }
    else{
        printf("\nCommand not find!\nEnter help to see available commands\n");
        return 0;
    }

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

    system("clear");
    while (1){
        
        read_sig = readInput(cmd_line);
        //Receive a ctrl+d signal
        if(read_sig == 2)
            exit(0);

        //Receive an empty string
        if(read_sig == 1)
            continue;

        command = processInput(cmd_line);

        builtinCommands(command);
 
        free(command);
    }

    return 0;
}