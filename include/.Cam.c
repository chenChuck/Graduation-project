#include "jpeglib.h"
#define	REQBUFS_COUNT	4
typedef struct buffer {
	void* start;
	unsigned int length;
}BUFF;

struct bufx{
	int length;
	char bufx[1024*50];
}buff;

BUFF bufs[REQBUFS_COUNT];
struct v4l2_requestbuffers req;
struct jpeg_mgr_info {
	unsigned long written;
	JSAMPROW row_pointer[1];//指向第一行图像数据的指针
	struct jpeg_error_mgr jerr;
	struct jpeg_compress_struct cinfo;
};
static struct jpeg_mgr_info jinfo;

int rgb_to_jpeg_init(void *rgb, void *jpeg, unsigned int width, unsigned int height, unsigned int bpp, int quality)
{

	
	jinfo.cinfo.err = jpeg_std_error(&jinfo.jerr);  //初始化错误信息
	jpeg_create_compress(&jinfo.cinfo);
	
	unsigned long written = width * height * bpp / 3;
	jpeg_mem_dest(&jinfo.cinfo, (unsigned char **)&jpeg, &written);//关联JPEG的压缩对象跟输入文件
	
	//设置压缩参数
	jinfo.cinfo.image_width = width;
	jinfo.cinfo.image_height = height;
	jinfo.cinfo.input_components = bpp / 8;
	jinfo.cinfo.in_color_space = JCS_RGB;
	jpeg_set_defaults(&jinfo.cinfo);
	// 设定编码jpeg压缩质量 
	jpeg_set_quality(&jinfo.cinfo, quality, TRUE);
	//开始压缩
	jpeg_start_compress(&jinfo.cinfo, TRUE);
	//如果当前扫描的行数小于图片的总行数就继续
	while(jinfo.cinfo.next_scanline < height) {
		jinfo.row_pointer[0] = rgb + jinfo.cinfo.next_scanline * width * bpp / 8;
		jpeg_write_scanlines(&jinfo.cinfo, jinfo.row_pointer, 1);
	}
	jpeg_finish_compress(&jinfo.cinfo);//完成压缩
	return (written);
}



void rgb_to_jpeg_exit(void)
{
	jpeg_destroy_compress(&jinfo.cinfo);
}


