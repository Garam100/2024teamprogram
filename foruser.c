#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//구조체
typedef struct{
  int studentId;
  char password[20];
  char name[20];
  char address[20];
  char phone[20];
  struct clientInfo *next;
}clientInfo;

typedef struct{
  int bookId;
  char title[20];
  char publisher[20];
  char author[20];
  int isbn;
  char location[20];
  char borrow;
  struct bookInfo *next;
}bookInfo;

typedef struct{
  int studentId;
  int bookId;
  time_t rentalDtate;
  time_t returnDate;
  struct rentalInfo *next;
}borrowInfo;

//link없앨수도..
typedef clientInfo *LINKc;
typedef bookInfo *LINKb;
typedef borrowInfo *LINKd;

//함수 선언
int UserMenu(LINKb, LINKc, LINKd);
int BookSearch(LINKb);
void ShowbBorrowList(LINKd);
void DeleteAccount(LINKc);
void ResetInfo(LINKc);
void PrintSearchResult(LINKb,int,int);
LINKb SearchBookid(LINKb, char *);
LINKb SearchAuthor(LINKb, char *);
LINKb SearchPublisher(LINKb, char *);
LINKb SearchISBN(LINKb, int);
void PrintfAllBook(LINKb);

//로그인 후 나오는 회원 메뉴
int UserMenu(LINKb bhead, LINKc chead, LINKd dhead){
  int menu;
  while(1){
    printf(">> 회원 메뉴 <<\n");
    printf("1. 도서 검색  2. 내 대여 목록\n3. 프로그램 종료  4. 회원 탈퇴\n5. 로그아웃  6. 프로그램 종료\n\n");
    printf("번호를 선택하세요: ");
    scanf("%d", &menu);
    switch(menu){
      case 1:
        system("clear");
        BookSearch(bhead);
        system("clear");
      break;
      
      case 2:
        system("clear");
        ShowbBorrowList(dhead);
        system("clear");
      break;
      
      case 3:
        system("clear");
        DeleteAccount(chead);
        system("clear");
      break;
      
      case 4:
        system("clear");
        ResetInfo(chead);
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

//도서검색**관리자 메뉴에도 넣어야함
int BookSearch(LINKb bhead){
  int menu;
  while(1){
    printf(">> 도서 검색 <<\n");
    printf("1. 도서명 검색  2. 출판사 검색\n3.ISBN검색  4.저자명 검색\n5.전체 검색  6.이전 메뉴");
    printf("번호를 선택하세요: ");
    scanf("%d" , &menu);
    int searchISBN;
    char searchWord[20];
    LINKb searchResult;
    switch(menu){
      case 1:
        printf("\n도서명을 입력하세요: ");
        scanf("%s", searchWord);
        searchResult = SearchBookid(bhead,searchWord);
        PrintSearchResult(searchResult,1,1);
        break;
  
      case 2:
        printf("\n출판사를 입력하세요: ");
        scanf("%s", searchWord);
        searchResult = SearchBookid(bhead,searchWord);
        PrintSearchResult(searchResult,1,1);
        break;
  
      case 3:
        printf("\nISBN을 입력하세요: ");
        scanf("%d", &searchISBN);
        searchResult = SearchISBN(bhead,searchISBN);
        PrintSearchResult(searchResult,1,1);
        break;
  
      case 4:
        printf("\n저자명을 입력하세요: ");
        scanf("%s", searchWord);
        searchResult = SearchAuthor(bhead,searchWord);
        PrintSearchResult(searchResult1,1);
        break;
  
      case 5:
        printf("\n전체 도서를 출력합니다.\n");
        PrintfAllBook(bhead);
        break;
  
      case 6:
        return 0;
        break;
      
      default:
        printf("잘못된 번호입니다.\n");
        break;
    }
    system("clear");
  }
}
    
//검색 함수: 도서명
LINKb SearchBookid(LINKb head, char *name){
  while(head!=NULL){
    if(strcmp(head->title,name)==0)
      return head;
    head = head->next;
  }
  return head;
}

//검색 함수: 저자명
LINKb SearchAuthor(LINKb head, char *name){
  while(head!=NULL){
    if(strcmp(head->author,name)==0)
      return head;
    head = head->next;
  }
  return head;
}

//검색 함수: 출판사명
LINKb SearchPublisher(LINKb head, char *name){
  while(head!=NULL){
    if(strcmp(head->publisher,name)==0)
      return head;
    head = head->next;
  }
  return head;
}

//검색 함수: ISBN
LINKb SearchISBN(LINKb head, int isbn){
  while(head!=NULL){
    if(head->isbn== isbn)
      return head;
    head = head->next;
  }
  return head;
}




//검색 결과 함수
void PrintSearch(LINKb searchResult, int a, int b){
  printf(">> 검색 결과 <<\n");
  printf("도서명: %s\n", searchResult->title);
  printf("출판사: %s\n", searchResult->publisher);
  printf("저자명: %s\n",searchResult->author);
  printf("ISBN: %d\n", searchResult-> isbn);
  printf("소장처: %s\n", searchResult->location);
  printf("대여가능 여부:  %c(%d/%d)\n", searchResult->borrow,);
  printf("** Y는 대여가능, N은 대여불가를 의미 \n");
  printf("** (x/y) : (대여된 총 권수 / 보유하고 있는 총 권수) \n\n");
} 

//모든 도서 출력
void PrintfAllBook(LINKb bhead){
  printf("도서명 출판사 저자 ISBN 소장처 대여가능여부\n");
  while(bhead!=NULL)
    printf("%s %s %s %d %s %c\n", bhead->title, bhead->publisher, bhead->author, bhead->isbn, bhead->location, bhead->borrow);
}
//내 대여 목록
void ShowbBorrowList(LINKd dhead){
  printf(">> 내 대여 목록 <<\n");
  while(1){
    printf("도서 번호: \n");
    printf("도서명: \n");
    printf("대여 일자: \n");
    printf("반납 일자: \n\n");
  }
}
//개인정보 수정
void ResetInfo(LINKc){
  //구조체에서 해당 코드 찾기, 수정된 구조체 파일에 입력하기
}
//회원 탈퇴
void DeleteAccount(LINKc){
  //구조체에서 해당 코드 찾기, 노드 삭제후 앞뒤 노드 연결, 파일에 수정된 구조체 입력하기
}

