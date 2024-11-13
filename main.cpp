#include <iostream>        // 표준 입출력 라이브러리 (cout, cin 등을 사용하기 위해 필요)
#include <unistd.h>        // usleep() 함수를 사용하기 위한 헤더
#include <sys/time.h>      // gettimeofday() 함수를 사용하여 시간을 측정하기 위한 헤더
#include <signal.h>        // 시그널을 처리하기 위한 헤더
#include "dxl.hpp"         // Dynamixel 모터 제어를 위한 사용자 정의 헤더 파일
using namespace std;       // 표준 네임스페이스 사용 (std::cout, std::cin 등을 간편하게 사용하기 위해)
 
bool ctrl_c_pressed = false;  // Ctrl+C가 눌렸는지 여부를 나타내는 플래그 변수
 
// Ctrl+C 시그널이 발생했을 때 호출되는 함수
void ctrlc(int) {
    ctrl_c_pressed = true;  // ctrl_c_pressed 값을 true로 설정하여 프로그램 종료를 유도
}

int main(void)
{
    Dxl mx;  // Dxl 객체 생성, 이 객체는 Dynamixel 모터를 제어하는 클래스의 인스턴스입니다.
    struct timeval start, end1;  // 시간 측정을 위한 timeval 구조체 선언
    int leftspeed, rightspeed;   // 왼쪽, 오른쪽 모터의 속도를 저장할 변수
    double time1;  // 각 루프의 실행 시간을 저장할 변수
 
    // SIGINT 시그널(일반적으로 Ctrl+C) 발생 시 ctrlc 함수를 호출하도록 설정
    signal(SIGINT, ctrlc); 			

    // Dynamixel 모터와 연결을 시도하고 성공하면 true를 반환
    if (!mx.open()) { 
        cout << "dynamixel open error" << endl;  // 연결 실패 시 오류 메시지 출력
        return -1;  // 오류 발생 시 프로그램 종료
    }

    // 무한 루프 시작
    while (true)
    {
        // 루프 시작 시의 시간을 기록 (초 단위와 마이크로초 단위로)
        gettimeofday(&start, NULL); 
 
        // 사용자로부터 왼쪽 모터 속도 입력 받기
        cout << "left speed:";
        cin >> leftspeed;
        
        // 사용자로부터 오른쪽 모터 속도 입력 받기
        cout << "right speed:";
        cin >> rightspeed;

        // 입력받은 속도로 모터의 속도를 설정
        mx.setVelocity(leftspeed, rightspeed);

        // 20밀리초 대기 (모터 명령 후 빠르게 반복하지 않도록 잠시 멈춤)
        usleep(20 * 1000);   // 20ms sleep (20 * 1000 마이크로초)
        
        // Ctrl+C가 눌렸으면 루프를 종료
        if (ctrl_c_pressed) break;		

        // 루프가 끝난 후, 종료 시각을 기록
        gettimeofday(&end1, NULL); 

        // 시작 시간과 종료 시간을 이용해 실행 시간을 계산
        time1 = end1.tv_sec - start.tv_sec + (end1.tv_usec - start.tv_usec) / 1000000.0;

        // 실행 시간을 출력 (초 단위)
        cout << "time:" << time1 << " sec" << endl;
    }

    // 프로그램 종료 전에 Dynamixel 모터와의 연결을 종료
    mx.close();  // 모터 장치 닫기

    return 0;  // 프로그램 정상 종료
}