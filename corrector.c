#include <stdio.h>
#include <string.h>
#include <dirent.h>
char datab[256][3][256];
int n = 0;
int EndsWithTail(char *url)
{
    if (4 > strlen(url))
        return 0;
    int len = strlen(url);

    if (strcmp(&url[len-4],".txt") == 0)
        return 1;
    return 0;
}

void databsasereader()
{
    FILE* data;
    char read[100];
    data = fopen("database.txt", "r");
    if (data==NULL)
    {
        return;
    }
    while (fscanf(data, "%s", read)!=EOF)
    {
        strcpy(datab[n][0],read);
        fscanf(data, "%s", read);
        strcpy(datab[n][1],read);
        fscanf(data, "%s", read);
        strcpy(datab[n][2],read);
        n=n+1;
    }
}
void corrector(const char* dirname) {
    DIR* dir = opendir(dirname);
    if (dir == NULL) {
        return;
    }
    struct dirent* entity;
    entity = readdir(dir);
    
    while (entity != NULL) {
        if (strcmp(entity->d_name, ".") != 0 && strcmp(entity->d_name, "..") != 0)
        {
            char path[100] = { 0 };
            strcat(path, dirname);
            strcat(path, "/");
            strcat(path, entity->d_name);
            if (EndsWithTail(entity->d_name)==1)
            {
                if (!((strcmp(entity->d_name,"database.txt")==0)&&strcmp(dirname, ".")==0)) {
                    FILE* fileptr;
                    fileptr = fopen(path, "r+");
                    if (fileptr==NULL)
                    {
                        return;
                    }
                    else
                    {
                        char word[256];
                        int loc;
                        char nameinf[256];
                        while (fscanf(fileptr, "%s",word)!=EOF) {
                            if (strcmp(word, "Mr.")==0||strcmp(word, "Ms.")==0)
                            {
                                loc = ftell(fileptr);
                                loc = loc-3;
                                fscanf(fileptr, "%s", nameinf);
                                int i;
                                char completename[256]="";
                                for (i = 0; i<n; i++)
                                {
                                    if (strcmp(nameinf, datab[i][1])==0)
                                    {
                                        if (strcmp(datab[i][0],"m")==0)
                                        {
                                            strcat(completename, "Mr. ");
                                        }
                                        else if (strcmp(datab[i][0],"f")==0)
                                        {
                                            strcat(completename, "Ms. ");
                                        }
                                        strcat(completename,datab[i][1]);
                                        strcat(completename," ");
                                        strcat(completename,datab[i][2]);
                                    }
                                }
                                fseek(fileptr,loc,SEEK_SET);
                                fputs(completename,fileptr);
                            }
                        }
                    }
                }
            }
            corrector(path);
        }
        entity = readdir(dir);
    }

    closedir(dir);
}

int main(int argc, char* argv[]) {
    databsasereader();
    corrector(".");
    printf("All texts are corrected.\n");
    return 0;
}
