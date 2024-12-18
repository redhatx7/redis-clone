
#ifndef RESP_H
#define RESP_H

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>


struct Context {
    struct HashMap *hashmap;
    int client_fd;
};





struct RedisResponse {
    const char *response;
    size_t length;
    int client_fd;
};



typedef enum RedisCommand {
    GET,
    SET,
    UNKNOWN
} RedisCommand;

struct ParseResult {
    RedisCommand command;
    const char **args;
    size_t num_args;
    char *error;
};

struct Parser {
    int current_index;
    const char *buffer;
};

char* create_response_buffer(){
    return "Hello World";
}

int expect_char(struct Parser *parser, char ch){
    //printf("Expecting char %c getting %c\n", ch, parser->buffer[parser->current_index]);
    assert(parser->buffer[parser->current_index] == ch);
    return parser->buffer[parser->current_index++] == ch;
}

void skip_clrf(struct Parser *parser){
    expect_char(parser, '\r');
    expect_char(parser, '\n');
}

char* read_string(struct Parser *parser, size_t len){
    char *str = malloc(128);
    int str_len = 0;
    for (int i =0; i < len; i++){
        printf("Adding char %c\n", parser->buffer[parser->current_index]);
        str[str_len++] = parser->buffer[parser->current_index++];
    }
    skip_clrf(parser);
    return str;
}


RedisCommand parse_redis_command(const char* buffer){
    if(strcmp(buffer, "GET") == 0){
        return GET;
    } else if(strcmp(buffer, "SET") == 0){
        return SET;
    } else {
        return UNKNOWN;
    }
}

int read_number(struct Parser *parser){
    char num[10];
    int num_len = 0;
   
    while(parser->buffer[++parser->current_index] != '\r' ) {
        printf("Parsing integer %c\n",parser->buffer[parser->current_index]);
        if (isdigit(parser->buffer[parser->current_index])){
            num[num_len++] = parser->buffer[parser->current_index];
        } else {
            fprintf(stderr, "Invalid character in integer %c\n", parser->buffer[parser->current_index]);
            return -1;
        }
    }
    skip_clrf(parser);
    printf("Num array %s\n", num);
    int int_num = atoi(num);
    return int_num;
}



struct ParseResult parse_response( const char *buffer, size_t length){
    
    int char_index = 0;

    struct Parser parser = {
        .buffer = buffer,
        .current_index = 0,
        .state = INITIAL_STATE
    };
    struct ParseResult parse_result = {
        .command = UNKNOWN,
    };

    if (parser.buffer[parser.current_index] != '*'){
        fprintf(stderr, "Invalid response, Expected starting with \'*\', Got \'%c\'\n", parser.buffer[parser.current_index]);
        return parse_result;
    }

    int num_args = read_number(&parser);
    if (num_args <= 0){
        fprintf(stderr, "Invalid number of arguments\n");
        return parse_result;
    }

    parse_result.num_args = num_args;

    if (parser.buffer[parser.current_index] != '$'){
        fprintf(stderr, "Invalid response, Expected starting with \'$\', Got %c\n", parser.buffer[parser.current_index]);
        return parse_result;
    }

    parse_result.args = malloc(num_args * sizeof(char*));
    int cmd_len = read_number(&parser);
    
    char *command = read_string(&parser, cmd_len);
    printf("Command = %s\n", command);
    parse_result.command = parse_redis_command(command);
    parse_result.args[0] = command;
   

    for (int i = 1; i< num_args; i++){
        if (parser.buffer[parser.current_index] != '$'){
            fprintf(stderr, "Invalid response, Expected starting with \'$\', Got %c\n", parser.buffer[parser.current_index]);
            return parse_result;
        }
        int arg_len = read_number(&parser);
        parse_result.args[i] = read_string(&parser, arg_len);
    }

    return parse_result; 

}

char* handle_client(struct Context *context, const char* buffer, size_t len){
    struct ParseResult result = parse_response(buffer, len);
    if (result.command == GET){
        printf("GET Command\n");
        return "GET Command";
    } else if (result.command == SET){
        printf("SET Command\n");
        return "SET Command";
    } else {
        printf("Unknown Command\n");
        return "Unknown Command";
    }
}

#endif