#include<stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <limits.h>
#include <time.h>

#define PAGESTRINGNUMBER 500
int split(char* string, char* seperator, char* argv[]);
void randomNumberMaker();
void randomNumberPrint();
void startSimulator(int count,char **splitPageReplacement, int inputPageFrame);
void optimalSimulator(int inputPageFrame);
int checkIn(int *pageFrame, int nextValue);
int checkClose(int *pageFrame,int currentLocation, int *fifoHistory);
void fifoSimulator(int inputPageFrame);
void lifoSimulator(int inputPageFrame);
void lruSimulator(int inputPageFrame);
void lfuSimulator(int inputPageFrame);
void scSimulator(int inputPageFrame);
void escSimulator(int inputPageFrame);
void randomNumberBitMaker();
void randomNumberRBitMaker();
char pageString[PAGESTRINGNUMBER];
char rbitString[PAGESTRINGNUMBER];
char wbitString[PAGESTRINGNUMBER];
int inputPageFrame;

FILE* fp = NULL;

int main(){
    //결과 저장하는 파일
    fp = fopen("save.txt","a+");

    while(1){
        char inputPageReplacement[10]="";
        char *splitPageReplacement[10]; 
        inputPageFrame=0;
        int inputDataMethod=0;
        int inputExit=0;
        int count=0;
        
        if (fp == NULL)
        {
            fprintf(stderr, "File Open Error!\n");
            exit(1);
        }
        //재시행 마다 pageString reset
        memset(pageString,0,PAGESTRINGNUMBER);

        //page replacement 알고리즘 선택하는 부분
        printf("A. Page Replacement 알고리즘 시뮬레이터를 선택하시오 (최대 3개)\n");
        printf("(1) Optimal  (2) FIFO  (3) LIFO  (4)  LRU  (5) LFU  (6) SC  (7) ESC  (8) ALL\n");
        fgets(inputPageReplacement,sizeof(inputPageReplacement),stdin);
        
        count = split(inputPageReplacement," ",splitPageReplacement);
        
        for(int i=0; i<count; i++){
            if(atoi(splitPageReplacement[i])<1 || atoi(splitPageReplacement[i])>8){
                printf("잘못된 알고리즘 선택 갯수입니다. 1~8 만 입력 가능\n");
                return 0;
            } 
        }
        if(count>3 || count<1){
            printf("잘못된 알고리즘 선택 갯수입니다. 최소1개에서 3개만 입력 가능\n");
            return 0;
        }
        if(count>=2 && count<=3){
            for(int i=0; i<count; i++){
                if(atoi(splitPageReplacement[i])==8){
                    printf("8번 all 제외 최대 3개 알고리즘 선택 가능\n");
                    return 0;
                }
            }
            
        }

        //page 프레임 개수 선택하는 부분
        printf("B. 페이지 프레임의 개수를 입력하시오. (3~10)\n");
        scanf("%d", &inputPageFrame);
        if(inputPageFrame<3 || inputPageFrame>10){
            printf("잘못된 페이지 프레임 갯수입니다. 최소3 최대 10개의 프레임 선택 가능\n");
            return 0;
        }

        //데이터 입력 방식 선택하는 부분
        printf("C. 데이터를 입력 방식을 선택하시오. (1,2)\n");
        printf("(1) 랜덤하게 생성\n");
        printf("(2) 사용자 생성 파일 오픈\n");
        scanf("%d", &inputDataMethod);
        if(inputDataMethod==1){
            randomNumberMaker();
        }
        else if(inputDataMethod==2){
            char splitPageReplacement[PAGESTRINGNUMBER]; 
            char tmpPage[PAGESTRINGNUMBER]; 
            char inttmp[PAGESTRINGNUMBER];
            char tmp[PAGESTRINGNUMBER];
            int num=0;
            printf("사용자 생성 파일명 입력 : ");
            scanf("%s", splitPageReplacement);
            FILE *fp = fopen(splitPageReplacement,"r");
            if(fp == NULL) {
                    printf("파일이 없습니다. 프로그램을 종료합니다.\n");
                    return 0;
            }

            for(int i=0; i<PAGESTRINGNUMBER; i++){
                fscanf(fp,"%d",&num);
                pageString[i] = num;
            }
            fclose(fp);          
        }
        else{
            fprintf(stderr, "데이터 입력 방식 1,2만 입력 가능\n");
            exit(1);
        }
        //페이지프레임 출력
        randomNumberPrint();
        //시뮬레이터 시작
        startSimulator(count,splitPageReplacement, inputPageFrame);

        //종료 선택하는 부분
        printf("D. 종료. (1,2)\n");
        printf("(1) 종료\n");
        printf("(2) 계속 진행\n");
        scanf("%d", &inputExit);
        if(inputExit==1){
            fclose(fp);
            return 0;
        }
        getchar();

    }
    return 0;
}

