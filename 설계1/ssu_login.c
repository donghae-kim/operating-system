#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

char userID[16][32];
char pwdID[16][32];
char user[32];
char pwd[32];
void get_user_list(){
    int fd,n,num=0,pnum=0;

    char buf[100];
    int flag=0;
    fd=open("list.txt",O_RDONLY);
    
    for(int i=0; i<10; i++){
        num=0;
        pnum=0;
        flag=0;
        while((n = read(fd, buf,1)) > 0) {
            if(buf[0]==' '){
                flag=1;
                continue;
            }
            if(buf[0]=='\n'){
                break;
            }
            
            if(flag==0){
                userID[i][num]=buf[0];
            }
            else if(flag==1){
               
                pwdID[i][pnum]=buf[0];
                pnum++;
            }
            num++;
        }
    }
}

int check_idpw(){
    printf(1,"Username: ");
    gets(user,32);
    user[strlen(user)-1]='\0';

    printf(1,"Password: ");
    gets(pwd,32);
    pwd[strlen(pwd)-1]='\0';

    if(strcmp(user,"")==0 || strcmp(pwd,"")==0){
        return 0;
    }
    
    for(int i=0; i<10; i++){
        if(strcmp(user,userID[i])==0){
                if(strcmp(pwd,pwdID[i])==0){
                    return 1;
                }
        }
        else{
        }
    }
    return 0;
}

int main(int argc, char *argv[]){

    int pid, wpid;

    get_user_list();

    while(1){
    int a = check_idpw();
        if(a==1){
            pid = fork();
            if(pid < 0){
                printf(1, "init: fork failed\n");
                exit();
            }
            if(pid == 0){
                exec("sh", argv);
                printf(1, "init: exec sh failed\n");
                exit();
            }
            while((wpid=wait()) >= 0 && wpid != pid)
                printf(1, "zombie!\n");
            }
            else{
            printf(1,"wrong user or password\n\n");
            }
    }
    
    return 0;
}