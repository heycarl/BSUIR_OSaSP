//
// Created by sasha on 2/16/23.
//

#ifndef OSASP_LABS_FUNC_H
#define OSASP_LABS_FUNC_H

#define CLEAR_SCREEN system("clear");

void print_env(char* envp[]);
char** make_child_env(char* file_envp);

char* get_child_path(char** str_arr);

#endif //OSASP_LABS_FUNC_H