int camera_init(unsigned int *width, unsigned int *height, unsigned *size)
{
		int ret;
		int fd;
		int i;
		struct v4l2_capability cap;
		struct v4l2_format fmt;
		struct v4l2_buffer buf;
		struct v4l2_fmtdesc fmtdesc;
		struct v4l2_requestbuffers req;
		fd = open("/dev/video0",O_RDWR); //打开设备，读写方式
		if(fd < 0){
			bug(":open dev fail");
			return -1;
		}
		else
			printf("open cam success\n");
	
	
	ret = ioctl(fd,VIDIOC_QUERYCAP,&cap);		//获取设备属性
	if(ret == -1){
		bug(":Failed to obtain device properties");
		return -1;
	}
	printf("Driver Name:%s\nCard Name:%s\nBus info:%s\nnDriver Version:%u.%u.%u\n"
			,cap.driver,cap.card,cap.bus_info,(cap.version>>16)&0xFF,
			(cap.version>>8)&0xFF,cap.version&0xFF);

	
	fmtdesc.index=0;  							//序号
	fmtdesc.type=V4L2_BUF_TYPE_VIDEO_CAPTURE; 	//帧类型
	printf("Support format:\n");

	while(ioctl(fd, VIDIOC_ENUM_FMT, &fmtdesc) != -1)	//获取支持的所有格式

	{

		printf("%d.%s\n",fmtdesc.index+1,fmtdesc.description);//显示格式名称

		fmtdesc.index++;

	}
	
	fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	fmt.fmt.pix.width =  *width;
	fmt.fmt.pix.height = *height;
	fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV	;	//帧格式,YUV4：2：2
	ret = ioctl(fd, VIDIOC_S_FMT, &fmt);			//设置视频捕捉格式
	if(ret == -1){
		bug(":Format failed");
		return -1;
	}
	ret = ioctl(fd, VIDIOC_G_FMT, &fmt);			//查看当前视频捕捉格式
	if(ret == -1){
		bug(":View image format failed");
		return -1;
	}
	printf("Current data format information:\n\tdescription:%s\n\twidth:%d\n\theigth:%d\n",
			fmtdesc.description,fmt.fmt.pix.width,fmt.fmt.pix.height);
	
	
	memset(&req,0,sizeof(req));
	req.count = REQBUFS_COUNT;									//缓冲区内缓冲帧数
	req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;			//数据格式
	req.memory = V4L2_MEMORY_MMAP;					//内存映射方式
	
	ret = ioctl(fd,  VIDIOC_REQBUFS, &req);			//申请一个有4个缓冲帧的缓冲区
	if(ret == -1){
		bug(":APPlication buffer failed");
		return -1;
	}
	
// 开辟结构体数组()
	
	BUFF *buff = (BUFF *)calloc(req.count, sizeof(BUFF));
	if(buff == NULL){
		bug(":calloc failed");
		return -1;
	}
	
	for(i = 0;i < req.count; i++){
		memset(&buf, 0, sizeof(buf));
		buf.index = i;
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;	
		buf.memory = V4L2_MEMORY_MMAP;
		ret = ioctl(fd, VIDIOC_QUERYBUF, &buf); //把VIDIOC_REQBUFS中分配的数据缓存转换成物理地址
		if(ret == -1){
			bug(":(1)Address conversion failed");
			return -1;	
		}
		
		bufs[i].length = buf.length;
		bufs[i].start = mmap(NULL, buf.length, PROT_READ|PROT_WRITE,
							MAP_SHARED,fd,buf.m.offset);		//将地址转换为相对地址
		if(MAP_FAILED == bufs[i].start){
			bug(":Memory map failed");
			return -1;
			
		}
	}
	//入队：VIDIOC_QBUF
	for(i = 0; i < req.count; i++){
	struct v4l2_buffer buf;
	buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory = V4L2_MEMORY_MMAP;
	buf.index = i;
	ret = ioctl(fd, VIDIOC_QBUF, &buf); 
		if(ret == -1){
			bug(":The team failed");
			return -1;
		}
	}
	
	enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	ret = ioctl(fd, VIDIOC_STREAMON, &type);
	if (ret == -1) {
		bug(":camera_start failed");
		return -1;
	}
	else
	printf("start capture\n");
	
	
	*width = fmt.fmt.pix.width;
	*height = fmt.fmt.pix.height;
	*size = bufs[0].length;
	return fd;
}


