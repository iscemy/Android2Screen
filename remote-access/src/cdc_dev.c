#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>
#include <errno.h>
#include <time.h>




static int exec_prog(const char **argv, char* path)
{
    pid_t   my_pid;
    int     status, timeout /* unused ifdef WAIT_FOR_COMPLETION */;

    if (0 == (my_pid = fork())) {
            if (-1 == execv(path, argv)) {
                    perror("child process execve failed [%m]");
                    return -1;
            }
    }


    timeout = 1000;

    while (0 == waitpid(my_pid , &status , WNOHANG)) {
            if ( --timeout < 0 ) {
                    perror("timeout");
                    return -1;
            }
            sleep(1);
    }

    printf("%s WEXITSTATUS %d WIFEXITED %d [status %d]\n",
            argv[0], WEXITSTATUS(status), WIFEXITED(status), status);

    if (1 != WIFEXITED(status) || 0 != WEXITSTATUS(status)) {
            perror("%s failed, halt system");
            return -1;
    }


    return 0;
}

char usb_gadget_enable[] =  "/sys/class/android_usb/android0/enable";
int open_write_close(char* file, char* buf, int size){
    int fd = open(file, O_WRONLY);
    if(fd < 0) return -1;
    write(fd, buf, size);
    close(fd);  
    return 0; 
}

void set_usb_gadget_state(bool state){
    char d;
    if(state)
        d = '1';
    else
        d = '0';
    open_write_close(usb_gadget_enable, &d, 1);
}

void set_usbg_vendorid(unsigned int id){

}


#define PATH_MAX 100000
char *setprop1[] = {"setprop", "sys.usb.config", "adb,acm", NULL};
char *setprop2[] = {"setprop", "sys.storage.ums_enabled", "1", NULL};
char vendorid[] = "/sys/class/android_usb/android0/idVendor";
char productid[] = "/sys/class/android_usb/android0/idProduct";

char usb_gadget_functions[] = "/sys/class/android_usb/android0/functions";
char setprop[] = "/data/local/setprop";
char usb_state_prope[] = "sys.usb.state";
char usb_config_prope[] = "sys.usb.config";


void set_usb_modes(char *modes, int size){

    //set_usb_gadget_state(0);
    int ret;
    //open_write_close(usb_gadget_functions, modes, size);
    open_write_close(vendorid, "18d1", 4);
    open_write_close(productid, "d02", 3);
    //ret = system("/sbin/setprop sys.usb.state adb,acm ");
    //printf("ret %s\n", strerror(errno));
    //ret = system("/sbin/setprop sys.usb.config adb,acm");  
    //printf("ret %s\n", strerror(errno));

    //setprop1[2] = modes;
    ret = exec_prog(setprop1, "/sbin/setprop");

    ret = exec_prog(setprop2, "/sbin/setprop");



    //printf("%s \n", modes);


}