//입력값 seperator 로 분리해주는 함수
int split(char* string, char* seperator, char* argv[])		
{
	int argc = 0;
	char* ptr = NULL;
	ptr = strtok(string, seperator);
	while (ptr != NULL) {
		argv[argc++] = ptr;
		ptr = strtok(NULL, " ");
	}
    return argc;
}

//1~30까지의 랜덤 pageString을 만들어주는 함수
void randomNumberMaker(){
    srand((unsigned int)time(NULL));

    for (int i = 0; i<PAGESTRINGNUMBER; i++)
	{
		int random = rand() % 30+1;

        pageString[i] = random;
	}
}

//pageString 정보를 출력 하는 함수
void randomNumberPrint(){
    printf("pageString정보\n");
    for (int i = 0; i<PAGESTRINGNUMBER; i++)
	{
        printf("%d ",pageString[i]);
	}
    printf("\n");
}

//page replacement 알고리즘 컨트롤러
void startSimulator(int count, char **splitPageReplacement, int inputPageFrame){
    for(int i=0; i<count; i++){
        int num =0;
        num = atoi(splitPageReplacement[i]);
        switch(num){
            case 1:
                optimalSimulator(inputPageFrame);
                break;
            case 2: 
                fifoSimulator(inputPageFrame);
                break;
            case 3:
                lifoSimulator(inputPageFrame);
                break;
            case 4:
                lruSimulator(inputPageFrame);
                break;
            case 5:
                lfuSimulator(inputPageFrame);
                break;
            case 6:
                scSimulator(inputPageFrame);
                break;
            case 7:
                escSimulator(inputPageFrame);
                break;
            case 8:
                optimalSimulator(inputPageFrame);
                fifoSimulator(inputPageFrame);
                lifoSimulator(inputPageFrame);
                lruSimulator(inputPageFrame);
                lfuSimulator(inputPageFrame);
                scSimulator(inputPageFrame);
                escSimulator(inputPageFrame);
                break;
        }
    }
}

//optimal 알고리즘
void optimalSimulator(int inputPageFrame){
    printf("Optimal algorithm\n");
    fputs("Optimal algorithm\n",fp);

    int pageFrame[inputPageFrame];
    int fifoHistory[32];
    int fault = 0;
    int filled = 0;
    int index = 0;
    //pageframe 초기화
    for(int i=0; i<inputPageFrame; i++){
        pageFrame[i]=-1;
    }
    //pageString 삽입 부
    for(int i=0; i<PAGESTRINGNUMBER; i++){
        int pageFaultTmp = fault;
        int nextValue = (int)pageString[i];
        int tmp=0;
        //pageframe에 원하는 page가 있는지 확인하는 부분
        if(checkIn(pageFrame, nextValue)){
            printf("%2d  ",nextValue);
            fprintf(fp,"%2d  ",nextValue);
            printf("pageFrame: ");
            fputs("pageFrame: ",fp);
            for(int i=0; i<inputPageFrame; i++){
                if(pageFrame[i]==-1){
                    printf(" - ");
                    fputs(" - ",fp);
                    continue;
                }
                printf("%2d ",pageFrame[i]);
                fprintf(fp,"%2d ",pageFrame[i]);
            } 
            printf("\n");
            fputs("\n",fp);
            continue;
        }
        //filled는 페이지 프레임이 가득 차있을때 안차있다면 else 실행
        if(filled == inputPageFrame){
            //Optimal에서 가장 오랫동안 사용하지 않을 페이지 확인하는함수
            tmp = checkClose(pageFrame,i,fifoHistory);
            fifoHistory[nextValue]=i;
            pageFrame[tmp] = nextValue;
            fault++;
        }
        else{
            //프레임이 안차있을때 실행하는 부분
            pageFrame[index++] = nextValue;
            fifoHistory[nextValue]=i;
            filled++;
            fault++;
        }
        //페이지 프레임 변화 출력 부
        printf("%2d  ",nextValue);
        fprintf(fp,"%2d  ",nextValue);
        printf("pageFrame: ");
        fputs("pageFrame: ",fp);
        for(int i=0; i<inputPageFrame; i++){
            if(pageFrame[i]==-1){
                printf(" - ");
                fputs(" - ",fp);
                continue;
            }
            printf("%2d ",pageFrame[i]);
            fprintf(fp,"%2d ",pageFrame[i]);
        } 
        if(pageFaultTmp != fault){
            printf(" PageFault ");
            fputs(" PageFault ",fp);
        }
        printf("\n");
        fputs("\n",fp);
    }
    //페이지 프레임 fault 출력 부
    printf("pagefault of Optimal: %d\n",fault);
    fprintf(fp,"pagefault of Optimal: %d\n",fault);
}   