void yuv_rgb(unsigned char yuv_buf[], unsigned char RGB_buf[])
{
		int m,n;  
		unsigned char y1,y2,u,v;  
		int r1,g1,b1,r2,g2,b2;  
		unsigned char *src = yuv_buf; 
		unsigned char *dst = RGB_buf;
	
		for(m = 0; m < 480; m++)  
		{
			for(n = 0; n < 640 / 2; n++) 
			{  
				y1 = *( src + (m * 640 / 2 + n) * 4);       
				u  = *( src + (m * 640 / 2 + n) * 4 + 1);  
				y2 = *( src + (m * 640 / 2 + n) * 4 + 2);  
				v  = *( src + (m * 640 / 2 + n) * 4 + 3);  

				b1 = y1 + 1.042 * (v - 128);  
				g1 = y1 - 0.34414 * (u - 128) - 0.71414 * (v - 128);  
				r1 = y1 + 1.772 * (u - 128);  

		  
				b2 = y2 + 1.042 * (v - 128);  
				g2 = y2 - 0.34414 * (u - 128) - 0.71414 * (v - 128);  
				r2 = y2 + 1.772 * (u - 128);  

				if(r1 > 255)  
					r1 = 255;  
				else if(r1 < 0)  
					r1 = 0;  
		  
				if(b1 > 255)  
					b1 = 255;  
				else if(b1 < 0)  
					b1 = 0;      
		  
				if(g1 > 255)  
					g1 = 255;  
				else if(g1 < 0)  
					g1 = 0;      
		  
				if(r2 > 255)  
					r2 = 255;  
				else if(r2 < 0)  
					r2 = 0;  
		  
				if(b2 > 255)  
					b2 = 255;  
				else if(b2 < 0)  
					b2 = 0;      
		  
				if(g2 > 255)  
					g2 = 255;  
				else if(g2 < 0)  
					g2 = 0;
					
				*(dst + (m * 640 / 2 + n) * 6    ) = (unsigned char)b1;  
				*(dst + (m * 640 / 2 + n) * 6 + 1) = (unsigned char)g1;  
				*(dst + (m * 640 / 2 + n) * 6 + 2) = (unsigned char)r1;  
				*(dst + (m * 640 / 2 + n) * 6 + 3) = (unsigned char)b2;  
				*(dst + (m * 640 / 2 + n) * 6 + 4) = (unsigned char)g2;  
				*(dst + (m * 640 / 2 + n) * 6 + 5) = (unsigned char)r2;  
			}

			
		}
}
int camera_start(int fd,char jpeg_buf[])
{

	int i,j;
	int ret;
	int ii;
	int yuvnumber = 0;
	int bmpnumber = 0;
	int jpeg_size;
	unsigned char yuv_buf[614400] = {0};  //600k
	unsigned char RGB_buf[640*480*3] = {0}; //900k
	char buf_len[1] = {0};
	char jpeg_name[30] = {0};
	char data[1024]={0};
	int jpegnumber = 0;
	FILE* picfd;
	struct v4l2_buffer buf;
	memset(&buf, 0, sizeof(buf));
	buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory = V4L2_MEMORY_MMAP;
	
while(camflag)
{
		usleep(3000);	
		for(i = 0; i <4; i++)
		{
			struct v4l2_buffer buf;
			buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
			buf.memory = V4L2_MEMORY_MMAP;
			buf.index = i;
			ret = ioctl(fd, VIDIOC_DQBUF, &buf); //出队
			if(ret == -1){
				bug(":Failure of the team");
				return -1;
				printf("----------\n");
			}
			
			//YUV---RGB	
			yuv_rgb(yuv_buf, RGB_buf);
			printf("yuv-rgb OK\n");
			//压缩
			
			jpeg_size = rgb_to_jpeg_init(RGB_buf, jpeg_buf, 640, 480, 24, 80);
			printf("jpeg_size:%d\n",jpeg_size);
			ii = jpeg_size;
			buf_len[0] = jpeg_size;
			printf("----------\n");
			printf("%d\n",ii);
			
			
			
			sprintf(jpeg_name, "../pic_test/jpeg_pic%d.jpeg", ++jpegnumber);
			if(jpegnumber == 11)
				jpegnumber = 0;
	
			picfd = fopen(jpeg_name, "w");
			ret = fwrite(jpeg_buf, jpeg_size, 1,picfd);
			printf("write = %d\n", ret);
			fclose(picfd);
		#if 0	
			picfd = fopen(jpeg_name, "r");
			
			ret = write(new_fd,&buf_len[0],4);
			buf_len[0] = 0;
			printf ("%d\n",ret);
			j = 1;
			while(j){
				
				ret = fread(&buf,1,1024,picfd);
				if(ret == 0)
					i = 0;
				ret = write(new_fd,&buf,ret);
				
				if (ret == -1){
					perror("send");
					sleep(10);
					return 0;
				
				}
				printf ("%d\n",ret);
				ii -= ret;
				printf("ii = ：%d\n",ii);
				fclose(picfd);
	
		
			char jpeg_name[30] = {0};
			sprintf(jpeg_name, "../pic_test/jpeg_pic%d.jpeg", jpegnumber);
			
			memcpy(buff.bufx,jpeg_buf,jpeg_size);
			buff.length = jpeg_size;
			
	
			picfd = fopen(jpeg_name, "w");
			ret = fwrite(jpeg_buf, jpeg_size, 1,picfd);
			printf("write = %d\n", ret);
			fclose(picfd);
			rgb_to_jpeg_exit;
	
			printf("YUYV to RGB OK \n");
	
			
			memcpy(yuv_buf,bufs[buf.index].start,bufs[buf.index].length);	

			sprintf(yuv_name, "../pic_test/yuyv_pic%d", ++yuvnumber);
			printf("%s\n",yuv_name);
			picfd = fopen(yuv_name, "w");
			if(picfd == NULL){
				bug("init:");
				return -1;
			}
		printf("------------------\n");	
			ret = fwrite(yuv_buf, 1, bufs[buf.index].length, picfd);
			printf("write = %d\n", ret);
		printf("------------------\n");	
			fclose(picfd);
	#endif
			memcpy(yuv_buf,bufs[buf.index].start,bufs[buf.index].length);	
			if(ioctl(fd, VIDIOC_QBUF, &buf) < 0)
			{
				perror("QBUF Error");
				exit(-1);
			}
			
		}	
	}	
	return jpeg_size;
	//return 0;
}
