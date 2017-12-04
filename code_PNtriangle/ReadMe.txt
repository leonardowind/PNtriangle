2016-07-30
实现多边形物体的PN-Triangle显示
冯益民

输入：main.cpp 中readfileTea("teapot.obj");
注意：
obj中的数据大小要在 common.h中定义const int vNum，const int vnNum，const int fNum
如果不知道具体大小，就将三个值设置大一些。
在第一次运行后，会在控制台按顺序显示出三个的具体值，可以将其修改，来节省申请的内存。
inport.cpp 第10行的GLfloat nRange = 100.0f是开始时的看到的范围。


操作：
WASD	上下左右平移
Q	放大
E	缩小
1	增加三角面片
2	减少三角面片
左键	增加细分因子
右键	减少细分因子