//페이지 내에 nextValue가 있는지 확인해주는 함수
int checkIn(int *pageFrame, int nextValue){
    int i=0;
    for(int i=0; i<inputPageFrame; i++){
        if(pageFrame[i] == nextValue && pageFrame[i] != -1){
            return 1;
        }
    }
    return 0;
}

//Optimal에서 가장 오랫동안 사용하지 않을 페이지 확인하는 함수
int checkClose(int *pageFrame,int currentLocation, int *fifoHistory){
    int counter=0;
    int max=0;
    int lifoIndex=0;
    int min=INT_MAX;
    int indexTmp=0;
    int tmp=0;
    for(int i=0; i<inputPageFrame; i++){
        int flag=0;
        int nothave=0;
        int pf = pageFrame[i];

        //현위치 다음 pagestring부터 오랫동안 사용하지 않을 페이지 확인
        for(int j=currentLocation; j<PAGESTRINGNUMBER; j++){
            //앞으로 한번도 사용하지 않을 때
            //fifo 순으로
            if(flag==0 && j==(PAGESTRINGNUMBER-1)){
                tmp=1;
                counter = i;
                if(min>fifoHistory[pf]){
                    min = fifoHistory[pf];
                    indexTmp = i;
                }
            }
            //중간에 page 사용할때 제일 늦게 나오는 page 찾기
            if(pf == (int)pageString[j]){
                flag=1;
                if(j<max){
                    break;
                }
                counter = i;
                max = j;
                break;
            }
        }
    }
    if(tmp==1){
        return indexTmp;
    }
    lifoIndex=counter;
    return counter;
}

//fifo 알고리즘
void fifoSimulator(int inputPageFrame){
    printf("FIFO algorithm\n");
    fputs("FIFO algorithm\n",fp);
    int pageFrame[inputPageFrame];
    int fifoHistory[PAGESTRINGNUMBER];
    int fault = 0;
    int filled = 0;
    int index = 0;
    int tmp=0;
    //페이지프레임 정보 초기화
    for(int i=0; i<inputPageFrame; i++){
        pageFrame[i]=-1;
    }
    //지금까지 페이지 정보 초기화
    for(int i=0; i<PAGESTRINGNUMBER; i++){
        fifoHistory[i]=0;   
    }
    //pageString 삽입 부
    for(int i=0; i<PAGESTRINGNUMBER; i++){
        int pageFaultTmp = fault;
        int nextValue = (int)pageString[i];
        int min=INT_MAX;
        int indexTmp=0;
        //pageframe에 원하는 pagestring이 있는지 확인하는 부분
        if(checkIn(pageFrame, nextValue)){
            printf("%2d  ",nextValue);
            fprintf(fp,"%2d  ",nextValue);
            printf("pageFrame: ");
            fputs("pageFrame: ",fp);
            for(int i=0; i<inputPageFrame; i++){
                if(pageFrame[i]==-1){
                    printf(" - ");
                    fputs(" - ",fp);
                    continue;
                }
                printf("%2d ",pageFrame[i]);
                fprintf(fp,"%2d ",pageFrame[i]);
            } 
            printf("\n");
            fputs("\n",fp);
            continue;
        }
        //filled는 페이지 프레임이 가득 차있을때 안차있다면 else 실행
        if(filled == inputPageFrame){  
            //fifoHistory를 보면서 가장 먼저 들어온 page 찾기
            for(int k=0; k<inputPageFrame; k++){
                int tmp = pageFrame[k];
                if(min>fifoHistory[tmp]){
                    min = fifoHistory[tmp];
                    indexTmp = k;
                }
            }
            //페이지 프레임에 삽입하는 부분
            fifoHistory[nextValue] = i;
            pageFrame[indexTmp] = nextValue;
            fault++;
        }
        else{
            //프레임이 안차있을때 실행하는 부분
            pageFrame[index++] = nextValue;
            fifoHistory[nextValue]=i;
            filled++;
            fault++;
        }
        //페이지 프레임 출력 부
        printf("%2d  ",nextValue);
        fprintf(fp,"%2d  ",nextValue);
        printf("pageFrame: ");
        fputs("pageFrame: ",fp);
        for(int i=0; i<inputPageFrame; i++){
            if(pageFrame[i]==-1){
                printf(" - ");
                fputs(" - ",fp);
                continue;
            }
            printf("%2d ",pageFrame[i]);
            fprintf(fp,"%2d ",pageFrame[i]);
        } 
        if(pageFaultTmp != fault){
            printf(" PageFault ");
            fputs(" PageFault ",fp);
        }
        printf("\n");
        fputs("\n",fp);
    }
    //페이지 프레임 fault 출력 부
    printf("pagefault of FIFO: %d\n",fault);
    fprintf(fp,"pagefault of FIFO: %d\n",fault);
}

