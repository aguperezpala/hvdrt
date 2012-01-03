/*
 * main.cpp
 *
 *  Created on: 14/12/2011
 *      Author: agustin
 */

#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <iostream>

#include <linux/videodev2.h>

static int fd;

static void printFrameSizeEnum(struct v4l2_frmsizeenum *argp)
{

	switch(argp->type){
		case V4L2_FRMSIZE_TYPE_DISCRETE:
			std::cout << "Index: " << argp->index << "\n"
					<< "pixel_format: " << argp->pixel_format << "\n"
					"Width: " << argp->discrete.width << "\t" <<
					"Height: " << argp->discrete.height << "\n\n";

			break;
		case V4L2_FRMSIZE_TYPE_STEPWISE:
		case V4L2_FRMSIZE_TYPE_CONTINUOUS:
//			__u32	min_width	Minimum frame width [pixel].
//			__u32	max_width	Maximum frame width [pixel].
//			__u32	step_width	Frame width step size [pixel].
//			__u32	min_height	Minimum frame height [pixel].
//			__u32	max_height	Maximum frame height [pixel].
//			__u32	step_height	Frame height step size [pixel].
			std::cout << "min_width: " << argp->stepwise.min_width << "\n" <<
				"max_width: " << argp->stepwise.max_width << "\n" <<
				"step_width: " << argp->stepwise.step_width << "\n" <<
				"min_height: " << argp->stepwise.min_height << "\n" <<
				"max_height: " << argp->stepwise.max_height << "\n" <<
				"step_height: " << argp->stepwise.step_height << "\n\n";
			break;


		default:
			std::cout << "Invalid type\n";
		}
}

static void getResolutions(struct v4l2_frmsizeenum &argp)
{
	argp.index = 0;

	int err = ioctl(fd, VIDIOC_ENUM_FRAMESIZES, &argp);

	if(err == -1){
		perror("Error getting VIDIOC_ENUM_FRAMESIZES: ");
		return;
	}

	// check what type of type is
	switch(argp.type){
	case V4L2_FRMSIZE_TYPE_DISCRETE:
		std::cout << "Analyzing type : V4L2_FRMSIZE_TYPE_DISCRETE\n";
		// now print all the format supported
		printFrameSizeEnum(&argp);
		argp.index++;
		while(ioctl(fd, VIDIOC_ENUM_FRAMESIZES, &argp) != -1){
			printFrameSizeEnum(&argp);
			argp.index++;
			if(argp.index > 50){
				std::cout << "To much framesizes??\n";
				break;
			}
		}

		break;
	case V4L2_FRMSIZE_TYPE_STEPWISE:
		std::cout << "Analyzing type : V4L2_FRMSIZE_TYPE_STEPWISE\n";
		printFrameSizeEnum(&argp);
		break;
	case V4L2_FRMSIZE_TYPE_CONTINUOUS:
		std::cout << "Analyzing type : V4L2_FRMSIZE_TYPE_CONTINUOUS\n";
		printFrameSizeEnum(&argp);
		break;
	default:
		std::cout << "Invalid type\n";
	}

}


void getCap(struct v4l2_format &chan)
{

	if (ioctl(fd, VIDIOC_G_FMT, &chan) == -1){
		perror("err");
		return;
	}
	printf("Device format: %d\n", chan.type);

	switch(chan.type){
	case V4L2_BUF_TYPE_VIDEO_CAPTURE:
		// is pix
		std::cout << "Width: " <<  chan.fmt.pix.width << std::endl <<
		"Height: " << chan.fmt.pix.height << std::endl;
		break;

	default:
		std::cout << "Error: unknown type???\n";
		return;
	}
}
struct v4l2_queryctrl queryctrl;
	struct v4l2_querymenu querymenu;

static void
	enumerate_menu (void)
	{
	        printf ("  Menu items:\n");

	        memset (&querymenu, 0, sizeof (querymenu));
	        querymenu.id = queryctrl.id;

	        for (querymenu.index = queryctrl.minimum;
	             querymenu.index <= queryctrl.maximum;
	              querymenu.index++) {
	                if (0 == ioctl (fd, VIDIOC_QUERYMENU, &querymenu)) {
	                        printf ("  %s\n", querymenu.name);
	                } else {
	                        perror ("VIDIOC_QUERYMENU");
	                        return;
	                }
	        }
	}

int main(int argc, char **args)
{
	fd = open("/dev/video0", O_RDWR);
	if(fd == -1){
		perror("Error opening /dev/video0");
		return -1;
	}



//
//	memset (&queryctrl, 0, sizeof (queryctrl));
//
//	for (queryctrl.id = V4L2_CID_BASE;
//	     queryctrl.id < V4L2_CID_LASTP1;
//	     queryctrl.id++) {
//	        if (0 == ioctl (fd, VIDIOC_QUERYCTRL, &queryctrl)) {
//	                if (queryctrl.flags & V4L2_CTRL_FLAG_DISABLED)
//	                        continue;
//
//	                printf ("Control %s\n", queryctrl.name);
//
//	                if (queryctrl.type == V4L2_CTRL_TYPE_MENU)
//	                        enumerate_menu ();
//	        } else {
//	                if (errno == EINVAL)
//	                        continue;
//
//	                perror ("VIDIOC_QUERYCTRL");
//	                return -1;
//	        }
//	}
//
//	for (queryctrl.id = V4L2_CID_PRIVATE_BASE;;
//	     queryctrl.id++) {
//	        if (0 == ioctl (fd, VIDIOC_QUERYCTRL, &queryctrl)) {
//	                if (queryctrl.flags & V4L2_CTRL_FLAG_DISABLED)
//	                        continue;
//
//	                printf ("Control %s\n", queryctrl.name);
//
//	                if (queryctrl.type == V4L2_CTRL_TYPE_MENU)
//	                        enumerate_menu ();
//	        } else {
//	                if (errno == EINVAL)
//	                        break;
//
//	                perror ("VIDIOC_QUERYCTRL");
//	                return -1;
//	        }
//	}


//
//	struct v4l2_format chan;
//	getCap(chan);
//	return 0;

	struct v4l2_frmivalenum argp2;
	int err = ioctl(fd, VIDIOC_ENUM_FRAMEINTERVALS, &argp2);
	if(err == -1){
		perror("Error VIDIOC_ENUM_FRAMEINTERVALS: ");
	}

	return 0;

	struct v4l2_input input;
	int index;



	if (-1 == ioctl (fd, VIDIOC_G_INPUT, &index)) {
	        perror ("VIDIOC_G_INPUT");
	        return -1;
	}

	memset (&input, 0, sizeof (input));
	input.index = index;

	if (-1 == ioctl (fd, VIDIOC_ENUMINPUT, &input)) {
	        perror ("VIDIOC_ENUMINPUT");
	        return -1;
	}

	printf ("Current input: %s\n", input.name);


	struct v4l2_frmsizeenum argp;
	getResolutions(argp);

	return 0;
}
