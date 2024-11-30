#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

// 구조체
typedef struct client {
    int id;                 // 학번
    char password[20];      // 비밀번호
    char name[30];          // 이름
    char address[50];       // 주소
    char phone[20];         // 전화번호
    struct client *next;
} client;

typedef struct book {
    int book_id;            // 도서번호
    char book_name[50];     // 도서명
    char publisher[30];     // 출판사
    char author[30];        // 작가
    long long isbn;         // ISBN
    char location[50];      // 소장처
    char available;         // 대여 가능 여부 'Y' or 'N'
    struct book *next;
} book;

typedef struct borrow {
    int client_id;          // 학번
    int book_id;            // 도서번호
    time_t borrow_date;     // 대여일자
    time_t return_date;     // 반납일자
    struct borrow *next;
} borrow;

// 함수 선언
void UserMenu(book *, client *, borrow *, int);
void BookSearch(book *);
void ShowBorrowList(borrow *, book *);
void DeleteAccount(client **, int);
void ResetInfo(client **, int);
void PrintSearchResult(book *);
book *SearchBookByName(book *, const char *);
book *SearchBookByAuthor(book *, const char *);
book *SearchBookByPublisher(book *, const char *);
book *SearchBookByISBN(book *, long long);
void PrintAllBooks(book *);
void SaveToFile(client *);

//전역변수: 책 검색시 총권수랑 대여권수 계산하려고 만들었어요
int totalBooks;  
int borrowedBooks;

// 회원 메뉴: 파일을 읽고 생성한 구조체 헤드와 로그인한 학번을 인자로 받습니다
//bhead는 책, chead는 회원, dhead는 대여 헤드입니다
void UserMenu(book *bhead, client *chead, borrow *dhead,int login_id) {
    int menu;
    while (1) {
        system("clear");
        printf(">> 회원 메뉴 <<\n");
        printf("1. 도서 검색  2. 내 대여 목록\n3. 개인정보 수정  4. 회원 탈퇴\n5. 로그아웃  6. 프로그램 종료\n\n");
        printf("번호를 선택하세요: ");
        scanf("%d", &menu);
        switch (menu) {
            case 1:
                system("clear");
                BookSearch(bhead);
                break;
            case 2:
                system("clear");
                ShowBorrowList(dhead, bhead);
                break;
            case 3:
                system("clear");
                ResetInfo(&chead, login_id);//이 함수가 끝나면 파일과 client노드가 수정됩니다
                break;
            case 4:
                system("clear");
                DeleteAccount(&chead, login_id);//이 함수가 끝나면 파일과 client노드가 수정됩니다
                break;
            case 5:
                printf("로그아웃합니다.\n");
                sleep(1);
                system("clear");
                return;
            case 6:
                printf("프로그램을 종료합니다.\n");
                sleep(3);
                exit(0);
            default:
                printf("잘못된 번호입니다.\n");
        }
    }
}

// 도서 검색 메뉴: 관리자용 메뉴에도 넣어주세요
void BookSearch(book *bhead) {
  int menu;
  while (1) {
      totalBooks = 0;
      borrowedBooks = 0;
      printf("\n>> 도서 검색 <<\n");
      printf("1. 도서명 검색  2. 출판사 검색\n3.ISBN검색  4.저자명 검색\n5.전체 검색  6.이전 메뉴\n");
      printf("번호를 선택하세요: ");
      scanf("%d", &menu);
      char searchWord[50];
      long long searchISBN;
      book *result = NULL;

      switch (menu) {
          case 1:
              printf("도서명을 입력하세요: ");
              scanf("%s", searchWord);
              result = SearchBookByName(bhead, searchWord);
              PrintSearchResult(result);
              break;
          case 2:
              printf("저자명을 입력하세요: ");
              scanf("%s", searchWord);
              result = SearchBookByAuthor(bhead, searchWord);
              PrintSearchResult(result);
              break;
          case 3:
              printf("출판사를 입력하세요: ");
              scanf("%s", searchWord);
              result = SearchBookByPublisher(bhead, searchWord);
              PrintSearchResult(result);
              break;
          case 4:
              printf("ISBN을 입력하세요: ");
              scanf("%lld", &searchISBN);
              result = SearchBookByISBN(bhead, searchISBN);
              PrintSearchResult(result);
              break;
          case 5:
              PrintAllBooks(bhead);
              break;
          case 6:
              printf("이전메뉴로 돌아갑니다.\n");
              sleep(3);
              return;
          default:
              printf("잘못된 번호입니다.\n");
      }
  }
}

// 도서 검색 함수들: 첫번째로 찾은 책의 주소를 return하고 노드의 끝까지 검색합니다
// 책명으로 검색
book *SearchBookByName(book *bhead, const char *name) {
  book *tmp = bhead;  
  book *firstBook = NULL;

  while (tmp) {
      if (strcmp(tmp->book_name, name) == 0) {
          if (firstBook == NULL) 
              firstBook = tmp;
          totalBooks++;
          if (tmp->available == 'N') 
              borrowedBooks++;        
      }
      tmp = tmp->next;
  }
  return firstBook;
}