//LIFO 알고리즘
void lifoSimulator(int inputPageFrame){
    printf("LIFO algorithm\n");
    fputs("LIFO algorithm\n",fp);
    int pageFrame[inputPageFrame];
    int fault = 0;
    int filled = 0;
    int index = 0;
    int tmp=0;
    //페이지프레임 정보 초기화
    for(int i=0; i<inputPageFrame; i++){
        pageFrame[i]=-1;
    }
    //pageString 삽입 부
    for(int i=0; i<PAGESTRINGNUMBER; i++){
        int pageFaultTmp = fault;
        int nextValue = (int)pageString[i];
        int max=0;
        int indexTmp=0;
        //pageframe에 원하는 pageString값 있는지 확인하는 부분
         if(checkIn(pageFrame, nextValue)){
            printf("%2d  ",nextValue);
            fprintf(fp,"%2d  ",nextValue);
            printf("pageFrame: ");
            fputs("pageFrame: ",fp);
            for(int i=0; i<inputPageFrame; i++){
                if(pageFrame[i]==-1){
                    printf(" - ");
                    fputs(" - ",fp);
                    continue;
                }
                printf("%2d ",pageFrame[i]);
                fprintf(fp,"%2d ",pageFrame[i]);
            } 
            printf("\n");
            fputs("\n",fp);
            continue;
        }
        //filled는 페이지 프레임이 가득 차있을때 안차있다면 else 실행
        if(filled == inputPageFrame){  
            //가장 마지막 프레임에 삽입
            pageFrame[filled-1] = nextValue;
            fault++;
        }
        else{
            //프레임이 안차있을때 실행하는 부분
            pageFrame[index++] = nextValue;
            filled++;
            fault++;
        }

        //페이지프레임 결과 출력
        printf("%2d  ",nextValue);
        fprintf(fp,"%2d  ",nextValue);
        printf("pageFrame: ");
        fputs("pageFrame: ",fp);
        for(int i=0; i<inputPageFrame; i++){
            if(pageFrame[i]==-1){
                printf(" - ");
                fputs(" - ",fp);
                continue;
            }
            printf("%2d ",pageFrame[i]);
            fprintf(fp,"%2d ",pageFrame[i]);
        } 
        if(pageFaultTmp != fault){
            printf(" PageFault ");
            fputs(" PageFault ",fp);
        }
        printf("\n");
        fputs("\n",fp);
    }
    //페이지 프레임 fault 출력 부
    printf("pagefault of LIFO: %d\n",fault);
    fprintf(fp,"pagefault of LIFO: %d\n",fault);
}

void lruSimulator(int inputPageFrame){
    printf("LRU algorithm\n");
    fputs("LRU algorithm\n",fp);
    int pageFrame[inputPageFrame];
    int lruHistory[PAGESTRINGNUMBER];
    int fault = 0;
    int filled = 0;
    int index = 0;
    int tmp=0;
    //페이지프레임 정보 초기화
    for(int i=0; i<inputPageFrame; i++){
        pageFrame[i]=-1;
    }
    //lru기록 초기화
    for(int i=0; i<PAGESTRINGNUMBER; i++){
        lruHistory[i]=0;
    }
    //pageString 삽입부
    for(int i=0; i<PAGESTRINGNUMBER; i++){
        int pageFaultTmp =fault;
        int nextValue = (int)pageString[i];
        int min=INT_MAX;
        int indexTmp=0;
        //pageframe에 원하는 page가 있는지 확인하는 부분
        if(checkIn(pageFrame, nextValue)){
            //hit일 경우 nextValue의 최근기록 i를 저장
            lruHistory[nextValue] = i;

            printf("%2d  ",nextValue);
            fprintf(fp,"%2d  ",nextValue);
            printf("pageFrame: ");
            fputs("pageFrame: ",fp);
            for(int i=0; i<inputPageFrame; i++){
                if(pageFrame[i]==-1){
                    printf(" - ");
                    fputs(" - ",fp);
                    continue;
                }
                printf("%2d ",pageFrame[i]);
                fprintf(fp,"%2d ",pageFrame[i]);
            } 
            printf("\n");
            fputs("\n",fp);
            continue;
        }
        //filled는 페이지 프레임이 가득 차있을때 안차있다면 else 실행
        if(filled == inputPageFrame){  
            //lruHistory 보면서 가장 먼저 들어온 page 찾기
            for(int k=0; k<inputPageFrame; k++){
                int tmp = pageFrame[k];
                if(min>lruHistory[tmp]){
                    min = lruHistory[tmp];
                    indexTmp = k;
                }
            }
            //페이지에 삽입 및 nextValue의 최근기록 i를 저장
            lruHistory[nextValue] = i;
            pageFrame[indexTmp] = nextValue;
            fault++;
        }
        else{
            //프레임이 안차있을때 실행하는 부분
            pageFrame[index++] = nextValue;
            lruHistory[nextValue]=i;
            filled++;
            fault++;
        }

        //페이지프레임 출력 부
        printf("%2d  ",nextValue);
        fprintf(fp,"%2d  ",nextValue);
        printf("pageFrame: ");
        fputs("pageFrame: ",fp);
        for(int i=0; i<inputPageFrame; i++){
            if(pageFrame[i]==-1){
                printf(" - ");
                fputs(" - ",fp);
                continue;
            }
            printf("%2d ",pageFrame[i]);
            fprintf(fp,"%2d ",pageFrame[i]);
        } 
        if(pageFaultTmp != fault){
            printf(" PageFault ");
            fputs(" PageFault ",fp);
        }
        printf("\n");
        fputs("\n",fp);
    }
    //페이지 프레임 fault 출력 부
    printf("pagefault of LRU: %d\n",fault);
    fprintf(fp,"pagefault of LRU: %d\n",fault);
}

