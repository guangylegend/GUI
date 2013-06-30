#include "math.h"
#include "ArcBall.h"

//轨迹球参数:
//直径					2.0f
//半径					1.0f
//半径平方				1.0f


void ArcBall_t::_mapToSphere(const Point2fT* NewPt, Vector3fT* NewVec) const
{
    Point2fT TempPt;
    GLfloat length;

    //复制到临时变量
    TempPt = *NewPt;

    //把长宽调整到[-1 ... 1]区间
    TempPt.s.X  =        (TempPt.s.X * this->AdjustWidth)  - 1.0f;
    TempPt.s.Y  = 1.0f - (TempPt.s.Y * this->AdjustHeight);

    //计算长度的平方
    length      = (TempPt.s.X * TempPt.s.X) + (TempPt.s.Y * TempPt.s.Y);

    //如果点映射到球的外面
    if (length > 1.0f)
    {
        GLfloat norm;

        //缩放到球上
        norm    = 1.0f / FuncSqrt(length);

        //设置z坐标为0
        NewVec->s.X = TempPt.s.X * norm;
        NewVec->s.Y = TempPt.s.Y * norm;
        NewVec->s.Z = 0.0f;
    }
	//如果在球内
    else
    {
        //利用半径的平方为1,求出z坐标
        NewVec->s.X = TempPt.s.X;
        NewVec->s.Y = TempPt.s.Y;
        NewVec->s.Z = FuncSqrt(1.0f - length);
    }
}

ArcBall_t::ArcBall_t(GLfloat NewWidth, GLfloat NewHeight)
{
    this->StVec.s.X     =
    this->StVec.s.Y     =
    this->StVec.s.Z     =

    this->EnVec.s.X     =
    this->EnVec.s.Y     =
    this->EnVec.s.Z     = 0.0f;

    this->setBounds(NewWidth, NewHeight);
}

//按下鼠标,记录当前对应的轨迹球的位置
void    ArcBall_t::click(const Point2fT* NewPt)
{
    this->_mapToSphere(NewPt, &this->StVec);
}

//鼠标拖动,计算旋转四元数
void    ArcBall_t::drag(const Point2fT* NewPt, Quat4fT* NewRot)
{
    //新的位置
    this->_mapToSphere(NewPt, &this->EnVec);

    //计算旋转
    if (NewRot)
    {
        Vector3fT  Perp;

        //计算旋转轴
        Vector3fCross(&Perp, &this->StVec, &this->EnVec);

        //如果不为0
        if (Vector3fLength(&Perp) > Epsilon)
        {
            //记录旋转轴
            NewRot->s.X = Perp.s.X;
            NewRot->s.Y = Perp.s.Y;
            NewRot->s.Z = Perp.s.Z;
            //在四元数中,w=cos(a/2)，a为旋转的角度
            NewRot->s.W= Vector3fDot(&this->StVec, &this->EnVec);
        }
		//是0，说明没有旋转
        else
        {
            NewRot->s.X =
            NewRot->s.Y =
            NewRot->s.Z =
            NewRot->s.W = 0.0f;
        }
    }
}

