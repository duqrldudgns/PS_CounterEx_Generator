#include <bits/stdc++.h>
#include "userSetInput.hpp"

using namespace std;

extern int boj;

void Init(){
    // 두 코드 컴파일
    int ret;
    ret = system("g++ ./code/correct.cpp -o ./code/correct -O2 -lm -static");
    if(ret == -1 ){
        cout << "Correct compile Error\n";
        exit(1);
    }
    ret = system("g++ ./code/incorrect.cpp -o ./code/incorrect -O2 -lm -static");
    if(ret == -1 ){
        cout << "Incorrect compile Error\n";
        exit(1);
    }
}

int main(){

    Init();

    // Timer On, 걸린 시간 체크
    time_t start, finish;
    start = time(NULL); 
 
    while(true){
        // 랜덤 입력 값 받아오고 파일에 저장
        string seed = SetInput();

        ofstream inputFile;
        inputFile.open("input.txt");    //파일 열기(없으면 생성)

        inputFile << seed << "\n";

        inputFile.close();


        // 입력값을 넣고, 출력값을 파일로 저장
        // 속도 제일 많이 잡아먹는 구간, 0.03초걸림
        int ret;
        ret = system("cmd /c \".\\code\\correct < input.txt > .\\code\\correctOutput.txt\"");
        if(ret == -1 ){
            cout << "CorrectOutput create Error\n";
            exit(1);
        }
        ret = system("cmd /c \".\\code\\incorrect < input.txt > .\\code\\incorrectOutput.txt\"");
        if(ret == -1 ){
            cout << "CorrectOutput create Error\n";
            exit(1);
        }
        

        // 두 출력값 파일을 받아와 비교
        ifstream outputFile1, outputFile2;
        outputFile1.open("./code/correctOutput.txt");  
        outputFile2.open("./code/incorrectOutput.txt");

        bool diff = false;
        vector<string> answer, wrong;
        if (outputFile1.is_open() && outputFile2.is_open())
        {
            string line;
            while(!outputFile1.eof()){
                getline(outputFile1, line);
                answer.push_back(line);
            }
            while(!outputFile2.eof()){
                getline(outputFile2, line);
                wrong.push_back(line);
            }
            outputFile1.close();    
            outputFile2.close();
            
            if(answer.size() != wrong.size()){          // 출력값 라인 길이가 다를 경우
                diff = true;
            }
            else{
                for(int i=0; i<answer.size(); ++i){
                    if(answer[i] == wrong[i]) continue; // 라인 내의 출력값이 다를 경우
                    diff = true;
                    break;
                }
            }
        }


        // 두 출력값이 다른 경우
        if(diff){
            // Timer Off
            finish  = time(NULL);     
            auto duration = finish - start;
            struct tm* t = localtime(&finish);
            string nowtime;
            nowtime = '[' + to_string(t->tm_year + 1900) + '.' + to_string(t->tm_mon + 1) + '.' + to_string(t->tm_mday) + '/'
                    + to_string(t->tm_hour) + ':' + to_string(t->tm_min) + ':' + to_string(t->tm_sec) + ']';

            // logStr 세팅
            // 백준문제 번호, 날짜 및 시간, 걸린 시간
            string logStr = "------------------------------\n";   
            logStr += "boj - " + to_string(boj) + '\n';
            logStr += nowtime + '\t';
            logStr += to_string(duration) + "sec\n";

            logStr += "\n[input]\n" + seed;
            logStr += "\n\n[answer]\n";
            for(auto a : answer) logStr += a + "\n";
            logStr += "\n[wrong]\n";
            for(auto w : wrong) logStr += w + "\n";

            // 커맨드 출력
            cout << logStr;

            // 로그 남기기
            ofstream logFile;
            logFile.open("log.txt", ios::app);  // 이어쓰기

            logFile << logStr;

            logFile.close();
            break;
        }
    }
}