void lfuSimulator(int inputPageFrame){
    printf("LFU algorithm\n");
    fputs("LFU algorithm\n",fp);
    int pageFrame[inputPageFrame];
    int lfuHistory[PAGESTRINGNUMBER];
    int lruHistory[PAGESTRINGNUMBER];
    int fault = 0;
    int filled = 0;
    int index = 0;
    int tmp=0;
    
    //페이지프레임 정보 초기화
    for(int i=0; i<inputPageFrame; i++){
        pageFrame[i]=-1;
    }
    //lfu, lru 기록 배열 초기화
    for(int i=0; i<PAGESTRINGNUMBER; i++){
        lfuHistory[i]=-1;
        lruHistory[i]=-1;
    }
    for(int i=0; i<PAGESTRINGNUMBER; i++){
        int pageFaultTmp = fault;
        int nextValue = (int)pageString[i];
        int min=INT_MAX;
        int indexTmp=0;
        int passlru=0;
        //pageframe에 원하는 pageSTRING값이 있는지 확인하는 부분
        if(checkIn(pageFrame, nextValue)){
            //hit일 경우 nextValue에 해당하는 lfu기록을 증가시키고 lru기록에는 최근위치인 i 삽입
            lfuHistory[nextValue]++;
            lruHistory[nextValue] = i;
            printf("%2d  ",nextValue);
            fprintf(fp,"%2d  ",nextValue);
             printf("pageFrame: ");
            fputs("pageFrame: ",fp);
            for(int i=0; i<inputPageFrame; i++){
                if(pageFrame[i]==-1){
                    printf(" - ");
                    fputs(" - ",fp);
                    continue;
                }
                printf("%2d ",pageFrame[i]);
                fprintf(fp,"%2d ",pageFrame[i]);
            } 
            printf("\n");
            fputs("\n",fp);
            continue;
        }
        //filled는 페이지 프레임이 가득 차있을때 안차있다면 else 실행
        if(filled == inputPageFrame){
            int lruChecker = 0;
            //lfuHistory 보면서 가장 적게 들어온 page 찾기
            for(int k=0; k<inputPageFrame; k++){
                int tmp = pageFrame[k];
                if(min>lfuHistory[tmp]){
                    min = lfuHistory[tmp];
                    indexTmp = k;
                }
            }
            //lfu 숫자가 같아 교체대상이 여러개일때 확인
            for(int k=0; k<inputPageFrame; k++){
                int tmp = pageFrame[k];
                if(min==lfuHistory[tmp] && k!=indexTmp){
                    lruChecker=1;
                    //lru 적용할 값을 갖는 값
                    passlru=min;
                }
            }
            //교체대상이 여러개라면 lru적용해서 교환
            if(lruChecker==1){
                min = INT_MAX;
                for(int k=0; k<inputPageFrame; k++){
                    int tmp = pageFrame[k];
                    if(min>lruHistory[tmp] && lfuHistory[tmp]==passlru){
                        min = lruHistory[tmp];
                        indexTmp = k;
                    }
                }
                lruChecker=0;
            }
            //nextValue에 해당하는 lfu기록을 증가시키고 lru기록에는 최근위치인 i 삽입 그리고 페이지폴트
            lfuHistory[nextValue]++;
            lruHistory[nextValue] = i;
            pageFrame[indexTmp] = nextValue;
            fault++;
        }
        else{
            //프레임이 안차있을때 실행하는 부분
            pageFrame[index++] = nextValue;
            lfuHistory[nextValue]++;
            lruHistory[nextValue] = i;
            filled++;
            fault++;
        }

        //페이지프레임 출력 부
        printf("%2d  ",nextValue);
        fprintf(fp,"%2d  ",nextValue);
        printf("pageFrame: ");
        fputs("pageFrame: ",fp);
        for(int i=0; i<inputPageFrame; i++){
            if(pageFrame[i]==-1){
                printf(" - ");
                fputs(" - ",fp);
                continue;
            }
            printf("%2d ",pageFrame[i]);
            fprintf(fp,"%2d ",pageFrame[i]);
        } 
        if(pageFaultTmp != fault){
            printf(" PageFault ");
            fputs(" PageFault ",fp);
        }
        printf("\n");
        fputs("\n",fp);
    }
    //페이지 프레임 fault 출력 부
    printf("pagefault of LFU: %d\n",fault);
    fprintf(fp,"pagefault of LFU: %d\n",fault);
}

