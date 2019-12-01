#ifndef _CONSTANT_H_
#define _CONSTANT_H_

/*------------------------------------------------------*/

/*Constant(íËêî)*/

/*------------------------------------------------------*/

/*------------------------------------------------------*/
/*				< static_cast >*/
/*------------------------------------------------------*/

#define		scast_i(x)			static_cast<int>(x)
#define		scast_f(x)			static_cast<float>(x)
#define		scast_d(x)			static_cast<double>(x)
#define		scast_c(x)			static_cast<char>(x)
#define		scast_l(x)			static_cast<long>(x)
#define		scast_s(x)			static_cast<short>(x)


/*------------------------------------------------------*/
/*				< Angle Relation >*/
/*------------------------------------------------------*/

#define		PI					3.141592654f
#define		ToRadian(x)			x * (PI/180.f)
#define		ToDegree(x)			x * (180.f/PI)


/*------------------------------------------------------*/
/*				< Time Relation >*/
/*------------------------------------------------------*/

#define		ToSecond(x)			x = x / 60;
#define		ToFrame(x)			x = x * 60;

#endif //!_CONSTANT_H_