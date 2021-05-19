# KPU-NetP-TP

학교 리눅스 서버에서 바로 실행하는 방법
1. 초록색 code버튼 클릭
2. 복사버튼 클릭
3. 리눅스 서버에서
4. git clone (오른쪽마우스 눌러서 url 붙여넣기)
 
# 210518 업데이트 - 서버,클라이언트 파일 v0.a
컴파일은 $make 로. (-p_thread, -D_REENTRANT 플래그 사용해야 하는데 make 실행하면 바로 됨)

"1"을 입력하면 "selected case 1" 출력. 다른 건 "default" 출력.

이상.

# 210519 업데이트 - 서버,클라이언트 파일 v0.b
1. "1" 입력 - 읽기(read.c) 실행
2. "2" 입력 - 추가(append.c) 
3. "3" 입력 - "selected case 3" 출력
4. "4" 입력 - "selected case 4" 출력
5. "0" 입력 - 클라이언트 종료
6. 서버 종료는 Ctrl+C

실행되면 서버, 클라이언트에 메세지 출력됨.

이상.



# 참고했던 자료들 
append, read codes from
https://www.geeksforgeeks.org/relational-database-from-csv-files-in-c/