void scSimulator(int inputPageFrame){
    printf("SC algorithm\n");
    fputs("SC algorithm\n",fp);
    int pageFrame[inputPageFrame];
    int referenceHistory[PAGESTRINGNUMBER];
    int fault = 0;
    int filled = 0;
    int index = 0;
    int tmp=0;
    int flag=0;
    int pointer=0;
    //페이지프레임 정보 초기화
    for(int i=0; i<inputPageFrame; i++){
        pageFrame[i]=-1;
    }
    //reference 저장 배열 초기화
    for(int i=0; i<PAGESTRINGNUMBER; i++){
        referenceHistory[i]=0;
    }
    //pageString에 맞게 삽입
    for(int i=0; i<PAGESTRINGNUMBER; i++){
        int pageFaultTmp = fault;
        int nextValue = (int)pageString[i];
        int min=INT_MAX;
        int indexTmp=0;
        //pageframe에 원하는 page스트링값 있는지 확인하는 부분
        if(checkIn(pageFrame, nextValue)){
            //참조비트가 0일경우 secondchance 줌 레퍼런스비트 1로바꿈
            if(referenceHistory[nextValue]==0)
                referenceHistory[nextValue]=1;

            printf("%2d  ",nextValue);
            fprintf(fp,"%2d  ",nextValue);
            printf("pageFrame: ");
            fputs("pageFrame: ",fp);
            for(int i=0; i<inputPageFrame; i++){
                if(pageFrame[i]==-1){
                    printf(" ---- ");
                    fputs(" ---- ",fp);
                    continue;
                }
                printf("%2d(%d) ",pageFrame[i],referenceHistory[pageFrame[i]]);
                fprintf(fp,"%2d(%d) ",pageFrame[i],referenceHistory[pageFrame[i]]);
            } 

            printf("\n");
            fputs("\n",fp);
            continue;
        }
        //pageFrame이 가득 차 있을 경우
        if(filled == inputPageFrame){  
            //circular queue 방식
            for(int k=pointer; k<pointer+inputPageFrame; k++){
                int ktmp=k;
                //inputPageFrame의 나머지 값만 받아옴
                ktmp %= inputPageFrame;
                int tmp = pageFrame[ktmp];
                //reference bit가 0 일경우 값을 넣어준다. 그후 포인터를 옮겨준다.
                if(referenceHistory[tmp]==0){
                    pageFrame[ktmp] = nextValue;
                    pointer++;
                    break;
                }
                //reference bit가 1일경우 다음으로 포인터를 옮기고 0으로 바꿔준다.
                else if(referenceHistory[tmp]==1){
                    pointer++;
                    referenceHistory[tmp]=0;
                    continue;
                }
            }
            //가리키고 있는 포인터를 module 연산을 통해서 계산함
            fault++;
            pointer = pointer%inputPageFrame;
        }
        else{
            //프레임이 안차있을때 실행하는 부분
            pageFrame[index++] = nextValue;
            referenceHistory[nextValue]=0;
            filled++;
            fault++;
            pointer++;
            pointer = pointer%inputPageFrame;
        }
        //페이지프레임 결과 출력 부
        printf("%2d  ",nextValue);
        fprintf(fp,"%2d  ",nextValue);
        printf("pageFrame: ");
        fputs("pageFrame: ",fp);
        for(int i=0; i<inputPageFrame; i++){
            if(pageFrame[i]==-1){
                printf(" ---- ");
                fputs(" ---- ",fp);
                continue;
            }
            printf("%2d(%d) ",pageFrame[i],referenceHistory[pageFrame[i]]);
            fprintf(fp,"%2d(%d) ",pageFrame[i],referenceHistory[pageFrame[i]]);
        } 

        if(pageFaultTmp != fault){
        printf(" PageFault ");
        fputs(" PageFault ",fp);
        }
        printf("\n");
        fputs("\n",fp);
    }
    //페이지 폴트 결과 출력부
    printf("pagefault of SC: %d\n",fault);
    fprintf(fp,"pagefault of SC: %d\n",fault);
}

