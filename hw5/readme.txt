Name: 최진성 Student ID: 20170699
Name: 윤석범 Student ID: 20170429
# of assignment: 5

EthicsOath(1): 최진성
EthicsOath(2): 윤석범

프로그램이 global variable을 굉장히 많이, 또 중요하게 사용해 파일을 하나만 만들었음.
Makefile 역시 ish.c로 build하였음

Work division:
최진성: 변수 및 구조 설정 및 구현,lexical analysis DFA, Syntactic analysis, .ishrc initialization
윤석범: lexical analysis coding, execute, pipe, signal handling
거의 대부분 같이 공유하며 지속적인 피드백, 위에 적은 것은 주도적으로 진행한 사람

Program:
Used DynArray ADT
Redirection 구현 안됨
command가 에러여서 에러가 한번 나왓을 경우, 그 다음 바로 exit이 되지 않고 한번 더 exit을 쳐야 종료됨(bug)
Signal handling에서 Ctrl-\입력후 'Type Ctrl-\  again within 5 seconds to exit'에서 다시 Ctrl-\를 입력하면 명령은 잘 받지만 각 줄마다 %가 처음에 출력되지 않음. 작동은 잘 이루어짐(bug)