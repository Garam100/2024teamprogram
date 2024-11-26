#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int BookSearch();
void ShowbBorrowList();
void DeleteAccount();
void ResetInfo(client구조체);
void PrintSearchResult(book구조체링크);

int UserMenu(){
  int menu;
  while(1){
    printf(">> 회원 메뉴 <<\n");
    printf("1. 도서 검색  2. 내 대여 목록\n3. 프로그램 종료  4. 회원 탈퇴\n5. 로그아웃  6. 프로그램 종료\n\n");
    printf("번호를 선택하세요: ");
    scanf("%d", &menu);
    switch(menu){
      case 1:
        system("clear");
        BookSearch(book구조체링크);
        system("clear");
      break;
      
      case 2:
        system("clear");
        ShowbBorrowList(book구조체링크);
        system("clear");
      break;
      
      case 3:
        system("clear");
        DeleteAccount(client구조체);
        system("clear");
      break;
      
      case 4:
        system("clear");
        ResetInfo(client구조체);
        system("clear");
      break;
      
      case 5:
        printf("로그아웃합니다.");
        sleep(3);
        system("clear");
        return 0;
      break;
      
      case 6:
        printf("프로그램을 종료합니다.");
        sleep(3);
        system("clear");
        exit(1);
      break;
      
      default:
        printf("잘못된 번호입니다.\n");
      break;
  
    }
  }
}

//도서검색
int BookSearch(){
  int menu;
  while(1){
    printf(">> 도서 검색 <<\n");
    printf("1. 도서명 검색  2. 출판사 검색\n3.ISBN검색  4.저자명 검색\n5.전체 검색  6.이전 메뉴");
    printf("번호를 선택하세요: ");
    scanf("%d" , &menu);
    switch(menu){
      case 1:
        PrintSearchResult(book구조체링크);
        break;
  
      case 2:
        PrintSearchResult(book구조체링크);
        break;
  
      case 3:
        PrintSearchResult(book구조체링크);
        break;
  
      case 4:
        PrintSearchResult(book구조체링크);
        break;
  
      case 5:
        PrintSearchResult(book구조체링크);
        break;
  
      case 6:
        return 0;
        break;
      
      default:
        printf("잘못된 번호입니다.\n");
        break;
    }
  }
}
    

//검색 결과 함수
void PrintSearchResult(book구조체링크){
  printf(">> 검색 결과 <<\n");
  printf("도서명: \n");
  printf("출판사: \n");
  printf("저자명: \n");
  printf("ISBN: \n");
  printf("소장처: \n");
  printf("대여가능 여부:  \n");
  printf("** Y는 대여가능, N은 대여불가를 의미 \n");
  printf("** (x/y) : (대여된 총 권수 / 보유하고 있는 총 권수) \n\n");
} 
    
//내 대여 목록
void ShowbBorrowList(){
  printf(">> 내 대여 목록 <<\n");
  while(1){
    printf("도서 번호: \n");
    printf("도서명: \n");
    printf("대여 일자: \n");
    printf("반납 일자: \n\n");
  }
}
//개인정보 수정
void ResetInfo(client구조체){
  //구조체에서 해당 코드 찾기, 노드 삭제후 앞뒤 노드 연결, 수정된 구조체 파일에 입력하기
}
//회원 탈퇴
void DeleteAccount(client구조체){
  //구조체에서 해당 코드 찾기, 파일에 수정된 구조체 입력하기
}