//wbit 랜덤생성을 위한 함수
void randomNumberBitMaker(){
    srand((unsigned int)time(NULL));
    for (int i = 0; i<PAGESTRINGNUMBER; i++)
	{
		int random = rand() % 2;
        wbitString[i] = random;
	}
}

//Rbit 랜덤생성을 위한 함수
void randomNumberRBitMaker(){
    srand((unsigned int)time(NULL));
    for (int i = 0; i<PAGESTRINGNUMBER; i++)
	{
		int random = rand() % 2;
        rbitString[i] = random;
	}
}

void escSimulator(int inputPageFrame){
    printf("ESC algorithm\n");
    fputs("ESC algorithm\n",fp);
    int pageFrame[inputPageFrame];
    int referenceHistory[PAGESTRINGNUMBER];
    int writeHistory[PAGESTRINGNUMBER];
    int fault = 0;
    int filled = 0;
    int index = 0;
    int tmp=0;
    int flag=0;
    int pointer=0;
    int inputDataMethod=0;
    
    //R비트 랜덤생성 OR 사용자생성파일 오픈으로 받음
    printf("ESC R bit 입력 방식을 선택하시오. (1,2)\n");
    printf("(1) 랜덤하게 생성\n");
    printf("(2) 사용자 생성 파일 오픈\n");
    scanf("%d", &inputDataMethod);
    if(inputDataMethod==1){
        randomNumberRBitMaker();
    }
    else if(inputDataMethod==2){
        char splitPageReplacement[PAGESTRINGNUMBER]; 
        char tmpPage[PAGESTRINGNUMBER]; 
        char inttmp[PAGESTRINGNUMBER];
        char tmp[PAGESTRINGNUMBER];
        int num=0;
        printf("사용자 생성 파일명 입력 : ");
        scanf("%s", splitPageReplacement);
        FILE *fp = fopen(splitPageReplacement,"r");
        if(fp == NULL) {
                printf("파일이 없습니다. 프로그램을 종료합니다.\n");
                exit(1);
        }

        for(int i=0; i<PAGESTRINGNUMBER; i++){
            fscanf(fp,"%d",&num);
            rbitString[i] = num;
        }

        fclose(fp);          
    }
    else{
        fprintf(stderr, "입력방식 에러!\n");
        exit(1);
    }

    //W비트 랜덤생성 OR 사용자생성파일 오픈으로 받음
    printf("ESC W bit 입력 방식을 선택하시오. (1,2)\n");
    printf("(1) 랜덤하게 생성\n");
    printf("(2) 사용자 생성 파일 오픈\n");
    scanf("%d", &inputDataMethod);
    if(inputDataMethod==1){
        randomNumberBitMaker();
    }
    else if(inputDataMethod==2){
        char splitPageReplacement[PAGESTRINGNUMBER]; 
        char tmpPage[PAGESTRINGNUMBER]; 
        char inttmp[PAGESTRINGNUMBER];
        char tmp[PAGESTRINGNUMBER];
        int num=0;
        printf("사용자 생성 파일명 입력 : ");
        scanf("%s", splitPageReplacement);
        FILE *fp = fopen(splitPageReplacement,"r");
        if(fp == NULL) {
                printf("파일이 없습니다. 프로그램을 종료합니다.\n");
                exit(1);
        }

        for(int i=0; i<PAGESTRINGNUMBER; i++){
            fscanf(fp,"%d",&num);
            wbitString[i] = num;
        }

        fclose(fp);          
    }
    else{
        fprintf(stderr, "입력방식 에러!\n");
        exit(1);
    }

    //페이지 스트링 정보 및 r비트,w비트 출력
    printf("pageString정보\n");
    for(int i=0; i<PAGESTRINGNUMBER; i++){
        printf("%d(%d/%d) ",pageString[i],rbitString[i],wbitString[i]);
    }
    printf("\n");
    printf("\n");

    //페이지프레임 정보 초기화
    for(int i=0; i<inputPageFrame; i++){
        pageFrame[i]=-1;
    }

    //w비트 r비트 저장 배열 초기화
    for(int i=0; i<PAGESTRINGNUMBER; i++){
        writeHistory[i]=0;
        referenceHistory[i]=0;
    }
    int writeHistoryTmp=0;
    //pageString에 맞게 삽입하는 부
    for(int i=0; i<PAGESTRINGNUMBER; i++){
        int pageFaultTmp = fault;
        writeHistory[pageString[i]]=wbitString[i];
        referenceHistory[pageString[i]]=rbitString[i];
        int nextValue = (int)pageString[i];
        int min=INT_MAX;
        int indexTmp=0;
        //pageframe에 원하는 page가 있는지 확인하는 부분
        if(checkIn(pageFrame, nextValue)){
            //참조비트가 0일경우 secondchance 줌
            if(referenceHistory[nextValue]==0)
                referenceHistory[nextValue]=1;

            printf("%2d  ",nextValue);
            fprintf(fp,"%2d  ",nextValue);
            printf("pageFrame: ");
            fputs("pageFrame: ",fp);
            for(int i=0; i<inputPageFrame; i++){
                if(pageFrame[i]==-1){
                    printf(" -- -----");
                    fputs(" -- -----",fp);
                    continue;
                }
                printf(" %2d (%d/%d)",pageFrame[i],referenceHistory[pageFrame[i]],writeHistory[pageFrame[i]]);
                fprintf(fp," %2d (%d/%d)",pageFrame[i],referenceHistory[pageFrame[i]],writeHistory[pageFrame[i]]);
            } 
            printf("\n");
            fputs("\n",fp);
            continue;
        }
        //pageFrame이 가득 차 있을 경우
        if(filled == inputPageFrame){  
            flag=0;
            //R W 비트 0 0 일경우 탐색
            int tmpPointer = pointer;
            for(int k=tmpPointer; k<tmpPointer+inputPageFrame; k++){
                int ktmp=k;
                //inputPageFrame의 나머지 값만 받아옴
                ktmp %= inputPageFrame;
                int tmp = pageFrame[ktmp];
                //0 0 일 경우
                if(referenceHistory[tmp]==0 && writeHistory[tmp]==0){
                    pageFrame[ktmp] = nextValue;
                    referenceHistory[nextValue]=1;
                    pointer++;
                    flag=1;
                    break;
                }
                pointer++;
            }
            //0 1 일 경우 포인터를 움직이면서 R비트를 0으로 만들어준다.
            tmpPointer = pointer;
            //FLAG==0 은 0 0 비트가 없었음을 의미
            if(flag==0)
            for(int k=tmpPointer; k<tmpPointer+inputPageFrame; k++){
                int ktmp=k;
                //inputPageFrame의 나머지 값만 받아옴
                ktmp %= inputPageFrame;
                int tmp = pageFrame[ktmp];
                if(referenceHistory[tmp]==0 && writeHistory[tmp]==1){
                    pageFrame[ktmp] = nextValue;
                    referenceHistory[nextValue]=1;
                    pointer++;
                    flag=2;
                    break;
                }
                else{
                    //0 1 이 없을 경우에 POINTER움직이며 R비트 0으로만들어줌
                    referenceHistory[tmp]=0;
                    pointer++;
                }
            }
            //FLAG가 0이라는 것은 0 0, 0 1 값이 없었음을 의미한다.
            //R비트가 0으로 만들어진 상태에서 다시 0 0, 0 1탐색
            if(flag==0){
                 i--;
                 continue;
            }

            //가리키고 있는 포인터를 module 연산을 통해서 계산함
            fault++;
            pointer = pointer%inputPageFrame;
        
        }
        else{
            //프레임이 안차있을때 실행하는 부분
            pageFrame[index++] = nextValue;
            referenceHistory[nextValue]=1;
            filled++;
            fault++;
            pointer++;
        }
        pointer = pointer%inputPageFrame;
        //페이지프레임 출력 부
        printf("%2d  ",nextValue);
        fprintf(fp,"%2d  ",nextValue);
        printf("pageFrame: ");
        fputs("pageFrame: ",fp);
        for(int i=0; i<inputPageFrame; i++){
            if(pageFrame[i]==-1){
                printf(" -- -----");
                fputs(" -- -----",fp);
                continue;
            }
            printf(" %2d (%d/%d)",pageFrame[i],referenceHistory[pageFrame[i]],writeHistory[pageFrame[i]]);
            fprintf(fp," %2d (%d/%d)",pageFrame[i],referenceHistory[pageFrame[i]],writeHistory[pageFrame[i]]);
        } 

        if(pageFaultTmp != fault){
            printf(" PageFault ");
            fputs(" PageFault ",fp);
        }
        printf("\n");
        fputs("\n",fp);
    }
    //페이지폴트 출력부
    printf("pagefault of ESC: %d\n",fault);
    fprintf(fp,"page Fault of ESC: %d\n",fault);
}