#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

// 구조체
typedef struct client {
    char id[20];            // 학번
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
    char client_id[20];     // 학번
    int book_id;            // 도서번호
    time_t borrow_date;     // 대여일자
    time_t return_date;     // 반납일자
    struct borrow *next;
} borrow;

typedef struct search { //동일한 검색결과를 isbn별로 누적해서 더하기 위해 만들었습니다
    long long isbn;
    int total;
    int borrowed;
    struct search *next;
} search;

// 함수 선언
void UserMenu(book *, client *, borrow *, char *);
void BookSearch(book *);
void ShowBorrowList(borrow *, book *, char *);
void DeleteAccount(client **, char *, borrow *);
void ResetInfo(client **, char *);
void PrintSearchResult(book *);
void SearchBook(book *, int , char *);
book *SearchBookByISBN(book *, long long);
book *SearchBookByBookID(book *, int);
void PrintAllBooks(book *);
void SaveToFile(client *);


//전역변수: 제목과 ISBN 검색시 총권수랑 대여권수 계산용
int totalBooks;  
int borrowedBooks;

// 회원 메뉴: 파일을 읽고 생성한 구조체 헤드와 로그인한 학번을 인자로 받습니다
//bhead는 책, chead는 회원, dhead는 대여 헤드입니다
void UserMenu(book *bhead, client *chead, borrow *dhead, char *login_id) {
    int menu;
    while (1) {
        printf("\n>> 회원 메뉴 <<\n");
        printf("1. 도서 검색  2. 내 대여 목록\n3. 개인정보 수정  4. 회원 탈퇴\n5. 로그아웃  6. 프로그램 종료\n\n");
        printf("번호를 선택하세요: ");
        scanf("%d", &menu);
        getchar(); // 버퍼 비우기
        switch (menu) {
            case 1:
                system("clear");
                BookSearch(bhead);
                break;
            case 2:
                ShowBorrowList(dhead, bhead, login_id);
                break;
            case 3:
                ResetInfo(&chead, login_id);
                break;
            case 4:
                DeleteAccount(&chead, login_id, dhead);
                return;
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
              SearchBook(bhead, 1, searchWord);
              break;
          case 2:
              printf("저자명을 입력하세요: ");
              scanf("%s", searchWord);
              SearchBook(bhead, 2, searchWord);
              break;
          case 3:
              printf("출판사를 입력하세요: ");
              scanf("%s", searchWord);
              SearchBook(bhead, 3, searchWord);
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

// 도서 검색 함수(제목, 저자, 출판사: 중복 검색결과가 존재할수 있음->isbn을 누적하는 구조체 리스트로 관리)
void SearchBook(book *bhead, int searchType, char *searchWord) {
    book *tmp = bhead;
    search *searchList = NULL; // search 리스트 초기화
    search *searchTmp = NULL;
    int match = 0;

    printf("\n>> 검색 결과 <<\n");

    // 도서 목록을 순회
    while (tmp) {
        // 조건에 맞는 도서를 찾은 경우
        if ((searchType == 1 && strcmp(tmp->book_name, searchWord) == 0) ||
            (searchType == 2 && strcmp(tmp->author, searchWord) == 0) ||
            (searchType == 3 && strcmp(tmp->publisher, searchWord) == 0)) {
            match = 1;
            // search 리스트에 해당 ISBN이 이미 있는지 확인
            searchTmp = searchList;
            while (searchTmp) {
                if (searchTmp->isbn == tmp->isbn) {
                    // 이미 존재하는 ISBN이면 누적
                    searchTmp->total++;
                    if (tmp->available == 'N') {
                        searchTmp->borrowed++;
                    }
                    break;
                }
                searchTmp = searchTmp->next;
            }

            // 해당 ISBN이 리스트에 없다면 새로 추가
            if (searchTmp == NULL) {
                search *newSearch = (search *)malloc(sizeof(search));
                newSearch->isbn = tmp->isbn;
                newSearch->total = 1;
                newSearch->borrowed = (tmp->available == 'N') ? 1 : 0;
                newSearch->next = searchList;
                searchList = newSearch;
            }
        }

        tmp = tmp->next;
    }

    // 검색 결과 출력
    if (match) {
        searchTmp = searchList;
        while (searchTmp) {
            printf("\nISBN: %lld\n", searchTmp->isbn);
            printf("총 권수: %d\n", searchTmp->total);
            printf("대여된 권수: %d\n", searchTmp->borrowed);
            printf("대여 가능 여부: ");
            if (searchTmp->total > searchTmp->borrowed) {
                printf("Y\n");
            } else {
                printf("N\n");
            }
            printf("%d/%d\n", searchTmp->borrowed, searchTmp->total);
            printf("** Y는 대여가능, N은 대여불가를 의미\n** (x/y) : (대여된 총 권수 / 보유하고 있는 총 권수)");
            searchTmp = searchTmp->next;
        }
    } 
    else {
        printf("검색결과가 없습니다.\n");
    }

    // search 리스트 메모리 해제
    while (searchList) {
        searchTmp = searchList;
        searchList = searchList->next;
        free(searchTmp);
    }
}



// ISBN으로 검색: : 첫번째로 찾은 책의 주소를 return하고 노드의 끝까지 검색합니다
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

//bookid로 검색: : 첫번째로 찾은 책의 주소를 return함
book *SearchBookByBookID(book *bhead, int book_id){
  book *tmp = bhead;  
  while (tmp) {
      if (tmp->book_id == book_id) 
        return tmp;
      tmp = tmp->next;
  }
  return NULL;
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
      printf("** Y는 대여가능, N은 대여불가를 의미\n** (x/y) : (대여된 총 권수 / 보유하고 있는 총 권수)");
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
void ShowBorrowList(borrow *dhead, book *bhead, char *login_id) {
    borrow *borrow_tmp = dhead;
    book *book_tmp = NULL;
    struct tm *borrowDate = NULL, *returnDate = NULL;
    char *weekdays[] = {"일", "월", "화", "수", "목", "금", "토"};

    printf("\n>> 내 대여 목록 <<\n");
    while (borrow_tmp) {
        if (strcmp(borrow_tmp->client_id, login_id) == 0) {
            book_tmp = SearchBookByBookID(bhead, borrow_tmp->book_id);
            borrowDate = localtime(&borrow_tmp->borrow_date);
            returnDate = localtime(&borrow_tmp->return_date);

            printf("도서번호: %d\n", book_tmp->book_id);
            printf("도서명: %s\n", book_tmp->book_name);
            printf("대여일자: %d년 %d월 %d일 %s요일\n", borrowDate->tm_year + 1900,
                   borrowDate->tm_mon + 1, borrowDate->tm_mday, weekdays[borrowDate->tm_wday]);
            printf("반납일자: %d년 %d월 %d일 %s요일\n\n", returnDate->tm_year + 1900,
                   returnDate->tm_mon + 1, returnDate->tm_mday, weekdays[returnDate->tm_wday]);
        }
        borrow_tmp = borrow_tmp->next;
    }
}

//수정된 연결리스트 파일에 저장하는 함수
void SaveToFile(client *chead) {
    FILE *file = fopen("client.txt", "w");
    if (!file) {
        printf("파일 저장 실패.\n");
        return;
    }

    while (chead) {
        fprintf(file, "%s | %s | %s | %s | %s\n", chead->id, chead->password,
                chead->name, chead->address, chead->phone);
        chead = chead->next;
    }
    fclose(file);
}

// 회원 탈퇴 : borrow노드에 id가 없으면 id삭제
void DeleteAccount(client **chead, char *login_id, borrow *dhead) {
    borrow *dtmp = dhead;
    while (dtmp) {
        if (strcmp(dtmp->client_id, login_id) == 0) {
            printf("반납하지 않은 도서가 있습니다.\n");
            return;
        }
        dtmp = dtmp->next;
    }

    client *tmp1 = *chead, *tmp2 = NULL;
    while (tmp1) {
        if (strcmp(tmp1->id, login_id) == 0) {
            if (tmp2) {
                tmp2->next = tmp1->next;
            } else {
                *chead = tmp1->next;
            }
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
void ResetInfo(client **chead, char *login_id) {
    client *temp = *chead;
    while (temp) {
        if (strcmp(temp->id, login_id) == 0) {
            printf("새로운 비밀번호를 입력하세요");
            scanf("%19[^\n]", temp->password);
            getchar();
            printf("새로운 주소를 입력하세요: ");
            scanf("%49[^\n]", temp->address);
            getchar();
            printf("새로운 전화번호를 입력하세요: ");
            scanf("%19[^\n]", temp->phone);
            getchar();
            printf("개인정보가 수정되었습니다.\n");
            SaveToFile(*chead);
            return;
        }
        temp = temp->next;
    }
}


