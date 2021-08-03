#include <opencv2/opencv.hpp>
int cnt = 0;
int window_size = 11;
int **isValid;

float  getDist(CvScalar a, CvScalar b)
{
	return sqrt((a.val[0] - b.val[0]) * (a.val[0] - b.val[0])
		+ (a.val[1] - b.val[1]) * (a.val[1] - b.val[1])
		+ (a.val[2] - b.val[2]) * (a.val[2] - b.val[2]));
}

CvScalar FindMatch(IplImage *src, IplImage *dst, int dst_y, int dst_x)
{
	CvScalar out = cvScalar(0, 0, 0);
	float error = 0;
	float min_error = FLT_MAX;

	for (int y = window_size / 2; y < src->height - window_size / 2; y++)
		for (int x = window_size / 2; x < src->width - window_size / 2; x++)
		{
			for (int v = -window_size / 2; v <= window_size / 2; v++)
			{
				for (int u = -window_size / 2; u <= window_size / 2; u++)
				{
					int sx = x + u;
					int sy = y + v;
					int dx = dst_x + u;
					int dy = dst_y + v;

					if (dy<0 || dy>dst->height - 1) continue;
					if (dx<0 || dx>dst->width - 1) continue;
					if (isValid[dy][dx] == 0) continue;

					error += getDist(cvGet2D(src, sy, sx), cvGet2D(dst, dy, dx));
					cnt++;
				}
			}

			if (cnt != 0)
			{
				error /= cnt;
				cnt = 0;
			}

			if (error < min_error)
			{
				out = cvGet2D(src, y, x);
				min_error = error;
			}
		}
	return out;
}

void GrowImage(IplImage* src, IplImage * dst)
{
	for (int x = src->width; x < dst->width; x++)
	{
		for (int y = 0; y < src->height; y++)
		{
			CvScalar c = FindMatch(src, dst, y, x);

			cvSet2D(dst, y, x, c);
			isValid[y][x] = 1;
			cvShowImage("dst", dst);
			cvWaitKey(1);
		}
	}

	for (int y = src->height; y < dst->height; y++)
	{
		for (int x = 0; x < dst->width; x++)
		{
			CvScalar c = FindMatch(src, dst, y, x);

			cvSet2D(dst, y, x, c);
			isValid[y][x] = 1;
			cvShowImage("dst", dst);
			cvWaitKey(1);
		}
	}
}

int main()
{
	//scanf("%d", &window_size);
	IplImage *src = cvLoadImage("c:\\temp\\161.bmp");
	IplImage *dst = cvCreateImage(cvSize(150, 100), 8, 3);
	isValid = (int **)malloc(sizeof(int*)*(dst->height));

	for (int i = 0; i < dst->height; i++) {
		isValid[i] = (int*)malloc(sizeof(int) * (dst->width));
	}

	for (int y = 0; y < dst->height; y++)
		for (int x = 0; x < dst->width; x++)
			isValid[y][x] = 0;

	for (int y = 0; y < src->height; y++)
		for (int x = 0; x < src->width; x++)
		{
			CvScalar f = cvGet2D(src, y, x);
			cvSet2D(dst, y, x, f);
			isValid[y][x] = 1;
		}

	GrowImage(src, dst);
	cvShowImage("dst", dst);
	cvWaitKey();
	return 0;
}