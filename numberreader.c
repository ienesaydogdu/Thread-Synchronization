#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
int main(int argc,char *argv[],char **envp){
    
    char c;
    int value;
    FILE *file;
    int linenum; // kaçıncı satırı okuduğumuzu tutuyoruz
    int counter = 0; // while içinde counter = linenum olursa while break olacak
    char gelendegerler[2];
    int pipeOku,writeToPipe;
    char * myfifo = "/tmp/myfifo";
    mkfifo(myfifo, 0666);
    while(1){
       
        pipeOku = open(myfifo,O_RDONLY);
        read(pipeOku, &gelendegerler, sizeof(gelendegerler));
        close(pipeOku);
       // printf("file open başarılı!\n");
        linenum = (int)gelendegerler[1];               
       // printf("atoi başarılı!\n");
        file = fopen("numbers.txt", "r");
        if (file) {
            while (!feof(file)){// txtden veri okunuyor
                    fscanf (file, "%d", &value);
         
                if(counter == linenum)break;//eşitlenme olduğunda istediğim değer c de oluyor ve while çıkıyorum (linenum*2) dememin sebebi her satırda bir değer ve satır sonu karakteri var. Satır sonu karakterini de okuduğundan her iki dönüşte alttaki değeri okuyabiliyorum.
                counter++;
             }
            fclose(file);
            counter = 0;
        }
    
    printf("%d. %c= %d \n",linenum,gelendegerler[0],value);
    
    writeToPipe = open(myfifo,O_WRONLY);
    write(writeToPipe, &value, sizeof(value));
    close(writeToPipe);
    }
	return 0;
}
