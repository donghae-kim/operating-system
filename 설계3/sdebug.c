#include "types.h"
#include "stat.h"
#include "user.h"
#define PNUM 5
#define PRINT_CYCLE 10000000
#define TOTAL_COUNTER 500000000

void sdebug_func(void){
    int pid;
    printf(1, "start sdebug command\n");
    int starttime;
    long i;
    //pnum 갯수만큼 fork
    for(i=0; i<PNUM; i++){
        pid = fork();

        if(pid<0){
            break;
        }
        if (pid==0) {
            //프로세스 시작 시간 측정
            starttime =  uptime();
            long counter=0;
            long print_counter =PRINT_CYCLE;
            //weightset 시스템콜을 통해서 현재 프로세스의 weight값 설정
            int weight = weightset(i+1);
            int first=1;
            //counter가 totalcounter가 될때가지 실행
            while(counter <=TOTAL_COUNTER){
                counter++;
                print_counter--;
                //출력카운터가 천만이 다되면 출력
                if(print_counter==0){
                    if(first){
                        //종료 시간 측정
                        int endtime = uptime();
                        printf(1, "PID: %d, WEIGHT: %d, TIMES: %d ms\n", getpid(),  weight, (endtime-starttime)*10);
                        first=0;
                    }
                    print_counter = PRINT_CYCLE;
                }
            }
            //terminated 출력
                printf(1, "PID: %d terminated\n",getpid());
                exit();
        }
    }

    //자신의 프로세스 개수만큼 wait
    for(; i > 0; i--){
        if(wait() < 0){
        printf(1, "wait stopped early\n");
        exit();
        }
    }

    //위에가 잘 작동했다면 이제 자식이 없어야한다.
    if(wait() != -1){
        printf(1, "wait got too many\n");
        exit();
    }

    printf(1, "end of sdebug command\n");
    return ;    
}

int main(void){
    sdebug_func();
    exit();
}
