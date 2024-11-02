#include <utility>
#include "tiny3d_trapezoid.h"


// 根据三角形生成 0-2 个梯形，并且返回合法梯形的数量
int Trapezoid::SplitTriangleIntoTrapezoids(std::array<Trapezoid, 2>& trap, const vertex_t* p1, const vertex_t* p2, const vertex_t* p3)
{
    float k, x;

    // 将三个顶点，以y值向下递增的顺序，交换排列成p1,p2,p3
    if (p1->pos.y > p2->pos.y)
    {
        std::swap(p1, p2);
    }

    if (p1->pos.y > p3->pos.y)
    {
        std::swap(p1, p3);
    }

    if (p2->pos.y > p3->pos.y)
    {
        std::swap(p3, p2);
    }

    // 三点同线，无法分割梯形
    if ((p1->pos.y == p2->pos.y && p1->pos.y == p3->pos.y) ||
        (p1->pos.x == p2->pos.x && p1->pos.x == p3->pos.x))
    {
        return 0;
    }

    // 三角形的一条边和屏幕的顶边平行
    if (p1->pos.y == p2->pos.y)
    {
        // 交换一下，使得p1一定在p2的左边
        if (p1->pos.x > p2->pos.x)
        {
            std::swap(p1, p2);
        }

        /* 这是一个特殊的梯形，其实就是三角形，这三角形的
        顶边和屏幕的水平方向平行，如下图：

        p1     p2
        -------
        |    /
        |   /
        |  /
        | /
        |/
        p3

        */

        trap[0].set_top(p1->pos.y);    // 梯形的顶边Y值为p1
        trap[0].set_bottom(p3->pos.y); // 梯形的底边Y值为p1
        trap[0].left().v1 = *p1;      // 梯形的左腰边上顶点为p1,下顶点为p3
        trap[0].left().v2 = *p3;
        trap[0].right().v1 = *p2;     // 梯形的右腰边上顶点为p2，下顶点为p3
        trap[0].right().v2 = *p3;
        return trap[0].top() < trap[0].bottom() ? 1 : 0;
    }

    if (p2->pos.y == p3->pos.y)
    {
        if (p2->pos.x > p3->pos.x)
        {
            std::swap(p3, p2);
        }
        // 和上面的梯形（三角形）类似，这里的三角形如下
        /*

        p1
        |\
        | \
        |  \
        |   \
        |    \
        -------
        p2    p3
        */

        trap[0].set_top(p1->pos.y);
        trap[0].set_bottom(p3->pos.y);
        trap[0].left().v1 = *p1;
        trap[0].left().v2 = *p2;
        trap[0].right().v1 = *p1;
        trap[0].right().v2 = *p3;
        return (trap[0].top() < trap[0].bottom()) ? 1 : 0;
    }

    // 上面的是三角形的两种特殊情况，可以直接视为梯形，下面就是要分割三角形为两个梯形

    trap[0].set_top(p1->pos.y);
    trap[0].set_bottom(p2->pos.y);
    trap[1].set_top(p2->pos.y);
    trap[1].set_bottom(p3->pos.y);

    // 算出p2y到p1y的距离，是p3y到p1y的几分之几，然后在算出把一个三角形分割成两个三角形（梯形）的分割线在哪里
    k = (p3->pos.y - p1->pos.y) / (p2->pos.y - p1->pos.y);
    x = p1->pos.x + (p2->pos.x - p1->pos.x) * k;

    if (x <= p3->pos.x) // triangle left
    {
        trap[0].left().v1 = *p1;
        trap[0].left().v2 = *p2;
        trap[0].right().v1 = *p1;
        trap[0].right().v2 = *p3;
        trap[1].left().v1 = *p2;
        trap[1].left().v2 = *p3;
        trap[1].right().v1 = *p1;
        trap[1].right().v2 = *p3;
    }
    else  // triangle right
    {
        trap[0].left().v1 = *p1;
        trap[0].left().v2 = *p3;
        trap[0].right().v1 = *p1;
        trap[0].right().v2 = *p2;
        trap[1].left().v1 = *p1;
        trap[1].left().v2 = *p3;
        trap[1].right().v1 = *p2;
        trap[1].right().v2 = *p3;
    }

    return 2;
}

// 按照 Y 坐标计算出左右两条边纵坐标等于 Y 的顶点
void Trapezoid::CalculateEdgeInterpolatedPoint(float y)
{
    // 算出左右腰边的Y值差
    float s1 = this->left().v2.pos.y - this->left().v1.pos.y;
    float s2 = this->right().v2.pos.y - this->right().v1.pos.y;

    // 根据传递进来的y值，和左右腰边的Y值差，算出插值比例
    float t1 = (y - this->left().v1.pos.y) / s1;
    float t2 = (y - this->right().v1.pos.y) / s2;
    
    // 算出，根据左右腰边两个端点的值，算出左右腰边的插值点的纹理坐标，颜色值
    vertex_interp(&this->left().interpolated_point, &this->left().v1, &this->left().v2, t1);
    vertex_interp(&this->right().interpolated_point, &this->right().v1, &this->right().v2, t2);
}

// 根据左右两边的端点，初始化计算出扫描线的起点和步长
void Trapezoid::InitializeScanline(scanline_t* scanline, int y)
{
    // 根据算出来的梯形腰边的插值位置点，算出扫描线
    // 梯形的右腰边插值点x值，减去左腰边插值点x值，就是当下扫描线的长度
    float width = this->right().interpolated_point.pos.x - this->left().interpolated_point.pos.x;

    scanline->left_end_point_x = static_cast<int>(this->left().interpolated_point.pos.x + 0.5f); // 扫描线的左端点的x
    scanline->y = y; //扫描线的Y值，扫描线肯定平行于屏幕水平边
    scanline->width = static_cast<int32_t>(this->right().interpolated_point.pos.x + 0.5f) - scanline->left_end_point_x;

    scanline->interpolated_point = this->left().interpolated_point;

    if (this->left().interpolated_point.pos.x >= this->right().interpolated_point.pos.x)
        scanline->width = 0;

    // 根据左腰右腰插值点和扫描线宽度，算出每一个“插值步”的position，color，uv的值
    vertex_division(&scanline->interpolated_step, &this->left().interpolated_point, &this->right().interpolated_point, width);
}