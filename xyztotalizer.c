#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
pthread_mutex_t lock;

int readcount;
int linenum = 0, counter = 0;
char sendToValues[2];
char * myfifo = "/tmp/myfifo";
int x=0, y=0, z=0;
int temp;//numberreader dan gelen int değeri bununla alıyorum
int writeToPipe, readToPipe;  
void *sendToNumberReader(char tid)
{
    pthread_mutex_lock(&lock);
   // printf("sendToNumberReader içindeyim.\n");

    sendToValues[0] = tid;
    sendToValues[1] = (char)linenum;
    
    mkfifo(myfifo, 0666);
    writeToPipe = open(myfifo, O_WRONLY);
    write(writeToPipe, &sendToValues, sizeof(sendToValues));
    close(writeToPipe);
   // printf("pipe yazma bitti.\n");

//numberreader dan pipe ile okuma başlayyacak

    readToPipe = open(myfifo, O_RDONLY);
    read(readToPipe, &temp, sizeof(temp));
    close(readToPipe);
    if(tid == 'x'){
         x = temp;
         printf("x = %d\n",x);
    }
    else if(tid == 'y'){
         y = temp;
         printf("y = %d\n",y);
    } 
    else if(tid == 'z'){
         z = temp;
         printf("z = %d\n",z);
    }   
   // printf("pipe okuma bitti.\n");
    linenum++;
    pthread_mutex_unlock(&lock);
   
  return NULL;
}


int main(int argc,char *argv[],char **envp){

    readcount = atoi(argv[1]);
    printf("read count= %d\n",readcount);

   // pthread_t tids[3];
    void * status;
    if (pthread_mutex_init(&lock, NULL) != 0)
      {
         printf("\n mutex init hatasi\n");
         return 1;
     }

  //pthread_create(tids, NULL, sendToNumberReader, 'x');// 1 parametresi fonksiyona gönderdiğimiz değer threadin idsi anlamında. FOnksiyon int bir değer bekliyor ya bu 1 oraya gidiyor. ilk parametre de thread hakkında bilgi tutar thread beklemede çalışıyor gibi. Biz bu projede 1 yerine char olarak x y z vereceğiz.
    pthread_t xthread;
    pthread_t ythread;
    pthread_t zthread;
    
    for(int i=0; i<readcount/3; i++){//her thread bir satır okuduğundan bir loopta 3 satır okunur. Bundan dolayı readcount/3 yaptım.
        pthread_create(&xthread, NULL, sendToNumberReader, 'x');
        pthread_create(&ythread, NULL, sendToNumberReader, 'y');
        pthread_create(&zthread, NULL, sendToNumberReader, 'z');

        pthread_join(xthread, &status);
        pthread_join(ythread, &status);
        pthread_join(zthread, &status);

        int result = x + y + z;
        printf("%d + %d + %d = %d\n",x,y,z,result);
    }
    printf("END\n");
    pthread_mutex_destroy(&lock);
	return 0;
}
