#include <dirent.h>
#include <errno.h>
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char path[] = "./";
int d = 0, f = 0, l = 0, s = 0;

typedef enum types { FILE_TYPE, DIRECTORY, LINK } types;

typedef struct item {
    char *name;
    types type;
} Item;

int size_of_list = 0;

Item make_item(struct dirent *it) {
    Item one;
    one.name = it->d_name;
    one.type = (it->d_type == DT_DIR) ? DIRECTORY :
               (it->d_type == DT_REG) ? FILE_TYPE :
               (it->d_type == DT_LNK) ? LINK : FILE_TYPE;
    return one;
}

void print_files(Item *files) {
    if (!files)
        return;

    for (int i = 0; i < size_of_list; i++) {
        if ((files[i].type == DIRECTORY && d) ||
            (files[i].type == FILE_TYPE && f) ||
            (files[i].type == LINK && l))
            printf("%s\n", files[i].name);
    }

    free(files);
}

Item *sort_files(Item *files) {
    if (!files || !s)
        return files;

    for (int i = 0; i < size_of_list - 1; i++)
        for (int j = 0; j < size_of_list - i - 1; j++)
            if (strcmp(files[j].name, files[j + 1].name) > 0) {
                Item tmp = files[j];
                files[j] = files[j + 1];
                files[j + 1] = tmp;
            }

    return files;
}

Item *read_files(char *dir_name) {
    DIR *dir = opendir(dir_name);
    if (!dir) {
        perror("opendir");
        return NULL;
    }

    struct dirent *dir_item;
    Item *files = NULL;

    while ((dir_item = readdir(dir)) != NULL) {
        if (strcmp(dir_item->d_name, ".") == 0 || strcmp(dir_item->d_name, "..") == 0)
            continue;

        files = realloc(files, (size_of_list + 1) * sizeof(Item));
        if (!files) {
            fprintf(stderr, "realloc failed\n");
            closedir(dir);
            return NULL;
        }
        files[size_of_list++] = make_item(dir_item);
    }

    if (closedir(dir) != 0) {
        perror("closedir");
        return NULL;
    }

    return files;
}

void read_options(int argc, char **argv) {
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            for (int j = 1; argv[i][j]; j++) {
                switch (argv[i][j]) {
                    case 'l':
                        l = 1;
                        break;
                    case 'd':
                        d = 1;
                        break;
                    case 'f':
                        f = 1;
                        break;
                    case 's':
                        s = 1;
                        break;
                    default:
                        fprintf(stderr, "Invalid option: -%c\n", argv[i][j]);
                        exit(EXIT_FAILURE);
                }
            }
        } else {
            strcpy(path, argv[i]);
        }
    }

    if (!l && !d && !f) {
        d = f = l = 1;
    }
}

int main(int argc, char **argv) {
    read_options(argc, argv);
    print_files(sort_files(read_files(path)));
    return 0;
}