// 저자명으로 검색
book *SearchBookByAuthor(book *bhead, const char *author) {
  book *tmp = bhead;  
  book *firstBook = NULL;
  totalBooks = 0;
  borrowedBooks = 0;

  while (tmp) {
      if (strcmp(tmp->author, author) == 0) {
          if (firstBook == NULL) 
              firstBook = tmp;
          totalBooks++;
          if (tmp->available == 'N') 
              borrowedBooks++;
      }
      tmp = tmp->next;
  }
  return firstBook;
}

// 출판사명으로 검색
book *SearchBookByPublisher(book *bhead, const char *publisher) {
  book *tmp = bhead;
  book *firstBook = NULL;
  totalBooks = 0;
  borrowedBooks = 0;

  while (tmp) {
      if (strcmp(tmp->publisher, publisher) == 0) {
          if (firstBook == NULL) 
              firstBook = tmp;
          totalBooks++;
          if (tmp->available == 'N') 
              borrowedBooks++;
      }
      tmp = tmp->next;
  }
  return firstBook;
}

// ISBN으로 검색
book *SearchBookByISBN(book *bhead, long long isbn) {
  book *tmp = bhead;  
  book *firstBook = NULL;
  totalBooks = 0;
  borrowedBooks = 0;

  while (tmp) {
      if (tmp->isbn == isbn) {
          if (firstBook == NULL) 
              firstBook = tmp;
          totalBooks++;
          if (tmp->available == 'N') 
              borrowedBooks++;
      }
      tmp = tmp->next;
  }
  return firstBook;
}

// 검색 결과 출력
void PrintSearchResult(book *result) {
  if (result) {
      printf("\n>> 검색결과 <<\n");
      printf("도서명: %s\n", result->book_name);
      printf("출판사: %s\n", result->publisher);
      printf("저자명: %s\n", result->author);
      printf("ISBN: %lld\n", result->isbn);
      printf("소장처: %s\n", result->location);
      printf("대여 가능 여부:");
      if(borrowedBooks < totalBooks)
          printf("Y");
      else
          printf("N");
      printf("%d/%d\n", borrowedBooks, totalBooks);
  } 
  else 
      printf("검색 결과가 없습니다.\n");
}

// 모든 도서 출력
void PrintAllBooks(book *bhead) {
  book *tmp = bhead;  
  printf("\n>> 전체 도서 목록 <<\n");
  printf("도서명\t출판사\t저자명\tISBN\t소장처\t대여가능 여부\n");
  while (tmp) {
      printf("%s\t%s\t%s\t%lld\t%s\t%c\n",
             tmp->book_name, tmp->publisher, tmp->author,
             tmp->isbn, tmp->location, tmp->available);
      tmp = tmp->next;
  }
}

// 내 대여 목록 출력
void ShowBorrowList(borrow *dhead, book *bhead) {
  borrow *tmp = dhead;  
  printf(">> 내 대여 목록 <<\n");
  while (tmp) {
      book *book = SearchBookByISBN(bhead, tmp->book_id);
      if (book) {
          printf("도서번호: %d\n", book->book_id);
          printf("도서명: %s\n", book->book_name);
          printf("대여일자: %s", ctime(&tmp->borrow_date));
          printf("반납일자: %s\n", ctime(&tmp->return_date));
      }
      tmp = tmp->next;
  }
}


//수정된 연결리스트 파일에 저장하는 함수(저장 후엔 연결리스트 새롭게 생성해주세요!)
void SaveToFile(client *chead) {
    FILE *file = fopen("client.txt", "w"); // client.txt파일 열기(파일명 다르면 수정해주세요)
    client *tmp = chead;
    while (tmp) {
        fprintf(file, "%08d | %s | %s | %s | %s\n", tmp->id, tmp->password, tmp->name, tmp->address, tmp->phone);
        tmp = tmp->next;
    }
    fclose(file);
}

// 회원 탈퇴
void DeleteAccount(client **chead, int login_id) {
    client *tmp1 = *chead, *tmp2 = NULL;
    while (tmp1) {
        if (tmp1->id == login_id) {
            if (tmp2)
                tmp2->next = tmp1->next;
            else
                *chead = tmp1->next;
            free(tmp1);
            printf("회원 탈퇴가 완료되었습니다.\n");
            SaveToFile(*chead);
            return;
        }
        tmp2 = tmp1;
        tmp1 = tmp1->next;
    }
}

// 개인정보 수정(노드를 수정하고 파일을 수정된 노드로 덮어씌움)
void ResetInfo(client **chead, int login_id) {
    client *temp = *chead;
    while (temp) {
        if (temp->id == login_id) {
            printf("새로운 비밀번호를 입력하세요");
            scanf("%s", temp->password);
            printf("새로운 주소를 입력하세요: ");
            scanf("%s", temp->address);
            printf("새로운 전화번호를 입력하세요: ");
            scanf("%s", temp->phone);
            printf("개인정보가 성공적으로 수정되었습니다.\n");
            SaveToFile(*chead);
            return;
        }
        temp = temp->next;
    }
}


