#include <iostream>

#include <string.h> 
#include <linux/fb.h>
#include <sys/mman.h>

#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>

int main(int argc, char **argv){
    std::cout << "hello android with frame buffer access 2\n";
    char fbdev[] = "/dev/graphics/fb0";
    int fbfd = open (fbdev, O_RDWR);
    struct fb_var_screeninfo vinfo;

    ioctl (fbfd, FBIOGET_VSCREENINFO, &vinfo);

    int fb_width = vinfo.xres;
    int fb_height = vinfo.yres;
    int fb_bpp = vinfo.bits_per_pixel;
    int fb_bytes = fb_bpp / 8;

    printf("width %d height %d bytes-per-pixel %d", fb_width, fb_height, fb_bytes);

    int fb_data_size = fb_width * fb_height * fb_bytes;

    char *fbdata = (char*) mmap (0, fb_data_size, 
        PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, (off_t)0);

    memset (fbdata, 0, fb_data_size);

    munmap (fbdata, fb_data_size);
    close (fbfd);

    return 0;
}